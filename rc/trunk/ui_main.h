/********************************************************************************
** Form generated from reading ui file 'main.ui'
**
** Created: Tue Sep 4 18:50:58 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

class Ui_CRCLive
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QPushButton *pushButton_12;
    QLabel *lNPCServer;
    QPushButton *pushButton_10;
    QPushButton *pushButton_4;
    QSpacerItem *spacerItem;
    QLabel *lServer;
    QPushButton *pushButton_8;
    QPushButton *pushButton_2;
    QPushButton *pushButton_7;
    QLabel *lPlayers;
    QPushButton *pushButton_6;
    QSpacerItem *spacerItem1;
    QPushButton *bFileBrowser;
    QPushButton *pushButton;
    QPushButton *pushButton_11;
    QPushButton *pushButton_9;
    QPushButton *pushButton_15;
    QGridLayout *gridLayout2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLineEdit *lineEdit;

    void setupUi(QMainWindow *CRCLive)
    {
    if (CRCLive->objectName().isEmpty())
        CRCLive->setObjectName(QString::fromUtf8("CRCLive"));
    CRCLive->resize(608, 375);
    CRCLive->setWindowIcon(QIcon(QString::fromUtf8(":/RC/remotecontrol.png")));
    CRCLive->setLayoutDirection(Qt::LeftToRight);
    CRCLive->setAutoFillBackground(true);
    centralwidget = new QWidget(CRCLive);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout1 = new QGridLayout();
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(1);
    gridLayout1->setVerticalSpacing(1);
    gridLayout1->setContentsMargins(1, 1, 1, 1);
    pushButton_12 = new QPushButton(centralwidget);
    pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(pushButton_12->sizePolicy().hasHeightForWidth());
    pushButton_12->setSizePolicy(sizePolicy);
    pushButton_12->setMinimumSize(QSize(32, 32));
    pushButton_12->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_12, 4, 5, 1, 1);

    lNPCServer = new QLabel(centralwidget);
    lNPCServer->setObjectName(QString::fromUtf8("lNPCServer"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(lNPCServer->sizePolicy().hasHeightForWidth());
    lNPCServer->setSizePolicy(sizePolicy1);
    lNPCServer->setMinimumSize(QSize(0, 24));
    lNPCServer->setMaximumSize(QSize(16777215, 24));

    gridLayout1->addWidget(lNPCServer, 4, 0, 1, 4);

    pushButton_10 = new QPushButton(centralwidget);
    pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
    sizePolicy.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
    pushButton_10->setSizePolicy(sizePolicy);
    pushButton_10->setMinimumSize(QSize(32, 32));
    pushButton_10->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_10, 4, 7, 1, 1);

    pushButton_4 = new QPushButton(centralwidget);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
    sizePolicy.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
    pushButton_4->setSizePolicy(sizePolicy);
    pushButton_4->setMinimumSize(QSize(32, 32));
    pushButton_4->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_4, 0, 2, 1, 1);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem, 2, 7, 2, 1);

    lServer = new QLabel(centralwidget);
    lServer->setObjectName(QString::fromUtf8("lServer"));
    sizePolicy1.setHeightForWidth(lServer->sizePolicy().hasHeightForWidth());
    lServer->setSizePolicy(sizePolicy1);
    lServer->setMinimumSize(QSize(0, 24));
    lServer->setMaximumSize(QSize(16777215, 24));

    gridLayout1->addWidget(lServer, 2, 0, 1, 4);

    pushButton_8 = new QPushButton(centralwidget);
    pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
    sizePolicy.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
    pushButton_8->setSizePolicy(sizePolicy);
    pushButton_8->setMinimumSize(QSize(32, 32));
    pushButton_8->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_8, 0, 5, 1, 1);

    pushButton_2 = new QPushButton(centralwidget);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
    pushButton_2->setSizePolicy(sizePolicy);
    pushButton_2->setMinimumSize(QSize(32, 32));
    pushButton_2->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_2, 0, 1, 1, 1);

    pushButton_7 = new QPushButton(centralwidget);
    pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
    sizePolicy.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
    pushButton_7->setSizePolicy(sizePolicy);
    pushButton_7->setMinimumSize(QSize(32, 32));
    pushButton_7->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_7, 1, 7, 1, 1);

    lPlayers = new QLabel(centralwidget);
    lPlayers->setObjectName(QString::fromUtf8("lPlayers"));
    sizePolicy1.setHeightForWidth(lPlayers->sizePolicy().hasHeightForWidth());
    lPlayers->setSizePolicy(sizePolicy1);
    lPlayers->setMinimumSize(QSize(0, 24));
    lPlayers->setMaximumSize(QSize(16777215, 24));

    gridLayout1->addWidget(lPlayers, 3, 0, 1, 4);

    pushButton_6 = new QPushButton(centralwidget);
    pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
    sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
    pushButton_6->setSizePolicy(sizePolicy);
    pushButton_6->setMinimumSize(QSize(32, 32));
    pushButton_6->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_6, 0, 6, 1, 1);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem1, 0, 3, 1, 1);

    bFileBrowser = new QPushButton(centralwidget);
    bFileBrowser->setObjectName(QString::fromUtf8("bFileBrowser"));
    sizePolicy.setHeightForWidth(bFileBrowser->sizePolicy().hasHeightForWidth());
    bFileBrowser->setSizePolicy(sizePolicy);
    bFileBrowser->setMinimumSize(QSize(32, 32));
    bFileBrowser->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(bFileBrowser, 1, 0, 1, 1);

    pushButton = new QPushButton(centralwidget);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
    pushButton->setSizePolicy(sizePolicy);
    pushButton->setMinimumSize(QSize(32, 32));
    pushButton->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton, 0, 0, 1, 1);

    pushButton_11 = new QPushButton(centralwidget);
    pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
    sizePolicy.setHeightForWidth(pushButton_11->sizePolicy().hasHeightForWidth());
    pushButton_11->setSizePolicy(sizePolicy);
    pushButton_11->setMinimumSize(QSize(32, 32));
    pushButton_11->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_11, 4, 6, 1, 1);

    pushButton_9 = new QPushButton(centralwidget);
    pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
    sizePolicy.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
    pushButton_9->setSizePolicy(sizePolicy);
    pushButton_9->setMinimumSize(QSize(32, 32));
    pushButton_9->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_9, 0, 7, 1, 1);

    pushButton_15 = new QPushButton(centralwidget);
    pushButton_15->setObjectName(QString::fromUtf8("pushButton_15"));
    sizePolicy.setHeightForWidth(pushButton_15->sizePolicy().hasHeightForWidth());
    pushButton_15->setSizePolicy(sizePolicy);
    pushButton_15->setMinimumSize(QSize(32, 32));
    pushButton_15->setMaximumSize(QSize(32, 32));

    gridLayout1->addWidget(pushButton_15, 4, 4, 1, 1);


    gridLayout->addLayout(gridLayout1, 0, 0, 1, 1);

    gridLayout2 = new QGridLayout();
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(0);
    gridLayout2->setVerticalSpacing(0);
    gridLayout2->setContentsMargins(0, 0, 0, 0);
    tabWidget = new QTabWidget(centralwidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tabWidget->addTab(tab, QString());

    gridLayout2->addWidget(tabWidget, 0, 0, 1, 1);

    lineEdit = new QLineEdit(centralwidget);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    gridLayout2->addWidget(lineEdit, 1, 0, 1, 1);


    gridLayout->addLayout(gridLayout2, 1, 0, 1, 1);

    CRCLive->setCentralWidget(centralwidget);
    QWidget::setTabOrder(pushButton, pushButton_2);
    QWidget::setTabOrder(pushButton_2, pushButton_4);
    QWidget::setTabOrder(pushButton_4, pushButton_8);
    QWidget::setTabOrder(pushButton_8, pushButton_6);
    QWidget::setTabOrder(pushButton_6, pushButton_9);
    QWidget::setTabOrder(pushButton_9, bFileBrowser);
    QWidget::setTabOrder(bFileBrowser, pushButton_7);
    QWidget::setTabOrder(pushButton_7, pushButton_15);
    QWidget::setTabOrder(pushButton_15, pushButton_12);
    QWidget::setTabOrder(pushButton_12, pushButton_11);
    QWidget::setTabOrder(pushButton_11, pushButton_10);
    QWidget::setTabOrder(pushButton_10, tabWidget);
    QWidget::setTabOrder(tabWidget, lineEdit);

    retranslateUi(CRCLive);

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(CRCLive);
    } // setupUi

    void retranslateUi(QMainWindow *CRCLive)
    {
    CRCLive->setWindowTitle(QApplication::translate("CRCLive", "RemoteControl", 0, QApplication::UnicodeUTF8));
    CRCLive->setStyleSheet(QString());
    pushButton_12->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_12->setWhatsThis(QString());
    pushButton_12->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_weapons_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_weapons_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_12->setText(QString());
    lNPCServer->setText(QString());
    pushButton_10->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_10->setWhatsThis(QString());
    pushButton_10->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_levellist_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_levellist_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_10->setText(QString());
    pushButton_4->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_4->setWhatsThis(QString());
    pushButton_4->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_toalls_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_toalls_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_4->setText(QString());
    lServer->setText(QApplication::translate("CRCLive", "Server: ", 0, QApplication::UnicodeUTF8));
    pushButton_8->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_8->setWhatsThis(QString());
    pushButton_8->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_options_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_options_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_8->setText(QString());
    pushButton_2->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_2->setWhatsThis(QString());
    pushButton_2->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_accountlist_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_accountlist_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QString());
    pushButton_7->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_7->setWhatsThis(QString());
    pushButton_7->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_serveroptions_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_serveroptions_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_7->setText(QString());
    lPlayers->setText(QApplication::translate("CRCLive", "Players: ", 0, QApplication::UnicodeUTF8));
    pushButton_6->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_6->setWhatsThis(QString());
    pushButton_6->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_serverflags_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_serverflags_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_6->setText(QString());
    bFileBrowser->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    bFileBrowser->setWhatsThis(QString());
    bFileBrowser->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_filebrowser_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_filebrowser_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    bFileBrowser->setText(QString());
    pushButton->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton->setWhatsThis(QString());
    pushButton->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_playerlist_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_playerlist_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QString());
    pushButton_11->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_11->setWhatsThis(QString());
    pushButton_11->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_classes_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_classes_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_11->setText(QString());
    pushButton_9->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_9->setWhatsThis(QString());
    pushButton_9->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_folderconfig_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_folderconfig_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_9->setText(QString());
    pushButton_15->setToolTip(QApplication::translate("CRCLive", "Accounts List", 0, QApplication::UnicodeUTF8));
    pushButton_15->setWhatsThis(QString());
    pushButton_15->setStyleSheet(QApplication::translate("CRCLive", "QPushButton\n"
"{\n"
"  background: url(:/RC/rc_npcs_normal.png);\n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"  background: url(:/RC/rc_npcs_pressed.png);\n"
"}", 0, QApplication::UnicodeUTF8));
    pushButton_15->setText(QString());
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CRCLive", "Tab 1", 0, QApplication::UnicodeUTF8));
    lineEdit->setStyleSheet(QApplication::translate("CRCLive", "background: green", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CRCLive);
    } // retranslateUi

};

namespace Ui {
    class CRCLive: public Ui_CRCLive {};
} // namespace Ui

#endif // UI_MAIN_H
