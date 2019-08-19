#include "savefilesdialog.h"

#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QListWidget>
#include <QBoxLayout>
#include <QLabel>

#include "usermessages.h"

SaveFilesDialog::SaveFilesDialog(const QStringList &unsavedFiles, QWidget *pParent):
    QDialog (pParent)
{
    setWindowTitle(userMessages[UserMessages::PromptSaveTitle]);
    // creating window layout
    QVBoxLayout *pWdwLayout = new QVBoxLayout;
    pWdwLayout->addSpacing(20);

    // creating & adjusting message label
    QLabel *pInfoLbl = new QLabel;
    pInfoLbl->setText(userMessages[UserMessages::SaveAllQuestion]);
    pInfoLbl->setWordWrap(true);
    QFont lblFont("Segoe UI", 12);
    lblFont.setBold(true);
    pInfoLbl->setFont(lblFont);
    pWdwLayout->addWidget(pInfoLbl);
    pWdwLayout->addSpacing(20);

    // creating & adjusting files list
    QListWidget *pFilesList = new QListWidget;
    pFilesList->addItems(unsavedFiles);
    pWdwLayout->addWidget(pFilesList);

    // searching for the broadest item to set list width
    int maxWidth = pFilesList->sizeHintForColumn(0);
    for (int i = 1; i < pFilesList->count(); ++i)
    {
        maxWidth = std::max(maxWidth, pFilesList->sizeHintForColumn(i));
    }
    pFilesList->setMinimumWidth(maxWidth);

    // creating buttons
    pButtonBox = new QDialogButtonBox
            (QDialogButtonBox::StandardButton::YesToAll
             | QDialogButtonBox::StandardButton::NoToAll
             | QDialogButtonBox::StandardButton::Cancel);

    connect(pButtonBox, &QDialogButtonBox::clicked, this, &SaveFilesDialog::onClicked);

    pWdwLayout->addWidget(pButtonBox);
    setLayout(pWdwLayout);
}

QDialogButtonBox::StandardButton SaveFilesDialog::start()
{
    exec();
    // return enum of pressed button
    return pressedButton;
}

void SaveFilesDialog::onClicked(QAbstractButton *pButton)
{
    // ascertain which button was pressed
    pressedButton = pButtonBox->standardButton(pButton);
    accept();
}
