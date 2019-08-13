#include "documentmanager.h"

#include <QMdiSubWindow>
#include <QMessageBox>
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
    qDebug() << "found item " << pOpenedDoc;

    if (pOpenedDoc)
    {
        for (auto& area: mDocAreas)
        {
            if (area->subWindowList().contains(pOpenedDoc))
            {
                qDebug() << "contains";
                area->setActiveSubWindow(pOpenedDoc);
            }
        }
        return;
    }

    // create new view
    CodeEditor *newView = new CodeEditor;
    newView->setFileName(fileName);
    newView->setFocusPolicy(Qt::StrongFocus);

    // place view
    auto placementArea = selectAreaForPlacement();

    if (!placementArea)
    {
        throw QException();
    }
    qDebug() << "placement area: " << placementArea;
    placementArea->addSubWindow(newView);
    newView->setWindowState(Qt::WindowMaximized);

    if (load)
    {
        loadFile(newView, fileName);
    }

    int position = fileName.lastIndexOf(QChar{'/'});
    newView->setWindowTitle(fileName.mid(position + 1));
    newView->setBeginTextState();
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

void DocumentManager::onFocusChanged(QWidget *old, QWidget *now)
{
    auto prevWgtInFocus = qobject_cast<CodeEditor*>(old);
    if (prevWgtInFocus)
    {
        qDebug() << prevWgtInFocus << " is CodeEditor";
        mpPrevEditorInFocus = prevWgtInFocus;
        return;
    }
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
        qDebug() << "empty area";
        return *placementArea;
    }

    auto pAreaInFocus = areaInFocus();
    qDebug() << "in focus " << pAreaInFocus;
    return pAreaInFocus ? pAreaInFocus : mDocAreas.front();
}

QMdiSubWindow* DocumentManager::openedDoc(const QString &fileName)
{
    qDebug() << "openedDoc";
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
            qDebug() << "found";
            return *openedDocIter;
        }
    }

    // if document is opened - ptr to it is returned
    // otherwise null is returned
    return nullptr;
}

QMdiArea* DocumentManager::areaInFocus()
{
    qDebug() << "wgt in focus app " << QApplication::focusWidget();
    QList<QMdiSubWindow*>::const_iterator areaInFocusIter;

    for (const auto& area: mDocAreas)
    {
        auto subWdwList = area->subWindowList();

        areaInFocusIter = std::find_if(subWdwList.cbegin(), subWdwList.cend(),
                 [this](const auto& doc)
        {
            qDebug() << "prev editor in focus "
                     << (static_cast<CodeEditor*>(doc->widget()) == mpPrevEditorInFocus);
            return static_cast<CodeEditor*>(doc->widget()) == mpPrevEditorInFocus;
        });

        if (areaInFocusIter != subWdwList.end())
        {
            qDebug() << "found";
            return area;
        }
    }

    // if document is in focus - ptr to it is returned
    // otherwise null is returned
    return nullptr;
}
