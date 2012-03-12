#include "search_dialog.h"
#include <stdio.h>

SearchDialog::SearchDialog(QWidget *parent): QDialog(parent)
{
    // Title
    setWindowTitle(tr("Search and replace"));
    setWindowIcon(QIcon(QString(ICON_PATH) + "/search.png"));

    // Layout
    QVBoxLayout *vlayout = new QVBoxLayout();
    setLayout(vlayout);

    // Search
    QGroupBox *searchGroup = new QGroupBox(this);
    searchGroup->setTitle(tr("Search"));
    vlayout->addWidget(searchGroup);

    QVBoxLayout *searchLayout = new QVBoxLayout();
    searchGroup->setLayout(searchLayout);

    QLabel *searchLabel = new QLabel(searchGroup);
    searchLabel->setText(tr("Search"));
    searchLayout->addWidget(searchLabel);

    search = new QLineEdit(searchGroup);
    searchLayout->addWidget(search);

    searchRegExp = new QCheckBox(searchGroup);
    searchRegExp->setText(tr("Regular expression"));
    searchLayout->addWidget(searchRegExp);
    //searchRegExp->hide();

    // Replace
    QGroupBox *replaceGroup = new QGroupBox(this);
    replaceGroup->setTitle(tr("Replace"));
    vlayout->addWidget(replaceGroup);

    QVBoxLayout *replaceLayout = new QVBoxLayout();
    replaceGroup->setLayout(replaceLayout);

    QLabel *replaceLabel = new QLabel(replaceGroup);
    replaceLabel->setText(tr("Replace"));
    replaceLayout->addWidget(replaceLabel);

    replace = new QLineEdit(replaceGroup);
    replaceLayout->addWidget(replace);

    // Options
    QGroupBox *optionsGroup = new QGroupBox(this);
    optionsGroup->setTitle(tr("Options"));
    vlayout->addWidget(optionsGroup);

    QHBoxLayout *optionsLayout = new QHBoxLayout();
    optionsGroup->setLayout(optionsLayout);

    QVBoxLayout *optionsV1Layout = new QVBoxLayout();
    optionsLayout->addLayout(optionsV1Layout);

    QVBoxLayout *optionsV2Layout = new QVBoxLayout();
    optionsLayout->addLayout(optionsV2Layout);

    caseSensitiveCB = new QCheckBox(optionsGroup);
    caseSensitiveCB->setText(tr("Case sensitive"));
    optionsV1Layout->addWidget(caseSensitiveCB);

    wholeWordsCB = new QCheckBox(optionsGroup);
    wholeWordsCB->setText(tr("Only full words"));
    optionsV1Layout->addWidget(wholeWordsCB);

    fromCursorCB = new QCheckBox(optionsGroup);
    fromCursorCB->setText(tr("From cursor"));
    optionsV2Layout->addWidget(fromCursorCB);

    // Buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    vlayout->addLayout(buttonsLayout);

    QPushButton *acceptButton = new QPushButton(this);
    acceptButton->setText(tr("Search"));
    buttonsLayout->addWidget(acceptButton);
    connect(acceptButton, SIGNAL(clicked()), this, SIGNAL(search_signal()));

    QPushButton *replaceButton = new QPushButton(this);
    replaceButton->setText(tr("Replace"));
    buttonsLayout->addWidget(replaceButton);
    connect(replaceButton, SIGNAL(clicked()), this, SIGNAL(replace_signal()));

    QPushButton *cancelButton = new QPushButton(this);
    cancelButton->setText(tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(accept()));
}

SearchDialog::~SearchDialog()
{
	printf("SearchDialog eliminado\n");
}
