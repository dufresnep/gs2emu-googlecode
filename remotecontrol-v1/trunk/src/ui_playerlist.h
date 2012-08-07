/********************************************************************************
** Form generated from reading ui file 'playerlist.ui'
**
** Created: Sat May 3 07:12:36 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PLAYERLIST_H
#define UI_PLAYERLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include "TPlayerTree.h"

class Ui_FPlayerWindow
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QPushButton *pAdminMsg;
    QPushButton *pClose;
    QPushButton *pPrivMsg;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout2;
    TPlayerTree *treeWidget;

    void setupUi(QWidget *FPlayerWindow)
    {
    if (FPlayerWindow->objectName().isEmpty())
        FPlayerWindow->setObjectName(QString::fromUtf8("FPlayerWindow"));
    FPlayerWindow->resize(436, 313);
    gridLayout = new QGridLayout(FPlayerWindow);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout1 = new QGridLayout();
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(6);
    gridLayout1->setVerticalSpacing(6);
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    pAdminMsg = new QPushButton(FPlayerWindow);
    pAdminMsg->setObjectName(QString::fromUtf8("pAdminMsg"));

    gridLayout1->addWidget(pAdminMsg, 0, 0, 1, 1);

    pClose = new QPushButton(FPlayerWindow);
    pClose->setObjectName(QString::fromUtf8("pClose"));

    gridLayout1->addWidget(pClose, 0, 3, 1, 1);

    pPrivMsg = new QPushButton(FPlayerWindow);
    pPrivMsg->setObjectName(QString::fromUtf8("pPrivMsg"));

    gridLayout1->addWidget(pPrivMsg, 0, 1, 1, 1);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem, 0, 2, 1, 1);


    gridLayout->addLayout(gridLayout1, 1, 0, 1, 1);

    gridLayout2 = new QGridLayout();
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(6);
    gridLayout2->setVerticalSpacing(6);
    gridLayout2->setContentsMargins(0, 0, 0, 0);
    treeWidget = new TPlayerTree(FPlayerWindow);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setLineWidth(1);
    treeWidget->setMidLineWidth(0);
    treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    treeWidget->setIndentation(8);
    treeWidget->setRootIsDecorated(false);
    treeWidget->setUniformRowHeights(true);
    treeWidget->setItemsExpandable(false);
    treeWidget->setSortingEnabled(true);
    treeWidget->setColumnCount(4);

    gridLayout2->addWidget(treeWidget, 0, 0, 1, 1);


    gridLayout->addLayout(gridLayout2, 0, 0, 1, 1);


    retranslateUi(FPlayerWindow);

    QMetaObject::connectSlotsByName(FPlayerWindow);
    } // setupUi

    void retranslateUi(QWidget *FPlayerWindow)
    {
    FPlayerWindow->setWindowTitle(QApplication::translate("FPlayerWindow", "Form", 0, QApplication::UnicodeUTF8));
    pAdminMsg->setText(QApplication::translate("FPlayerWindow", "&Admin Message to All", 0, QApplication::UnicodeUTF8));
    pClose->setText(QApplication::translate("FPlayerWindow", "&Close", 0, QApplication::UnicodeUTF8));
    pPrivMsg->setText(QApplication::translate("FPlayerWindow", "&Private Message to All", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("FPlayerWindow", "Nickname", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(1, QApplication::translate("FPlayerWindow", "Account", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(2, QApplication::translate("FPlayerWindow", "Level", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(3, QApplication::translate("FPlayerWindow", "ID", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FPlayerWindow);
    } // retranslateUi

};

namespace Ui {
    class FPlayerWindow: public Ui_FPlayerWindow {};
} // namespace Ui

#endif // UI_PLAYERLIST_H
