/********************************************************************************
** Form generated from reading ui file 'rcmain.ui'
**
** Created: Thu May 1 02:47:56 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_RCMAIN_H
#define UI_RCMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_FRCWindow
{
public:
    QAction *actionPlayerlist;
    QAction *actionAccounts;
    QAction *actionToalls;
    QAction *actionFile_Browser;
    QAction *actionRC_Options;
    QAction *actionServer_Options;
    QAction *actionFolder_Configuration;
    QAction *actionNPCs;
    QAction *actionClasses;
    QAction *actionWeapons_GUI;
    QAction *actionServer_Flags;
    QAction *actionLevel_List;
    QAction *actionLevel_NPC_Dump;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout1;
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QMenuBar *menubar;
    QMenu *menuPlayers;
    QMenu *menuFiles;
    QMenu *menuConfiguration;
    QMenu *menuScripts;
    QMenu *menuMisc;

    void setupUi(QMainWindow *FRCWindow)
    {
    if (FRCWindow->objectName().isEmpty())
        FRCWindow->setObjectName(QString::fromUtf8("FRCWindow"));
    FRCWindow->resize(516, 245);
    actionPlayerlist = new QAction(FRCWindow);
    actionPlayerlist->setObjectName(QString::fromUtf8("actionPlayerlist"));
    actionAccounts = new QAction(FRCWindow);
    actionAccounts->setObjectName(QString::fromUtf8("actionAccounts"));
    actionToalls = new QAction(FRCWindow);
    actionToalls->setObjectName(QString::fromUtf8("actionToalls"));
    actionFile_Browser = new QAction(FRCWindow);
    actionFile_Browser->setObjectName(QString::fromUtf8("actionFile_Browser"));
    actionRC_Options = new QAction(FRCWindow);
    actionRC_Options->setObjectName(QString::fromUtf8("actionRC_Options"));
    actionServer_Options = new QAction(FRCWindow);
    actionServer_Options->setObjectName(QString::fromUtf8("actionServer_Options"));
    actionFolder_Configuration = new QAction(FRCWindow);
    actionFolder_Configuration->setObjectName(QString::fromUtf8("actionFolder_Configuration"));
    actionNPCs = new QAction(FRCWindow);
    actionNPCs->setObjectName(QString::fromUtf8("actionNPCs"));
    actionClasses = new QAction(FRCWindow);
    actionClasses->setObjectName(QString::fromUtf8("actionClasses"));
    actionWeapons_GUI = new QAction(FRCWindow);
    actionWeapons_GUI->setObjectName(QString::fromUtf8("actionWeapons_GUI"));
    actionServer_Flags = new QAction(FRCWindow);
    actionServer_Flags->setObjectName(QString::fromUtf8("actionServer_Flags"));
    actionLevel_List = new QAction(FRCWindow);
    actionLevel_List->setObjectName(QString::fromUtf8("actionLevel_List"));
    actionLevel_NPC_Dump = new QAction(FRCWindow);
    actionLevel_NPC_Dump->setObjectName(QString::fromUtf8("actionLevel_NPC_Dump"));
    centralwidget = new QWidget(FRCWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    tabWidget = new QTabWidget(centralwidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setWindowModality(Qt::NonModal);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
    tabWidget->setSizePolicy(sizePolicy);
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    gridLayout1 = new QGridLayout(tab);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(0);
    gridLayout1->setVerticalSpacing(0);
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    textEdit = new QTextEdit(tab);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setFrameShape(QFrame::NoFrame);
    textEdit->setFrameShadow(QFrame::Plain);
    textEdit->setLineWidth(0);

    gridLayout1->addWidget(textEdit, 0, 0, 1, 1);

    tabWidget->addTab(tab, QString());

    gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

    lineEdit = new QLineEdit(centralwidget);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    gridLayout->addWidget(lineEdit, 1, 0, 1, 1);

    FRCWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(FRCWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 516, 29));
    menuPlayers = new QMenu(menubar);
    menuPlayers->setObjectName(QString::fromUtf8("menuPlayers"));
    menuFiles = new QMenu(menubar);
    menuFiles->setObjectName(QString::fromUtf8("menuFiles"));
    menuConfiguration = new QMenu(menubar);
    menuConfiguration->setObjectName(QString::fromUtf8("menuConfiguration"));
    menuScripts = new QMenu(menubar);
    menuScripts->setObjectName(QString::fromUtf8("menuScripts"));
    menuMisc = new QMenu(menubar);
    menuMisc->setObjectName(QString::fromUtf8("menuMisc"));
    FRCWindow->setMenuBar(menubar);

    menubar->addAction(menuPlayers->menuAction());
    menubar->addAction(menuFiles->menuAction());
    menubar->addAction(menuConfiguration->menuAction());
    menubar->addAction(menuScripts->menuAction());
    menubar->addAction(menuMisc->menuAction());
    menuPlayers->addAction(actionPlayerlist);
    menuPlayers->addAction(actionAccounts);
    menuPlayers->addAction(actionToalls);
    menuFiles->addAction(actionFile_Browser);
    menuConfiguration->addAction(actionRC_Options);
    menuConfiguration->addAction(actionServer_Options);
    menuConfiguration->addAction(actionFolder_Configuration);
    menuScripts->addAction(actionClasses);
    menuScripts->addAction(actionNPCs);
    menuScripts->addAction(actionWeapons_GUI);
    menuMisc->addAction(actionServer_Flags);
    menuMisc->addAction(actionLevel_List);
    menuMisc->addAction(actionLevel_NPC_Dump);

    retranslateUi(FRCWindow);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(FRCWindow);
    } // setupUi

    void retranslateUi(QMainWindow *FRCWindow)
    {
    FRCWindow->setWindowTitle(QApplication::translate("FRCWindow", "RemoteControl - April 11, 2008", 0, QApplication::UnicodeUTF8));
    actionPlayerlist->setText(QApplication::translate("FRCWindow", "Playerlist", 0, QApplication::UnicodeUTF8));
    actionAccounts->setText(QApplication::translate("FRCWindow", "Accounts", 0, QApplication::UnicodeUTF8));
    actionToalls->setText(QApplication::translate("FRCWindow", "Toalls", 0, QApplication::UnicodeUTF8));
    actionFile_Browser->setText(QApplication::translate("FRCWindow", "File Browser", 0, QApplication::UnicodeUTF8));
    actionRC_Options->setText(QApplication::translate("FRCWindow", "RC Options", 0, QApplication::UnicodeUTF8));
    actionServer_Options->setText(QApplication::translate("FRCWindow", "Server Options", 0, QApplication::UnicodeUTF8));
    actionFolder_Configuration->setText(QApplication::translate("FRCWindow", "Folder Config", 0, QApplication::UnicodeUTF8));
    actionNPCs->setText(QApplication::translate("FRCWindow", "NPCs", 0, QApplication::UnicodeUTF8));
    actionClasses->setText(QApplication::translate("FRCWindow", "Classes", 0, QApplication::UnicodeUTF8));
    actionWeapons_GUI->setText(QApplication::translate("FRCWindow", "Weapons (GUI)", 0, QApplication::UnicodeUTF8));
    actionServer_Flags->setText(QApplication::translate("FRCWindow", "Server Flags", 0, QApplication::UnicodeUTF8));
    actionLevel_List->setText(QApplication::translate("FRCWindow", "Level List", 0, QApplication::UnicodeUTF8));
    actionLevel_NPC_Dump->setText(QApplication::translate("FRCWindow", "Level-NPC dump", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("FRCWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
    lineEdit->setStyleSheet(QApplication::translate("FRCWindow", "background: green", 0, QApplication::UnicodeUTF8));
    menuPlayers->setTitle(QApplication::translate("FRCWindow", "Players", 0, QApplication::UnicodeUTF8));
    menuFiles->setTitle(QApplication::translate("FRCWindow", "Files", 0, QApplication::UnicodeUTF8));
    menuConfiguration->setTitle(QApplication::translate("FRCWindow", "Configuration", 0, QApplication::UnicodeUTF8));
    menuScripts->setTitle(QApplication::translate("FRCWindow", "Scripts", 0, QApplication::UnicodeUTF8));
    menuMisc->setTitle(QApplication::translate("FRCWindow", "Misc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FRCWindow: public Ui_FRCWindow {};
} // namespace Ui

#endif // UI_RCMAIN_H
