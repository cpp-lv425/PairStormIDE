#include "documentmanager.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include <algorithm>
#include <QSplitter>
#include <algorithm>
#include <QMdiArea>
#include <QVector>
#include <QDebug>

#include "usermessages.h"
#include "filemanager.h"
#include "codeeditor.h"
#include "utils.h"

DocumentManager::DocumentManager()
{
    mpSplitter = new QSplitter;
    mpSplitter->setChildrenCollapsible(false);
    splitWindow();
    connect(qApp, &QApplication::focusChanged, this, &DocumentManager::onFocusChanged);
}

void DocumentManager::splitWindow()
{
    QMdiArea *pNewArea = createMdiArea();
    mpSplitter->addWidget(pNewArea);
    mDocAreas.push_back(pNewArea);
}

QSplitter* DocumentManager::getSplitter()
{
    return mpSplitter;
}

void DocumentManager::openDocument(const QString &fileName, bool load)
{
    auto pOpenedDoc = openedDoc(fileName);

    if (pOpenedDoc)
    {
        for (auto& area: mDocAreas)
        {
            if (area->subWindowList().contains(pOpenedDoc))
            {
                area->setActiveSubWindow(pOpenedDoc);
            }
        }
        return;
    }

    // create new view
    CodeEditor *newView = new CodeEditor;
    connect(newView, &CodeEditor::closeDocEventOccured, this, &DocumentManager::onCloseDocument);
    newView->setFileName(fileName);
    newView->setFocusPolicy(Qt::StrongFocus);

    // place view
    auto placementArea = selectAreaForPlacement();

    if (!placementArea)
    {
        throw QException();
    }
    placementArea->addSubWindow(newView);
    newView->setWindowState(Qt::WindowMaximized);

    if (load)
    {
        try
        {
            loadFile(newView, fileName);
        } catch (const QException&)
        {
            throw;
        }
    }

    int position = fileName.lastIndexOf(QChar{'/'});
    newView->setWindowTitle(fileName.mid(position + 1));
    newView->setBeginTextState();
}

bool DocumentManager::saveDocument()
{
    auto pCurrentDocument = getCurrentDocument();

    if (!pCurrentDocument)
    {
        return false;
    }

    if (!pCurrentDocument->isChanged())
    {
        return false;
    }

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

    pCurrentDocument->setBeginTextState();
    return true;
}

bool DocumentManager::saveAllDocuments()
{
    bool savedChanges = false;

    for (const auto& area : mDocAreas)
    {
        auto openedDocs = area->subWindowList();

        for (const auto& subWdw : openedDocs)
        {
            try
            {
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
        saveDocument(fileName, currentDocument->toPlainText());
    }
    catch (const QException&)
    {
        throw;
    }

    currentDocument->setFileName(fileName);
    int position = fileName.lastIndexOf(QChar{'/'});
    currentDocument->setWindowTitle(fileName.mid(position + 1));
    currentDocument->setBeginTextState();
}

void DocumentManager::loadFile(CodeEditor *newView, const QString &fileName)
{
    QString readResult;

    try
    {
        readResult = FileManager().readFromFile(fileName);
    }
    catch (const QException&)
    {
        throw;
    }

    newView->setPlainText(readResult);
}

void DocumentManager::onSplit(Qt::Orientation orientation)
{        
    if (orientation == mpSplitter->orientation())
    {
        splitWindow();
        return;
    }

    mpSplitter->setOrientation(orientation);
}

void DocumentManager::onFocusChanged(QWidget *old, QWidget *)
{
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
    QMdiArea *pMdiArea = new QMdiArea;
    pMdiArea->setTabsClosable(true);
    pMdiArea->setViewMode(QMdiArea::TabbedView);
    return pMdiArea;
}

QMdiArea* DocumentManager::selectAreaForPlacement()
{
    if (!mDocAreas.size())
    {
        return nullptr;
    }

    auto placementArea = std::find_if(mDocAreas.cbegin(), mDocAreas.cend(), [](const auto& area)
    {
        return !area->subWindowList().size();
    });

    if (placementArea != mDocAreas.end())
    {
        return *placementArea;
    }

    auto pAreaInFocus = lastAreaInFocus();
    return pAreaInFocus ? pAreaInFocus : mDocAreas.front();
}

QMdiSubWindow* DocumentManager::openedDoc(const QString &fileName)
{
    QList<QMdiSubWindow*>::const_iterator openedDocIter;

    for (const auto& area: mDocAreas)
    {
        auto subWdwList = area->subWindowList();

        openedDocIter = std::find_if(subWdwList.cbegin(), subWdwList.cend(),
                                     [&fileName](const auto& doc)
        {
            return static_cast<CodeEditor*>(doc->widget())->getFileName() == fileName;
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
    QList<QMdiSubWindow*>::const_iterator areaInFocusIter;

    if (!mpPrevEditorInFocus)
    {
        return nullptr;
    }

    for (const auto& area: mDocAreas)
    {
        auto subWdwList = area->subWindowList();

        areaInFocusIter = std::find_if(subWdwList.cbegin(), subWdwList.cend(),
                                       [this](const auto& doc)
        {
            return static_cast<CodeEditor*>(doc->widget()) == mpPrevEditorInFocus;
        });

        if (areaInFocusIter != subWdwList.end())
        {
            qDebug() << "found last area in focus";
            return area;
        }
    }

    // if document is in focus - ptr to it is returned
    // otherwise null is returned
    return nullptr;
}

QMdiArea *DocumentManager::areaInFocus()
{
    QList<QMdiSubWindow*>::const_iterator areaInFocusIter;

    for (const auto& area: mDocAreas)
    {
        auto subWdwList = area->subWindowList();

        areaInFocusIter = std::find_if(subWdwList.cbegin(), subWdwList.cend(),
                                       [](const auto& doc)
        {
            return static_cast<CodeEditor*>(doc->widget())->hasFocus();
        });

        if (areaInFocusIter != subWdwList.end())
        {
            return area;
        }
    }

    // if document is in focus - ptr to it is returned
    // otherwise null is returned
    return nullptr;
}

QMdiArea* DocumentManager::getArea(CodeEditor *doc)
{
    QList<QMdiSubWindow*>::const_iterator areaIter;

    for (const auto& area: mDocAreas)
    {
        auto subWdwList = area->subWindowList();

        areaIter = std::find_if(subWdwList.cbegin(), subWdwList.cend(),
                                [&doc](const auto& document)
        {
            return static_cast<CodeEditor*>(document->widget()) == doc;
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
    if (mDocAreas.size() < 2)
    {
        auto pCurrentWdw = mDocAreas.front()->currentSubWindow();
        return pCurrentWdw ? qobject_cast<CodeEditor*>(pCurrentWdw->widget()) : nullptr;
    }

    auto pAreaInFocus = areaInFocus();
    if (!pAreaInFocus)
    {
        return nullptr;
    }

    auto pCurrentDocument = pAreaInFocus->currentSubWindow();
    return pCurrentDocument ? static_cast<CodeEditor*>(pCurrentDocument->widget()) : nullptr;
}

void DocumentManager::closeCurrentDocument()
{
    auto pCurrentDocument = getCurrentDocument();

    if (!pCurrentDocument)
    {
        return;
    }

    auto pCurrentSubWdw = qobject_cast<QMdiSubWindow*>(pCurrentDocument->parent());

    if (pCurrentSubWdw)
    {
        pCurrentSubWdw->close();
    }
}

QVector<CodeEditor*> DocumentManager::getChangedDocuments()
{
    QVector<CodeEditor*> changedDocuments;

    for (const auto& area : mDocAreas)
    {
        auto windowsList = area->subWindowList();

        if (windowsList.size())
        {
            std::for_each(windowsList.begin(), windowsList.end(),
                          [&changedDocuments](const auto& wdw)
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

bool DocumentManager::saveDocument(CodeEditor *doc)
{
    if (!doc || !doc->isChanged())
    {
        return false;
    }
    try
    {
        FileManager().writeToFile(doc->getFileName(), doc->toPlainText());
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
