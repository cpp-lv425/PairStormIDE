#include "documentmanager.h"

#include <QMdiSubWindow>
#include <QDirIterator>
#include <QMessageBox>
#include <QSplitter>
#include <algorithm>
#include <QMdiArea>
#include <QVector>
#include <QDebug>
#include <QDir>

#include "usermessages.h"
#include "filemanager.h"
#include "codeeditor.h"
#include "utils.h"

DocumentManager::DocumentManager()
{
    mpSplitter = new QSplitter;
    mpSplitter->setChildrenCollapsible(false);

    // first doc area is created
    splitWindow();
    connect(qApp, &QApplication::focusChanged, this, &DocumentManager::onFocusChanged);
}

void DocumentManager::splitWindow()
{
    // creates new doc area & adds it to shown doc areas
    QMdiArea *pNewArea = createMdiArea();
    mpSplitter->addWidget(pNewArea);
    mDocAreas.push_back(pNewArea);
}

QSplitter* DocumentManager::getSplitter()
{
    return mpSplitter;
}

void DocumentManager::openProject(const QString &path)
{
    currentProject = path;
}

const QString& DocumentManager::getCurrentProjectPath() const
{
    return  currentProject;
}

void DocumentManager::closeCurrentProject()
{
    currentProject.clear();
}

void DocumentManager::openDocument(const QString &fileName, bool load)
{
    // checks if doc is not already opened
    auto pOpenedDoc = openedDoc(fileName);
    // if doc is already opened - it becomes active
    if (pOpenedDoc)
    {
        for (auto &area: mDocAreas)
        {
            if (area->subWindowList().contains(pOpenedDoc))
            {
                area->setActiveSubWindow(pOpenedDoc);
                return;
            }
        }
    }

    // create new view
    CodeEditor *newView = createDoc(fileName);

    // select doc area to accomodate new doc
    auto placementArea = selectAreaForPlacement();

    if (!placementArea)
    {
        throw DocumentPlacementFailure();
    }

    // doc is added to doc area & unfolded
    placementArea->addSubWindow(newView);
    newView->setWindowState(Qt::WindowMaximized);

    // if necessary - doc content is read from file & placed in doc
    if (load)
    {
        try
        {
            loadFile(newView, fileName);
        } catch (const FileOpeningFailure&)
        {
            throw;
        }
    }

    // doc name is set on tab
    int position = fileName.lastIndexOf(QChar{'/'});
    newView->setWindowTitle(fileName.mid(position + 1));
    // doc snaps current content state
    newView->setBeginTextState();
}

bool DocumentManager::saveDocument()
{
    // receive current doc
    auto pCurrentDocument = getCurrentDocument();

    if (!pCurrentDocument)
    {
        return false;
    }

    // check if doc was modified
    if (!pCurrentDocument->isChanged())
    {
        return false;
    }

    // content is written to file
    try
    {
        FileManager().writeToFile
                (pCurrentDocument->getFileName(),
                 pCurrentDocument->toPlainText());

    }
    catch (const FileOpeningFailure&)
    {
        throw;
    }

    // doc snaps current content state
    pCurrentDocument->setBeginTextState();
    return true;
}

bool DocumentManager::saveAllDocuments()
{
    bool savedChanges = false;

    // saves changes to every doc in every doc area
    for (const auto &area : mDocAreas)
    {
        auto openedDocs = area->subWindowList();

        for (const auto &subWdw : openedDocs)
        {
            try
            {
                // savedChanges is set to true if at lease one document
                // was saved
                savedChanges |= saveDocument(qobject_cast<CodeEditor*>(subWdw->widget()));
            }
            catch (const FileOpeningFailure&)
            {
                throw;
            }
        }
    }
    return savedChanges;
}

void DocumentManager::saveDocumentAs(CodeEditor *currentDocument, const QString &fileName)
{
    try
    {
        // doc is saved using new file name
        saveDocument(fileName, currentDocument->toPlainText());
    }
    catch (const QException&)
    {
        throw;
    }

    // opened doc represents newly created file
    currentDocument->setFileName(fileName);
    int position = fileName.lastIndexOf(QChar{'/'});
    currentDocument->setWindowTitle(fileName.mid(position + 1));
    // doc snaps current content state
    currentDocument->setBeginTextState();
}

void DocumentManager::loadFile(CodeEditor *newView, const QString &fileName)
{
    QString readResult;

    try
    {
        readResult = FileManager().readFromFile(fileName);
    }
    catch (const FileOpeningFailure&)
    {
        throw;
    }
    // loads file content to opened doc
    newView->setPlainText(readResult);
}

void DocumentManager::onSplit(Qt::Orientation orientation)
{
    // if number of doc areas is less than 2 -
    // then new doc area is added & orientation is set according to
    if (mDocAreas.size() < 2)
    {
        mpSplitter->setOrientation(orientation);
        splitWindow();
        return;
    }
    // if current orientation matches passed arg
    // then new doc area is created
    if (orientation == mpSplitter->orientation())
    {
        splitWindow();
        return;
    }

    // if current orientation doesn't match passed arg
    // orientation is changed
    mpSplitter->setOrientation(orientation);
}

void DocumentManager::onFocusChanged(QWidget *old, QWidget *)
{
    // method is used when document loses focus upon user input
    // (e.g. when Project Viewer item is clicked to open another document)

    // if previous wgt in focus is CodeEditor - we save ptr to it
    auto prevWgtInFocus = qobject_cast<CodeEditor*>(old);
    if (prevWgtInFocus)
    {
        mpPrevEditorInFocus = prevWgtInFocus;
        return;
    }
}

void DocumentManager::onCloseDocument(CodeEditor *doc)
{
    // if only one doc area left - it will not be removed
    if (mDocAreas.size() == 1)
    {
        return;
    }

    // find area to be removed
    auto placementArea = getArea(doc);

    // if area is not found or has other opened docs
    if (!placementArea || placementArea->subWindowList().size() > 1)
    {
        return;
    }

    // if we remove doc we must make sure that ptr to it (mpPrevEditorInFocus)
    // is brought to safe condition
    if (doc == mpPrevEditorInFocus)
    {
        mpPrevEditorInFocus = nullptr;
    }

    // area is removed from container
    mDocAreas.removeOne(placementArea);

    // area wgt is scheduled for deletion
    placementArea->deleteLater();
}

QMdiArea* DocumentManager::createMdiArea()
{
    // creating new doc area
    QMdiArea *pMdiArea = new QMdiArea;
    pMdiArea->setTabsClosable(true);
    pMdiArea->setViewMode(QMdiArea::TabbedView);
    return pMdiArea;
}

CodeEditor* DocumentManager::createDoc(const QString &fileName)
{
    // create new view
    CodeEditor *newView = new CodeEditor;
    connect(newView, &CodeEditor::closeDocEventOccured, this, &DocumentManager::onCloseDocument);
    newView->setFileName(fileName);
    newView->setFocusPolicy(Qt::StrongFocus);
    return newView;
}

QMdiArea* DocumentManager::selectAreaForPlacement()
{
    // check if there are any doc areas
    if (!mDocAreas.size())
    {
        return nullptr;
    }

    // I. Search for first doc area without opened docs
    auto placementArea = std::find_if(mDocAreas.cbegin(), mDocAreas.cend(),
                                      [](const auto &area)
    {
        return !area->subWindowList().size();
    });

    if (placementArea != mDocAreas.end())
    {
        return *placementArea;
    }

    // II. If all areas are populated - we use last area in focus
    auto pAreaInFocus = lastAreaInFocus();

    // III. If we fail to find last area in focus - we use front doc area
    return pAreaInFocus ? pAreaInFocus : mDocAreas.front();
}

QMdiSubWindow* DocumentManager::openedDoc(const QString &fileName)
{
    QList<QMdiSubWindow*>::const_iterator openedDocIter;

    // search for doc with specified name in every area
    for (const auto &area: mDocAreas)
    {
        auto subWdwList = area->subWindowList();

        openedDocIter = std::find_if(subWdwList.cbegin(), subWdwList.cend(),
                                     [&fileName](const auto &wdw)
        {
            auto doc = qobject_cast<CodeEditor*>(wdw->widget());
            return doc ? doc->getFileName() == fileName : false;
        });

        if (openedDocIter != subWdwList.end())
        {
            return *openedDocIter;
        }
    }

    // if document is opened - ptr to it is returned
    // otherwise null is returned
    return nullptr;
}

QMdiArea* DocumentManager::lastAreaInFocus()
{
    // if prev document in focus was closed - null is returned to indicate search failure
    if (!mpPrevEditorInFocus)
    {
        return nullptr;
    }

    // search for area which accomodates last doc in focus
    for (const auto &area: mDocAreas)
    {
        auto currentWindow = area->currentSubWindow();

        if (currentWindow)
        {
            auto doc = qobject_cast<CodeEditor*>(currentWindow->widget());
            if (doc && (doc == mpPrevEditorInFocus))
            {
                return area;
            }
        }
    }

    // if document-to-pointer is equal to pointer to previous document in focus
    // then pointer to it is returned
    // otherwise null is returned
    return nullptr;
}

QMdiArea* DocumentManager::areaInFocus()
{
    // search for area which accomodates current doc in focus
    for (const auto &area: mDocAreas)
    {
        auto currentWindow = area->currentSubWindow();

        if (currentWindow)
        {
            auto doc = qobject_cast<CodeEditor*>(currentWindow->widget());

            if (doc && doc->hasFocus())
            {
                return area;
            }
        }
    }

    // if document is in focus - ptr to it is returned
    // otherwise null is returned
    return nullptr;
}

QMdiArea* DocumentManager::getArea(CodeEditor *doc)
{
    QList<QMdiSubWindow*>::const_iterator areaIter;
    // search for area which accomodates current doc
    for (const auto &area: mDocAreas)
    {
        auto subWdwList = area->subWindowList();

        areaIter = std::find_if(subWdwList.cbegin(), subWdwList.cend(),
                                [&doc](const auto &document)
        {
            auto currentDoc = qobject_cast<CodeEditor*>(document->widget());
            return currentDoc ? (currentDoc == doc) : false;
        });

        if (areaIter != subWdwList.end())
        {
            return area;
        }
    }

    // if document is found - ptr to it is returned
    // otherwise null is returned
    return nullptr;
}

CodeEditor* DocumentManager::getCurrentDocument()
{
    // if there is only one doc area, we receive current sub wdw from it
    // if current sub wdw is null - we return null to indicate search failure
    if (mDocAreas.size() < 2)
    {
        auto pCurrentWindow = mDocAreas.front()->currentSubWindow();
        return pCurrentWindow ? qobject_cast<CodeEditor*>(pCurrentWindow->widget()) : nullptr;
    }

    // search for area in focus
    auto pAreaInFocus = areaInFocus();
    if (!pAreaInFocus)
    {
        return nullptr;
    }
    // receive current sub wdw from area in focus
    // if current sub wdw is null - we return null to indicate search failure
    auto pCurrentDocument = pAreaInFocus->currentSubWindow();
    return pCurrentDocument ? qobject_cast<CodeEditor*>(pCurrentDocument->widget()) : nullptr;
}

void DocumentManager::closeCurrentDocument()
{
    // get current document
    auto pCurrentDocument = getCurrentDocument();

    if (!pCurrentDocument)
    {
        return;
    }

    // call close() on parent wgt which is QMdiSubWindow
    auto pCurrentSubWdw = qobject_cast<QMdiSubWindow*>(pCurrentDocument->parent());

    if (pCurrentSubWdw)
    {
        pCurrentSubWdw->close();
    }
}

void DocumentManager::closeAllDocumentsWithoutSaving()
{
    // in order to close documents without saving changes
    // state of documents is set to "not modified"
    setAllDocumentsNotModified();

    // all doc areas but first are closed with all nested documents
    while (mDocAreas.size() > 1)
    {
        auto pDocArea = mDocAreas.back();
        mDocAreas.pop_back();
        pDocArea->close();
    }

    // all windows of the first doc area are closed
    auto windowsList = mDocAreas.front()->subWindowList();
    for (const auto &window: windowsList)
    {
        window->close();
    }
}

QVector<CodeEditor*> DocumentManager::getChangedDocuments()
{
    QVector<CodeEditor*> changedDocuments;

    // searches for modified docs through all doc areas
    // pointers to modified docs are added to container
    for (const auto &area : mDocAreas)
    {
        auto windowsList = area->subWindowList();

        if (windowsList.size())
        {
            std::for_each(windowsList.begin(), windowsList.end(),
                          [&changedDocuments](const auto &wdw)
            {
                auto doc = qobject_cast<CodeEditor*>(wdw->widget());

                if (doc && doc->isChanged())
                {
                    changedDocuments.push_back(doc);
                }
            });
        }
    }
    return changedDocuments;
}

void DocumentManager::combineDocAreas()
{
    if (mDocAreas.size() < 2)
    {
        return;
    }

    // loop is executed until single doc area
    while (mDocAreas.size() > 1)
    {
        auto areaIter = mDocAreas.begin() + 1;

        // receive the list of windows of current doc area
        auto windowsList = (*areaIter)->subWindowList();

        if (windowsList.size())
        {
            for (const auto &wdw: windowsList)
            {
                // every window is detached from current doc area
                // & placed on the first doc area
                auto doc = qobject_cast<CodeEditor*>(wdw->widget());

                if (doc)
                {
                    // window is removed from doc area
                    (*areaIter)->removeSubWindow(wdw);
                    QString fileName = doc->getFileName();

                    // new doc view is created
                    CodeEditor *newView = createDoc(fileName);

                    // text from opened document is placed on new document view
                    newView->setPlainText(doc->toPlainText());

                    // initial document state is passed to new view
                    // in order to keep track of doc modification
                    newView->setTextState(doc->getBeginTextState());

                    // new view is placed on front doc area
                    mDocAreas.front()->addSubWindow(newView);
                    newView->setWindowState(Qt::WindowMaximized);

                    // doc name is set on tab
                    int position = fileName.lastIndexOf(QChar{'/'});
                    newView->setWindowTitle(fileName.mid(position + 1));

                    delete wdw;
                }
            }
        }

        // current doc area is closed & scheduled for deletion
        (*areaIter)->deleteLater();
        mDocAreas.erase(areaIter);
    }
}

void DocumentManager::closeEmptyDocArea()
{
    if (mDocAreas.size() < 2)
    {
        return;
    }

    // closing empty rightmost doc area
    auto rightDocArea = mDocAreas.back();

    if (!rightDocArea->currentSubWindow())
    {
        // area is removed from container
        mDocAreas.pop_back();

        // area wgt is scheduled for deletion
        rightDocArea->deleteLater();
    }
}

bool DocumentManager::fileBelongsToCurrentProject(const QString &fileName) const
{    
    QDirIterator dirIter(currentProject, QDir::Files, QDirIterator::Subdirectories);

    while (dirIter.hasNext())
    {
        dirIter.next();        
        if (dirIter.filePath() == fileName)
        {
            return true;
        }
    }
    return false;
}

bool DocumentManager::projectOpened()
{
    // if project name is written then project is opened
    return currentProject.size();
}

void DocumentManager::applyChangesToCurrentDocument(std::function<void (CodeEditor*)> functor)
{
    auto pCurrentDocument = getCurrentDocument();

    if (pCurrentDocument)
    {
        functor(pCurrentDocument);
    }
}

void DocumentManager::configureDocuments(std::function<void(DocumentManager*, CodeEditor*, const QString&)> functor,
                                         const QString &newValue)
{
    // applying new settings to every opened doc view
    for (const auto &area: mDocAreas)
    {
        auto windowsList = area->subWindowList();

        std::for_each(windowsList.begin(), windowsList.end(),
                      [this, &functor, &newValue](const auto &wdw)
        {
            auto doc = qobject_cast<CodeEditor*>(wdw->widget());
            if (doc)
            {
                functor(this, doc, newValue);
            }
        });
    }
}

void DocumentManager::setStyle(CodeEditor *doc, const QString &styleName)
{
    doc->setIdeType(styleName);
    doc->highlighText();
}

void DocumentManager::setFontFamily(CodeEditor *doc, const QString &fontFamily)
{
    doc->setFontStyle(fontFamily);
    doc->highlighText();
}

void DocumentManager::setFontSize(CodeEditor *doc, const QString &fontSize)
{
    doc->setFontSize(fontSize);
    doc->highlighText();
}

bool DocumentManager::saveDocument(CodeEditor *doc)
{
    // if doc wasn't modified - then return
    if (!doc || !doc->isChanged())
    {
        return false;
    }
    // save document content to file
    try
    {
        FileManager().writeToFile(doc->getFileName(), doc->toPlainText());
        // snap current doc content state
        doc->setBeginTextState();
        return true;
    }
    catch (const FileOpeningFailure&)
    {
        throw;
    }
}

void DocumentManager::saveDocument(const QString &fileName, const QString &fileContent)
{    
    try
    {
        FileManager().writeToFile
                (fileName,
                 fileContent);
    }
    catch (const FileOpeningFailure&)
    {
        throw;
    }
}

void DocumentManager::setAllDocumentsNotModified()
{
    for (auto &area: mDocAreas)
    {
        auto windowsList = area->subWindowList();
        for (auto &window: windowsList)
        {
            auto doc = qobject_cast<CodeEditor*>(window->widget());
            if (doc)
            {
                doc->setBeginTextState();
            }
        }
    }
}
