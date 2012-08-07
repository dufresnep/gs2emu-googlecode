/********************************************************************************
** Form generated from reading ui file 'viewaccount.ui'
**
** Created: Thu May 1 02:47:57 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VIEWACCOUNT_H
#define UI_VIEWACCOUNT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_FViewAccount
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout1;
    QLabel *label_17;
    QLineEdit *ppX;
    QLineEdit *ppY;
    QLabel *label_16;
    QLineEdit *ppLevel;
    QLabel *label_15;
    QGroupBox *groupBox_4;
    QLabel *label_3;
    QLabel *label_9;
    QLabel *label_4;
    QLabel *label_10;
    QLabel *label_6;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_7;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *ppMP;
    QLineEdit *ppGralats;
    QLineEdit *ppGlove;
    QLineEdit *ppArrows;
    QLabel *label_5;
    QLabel *label_8;
    QLineEdit *ppBombs;
    QLineEdit *ppSwordImg;
    QLineEdit *ppShieldPower;
    QLineEdit *ppShieldImg;
    QLineEdit *ppPower;
    QLineEdit *ppMaxPower;
    QLineEdit *ppAP;
    QLineEdit *ppSwordPower;
    QCheckBox *ppGender;
    QCheckBox *ppWeapons;
    QCheckBox *ppSpin;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout2;
    QLabel *label_26;
    QComboBox *ppBelt;
    QComboBox *ppShoe;
    QComboBox *ppSleeve;
    QComboBox *ppCoat;
    QLineEdit *ppBody;
    QLineEdit *ppHead;
    QComboBox *ppSkin;
    QLineEdit *ppHat;
    QLabel *label_25;
    QLabel *label_24;
    QLabel *label_23;
    QLabel *label_22;
    QLineEdit *ppAni;
    QLabel *label_21;
    QLabel *label_20;
    QLabel *label_19;
    QLabel *label_18;
    QWidget *tab_2;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout3;
    QTextEdit *paFlags;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout4;
    QTextEdit *paChests;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout5;
    QTextEdit *paWeapons;
    QWidget *tab_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout6;
    QTextEdit *prFolders;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout7;
    QLineEdit *prIPRange;
    QLabel *label;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QLabel *label_2;
    QLineEdit *prAccount;
    QGroupBox *groupBox;
    QGridLayout *gridLayout8;
    QCheckBox *pr17;
    QCheckBox *pr19;
    QCheckBox *pr1;
    QCheckBox *pr9;
    QCheckBox *pr12;
    QCheckBox *pr16;
    QCheckBox *pr5;
    QCheckBox *pr13;
    QCheckBox *pr14;
    QCheckBox *pr6;
    QCheckBox *pr11;
    QCheckBox *pr18;
    QCheckBox *pr7;
    QCheckBox *pr3;
    QCheckBox *pr15;
    QCheckBox *pr4;
    QCheckBox *pr2;
    QCheckBox *pr8;
    QCheckBox *pr10;
    QCheckBox *pr20;
    QWidget *tab_4;
    QGroupBox *groupBox_10;
    QGridLayout *gridLayout9;
    QSpacerItem *spacerItem2;
    QLabel *label_27;
    QCheckBox *pcBanned;
    QTextEdit *pcBanReason;
    QComboBox *pcBans;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout10;
    QTextEdit *pcComments;
    QSpacerItem *spacerItem3;
    QPushButton *pApply;
    QPushButton *pCancel;

    void setupUi(QWidget *FViewAccount)
    {
    if (FViewAccount->objectName().isEmpty())
        FViewAccount->setObjectName(QString::fromUtf8("FViewAccount"));
    FViewAccount->resize(430, 530);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(FViewAccount->sizePolicy().hasHeightForWidth());
    FViewAccount->setSizePolicy(sizePolicy);
    FViewAccount->setWindowIcon(QIcon(QString::fromUtf8(":/RC/remotecontrol.png")));
    gridLayout = new QGridLayout(FViewAccount);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(3, 3, 3, 3);
    tabWidget = new QTabWidget(FViewAccount);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tab_1 = new QWidget();
    tab_1->setObjectName(QString::fromUtf8("tab_1"));
    groupBox_5 = new QGroupBox(tab_1);
    groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
    groupBox_5->setGeometry(QRect(9, 9, 401, 80));
    gridLayout1 = new QGridLayout(groupBox_5);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(6);
    gridLayout1->setVerticalSpacing(6);
    gridLayout1->setContentsMargins(9, 9, 9, 9);
    label_17 = new QLabel(groupBox_5);
    label_17->setObjectName(QString::fromUtf8("label_17"));
    label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout1->addWidget(label_17, 1, 2, 1, 1);

    ppX = new QLineEdit(groupBox_5);
    ppX->setObjectName(QString::fromUtf8("ppX"));

    gridLayout1->addWidget(ppX, 1, 1, 1, 1);

    ppY = new QLineEdit(groupBox_5);
    ppY->setObjectName(QString::fromUtf8("ppY"));

    gridLayout1->addWidget(ppY, 1, 3, 1, 1);

    label_16 = new QLabel(groupBox_5);
    label_16->setObjectName(QString::fromUtf8("label_16"));
    label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    gridLayout1->addWidget(label_16, 1, 0, 1, 1);

    ppLevel = new QLineEdit(groupBox_5);
    ppLevel->setObjectName(QString::fromUtf8("ppLevel"));

    gridLayout1->addWidget(ppLevel, 0, 1, 1, 3);

    label_15 = new QLabel(groupBox_5);
    label_15->setObjectName(QString::fromUtf8("label_15"));

    gridLayout1->addWidget(label_15, 0, 0, 1, 1);

    groupBox_4 = new QGroupBox(tab_1);
    groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
    groupBox_4->setGeometry(QRect(9, 88, 401, 211));
    label_3 = new QLabel(groupBox_4);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setGeometry(QRect(10, 24, 59, 20));
    label_9 = new QLabel(groupBox_4);
    label_9->setObjectName(QString::fromUtf8("label_9"));
    label_9->setGeometry(QRect(204, 24, 58, 20));
    label_4 = new QLabel(groupBox_4);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setGeometry(QRect(10, 50, 59, 20));
    label_10 = new QLabel(groupBox_4);
    label_10->setObjectName(QString::fromUtf8("label_10"));
    label_10->setGeometry(QRect(204, 50, 58, 20));
    label_6 = new QLabel(groupBox_4);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(10, 76, 59, 20));
    label_11 = new QLabel(groupBox_4);
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setGeometry(QRect(204, 76, 58, 20));
    label_12 = new QLabel(groupBox_4);
    label_12->setObjectName(QString::fromUtf8("label_12"));
    label_12->setGeometry(QRect(204, 102, 58, 20));
    label_7 = new QLabel(groupBox_4);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(10, 154, 59, 20));
    label_13 = new QLabel(groupBox_4);
    label_13->setObjectName(QString::fromUtf8("label_13"));
    label_13->setGeometry(QRect(204, 128, 58, 20));
    label_14 = new QLabel(groupBox_4);
    label_14->setObjectName(QString::fromUtf8("label_14"));
    label_14->setGeometry(QRect(204, 154, 58, 20));
    ppMP = new QLineEdit(groupBox_4);
    ppMP->setObjectName(QString::fromUtf8("ppMP"));
    ppMP->setGeometry(QRect(268, 50, 123, 20));
    ppGralats = new QLineEdit(groupBox_4);
    ppGralats->setObjectName(QString::fromUtf8("ppGralats"));
    ppGralats->setGeometry(QRect(75, 76, 123, 20));
    ppGlove = new QLineEdit(groupBox_4);
    ppGlove->setObjectName(QString::fromUtf8("ppGlove"));
    ppGlove->setGeometry(QRect(268, 76, 123, 20));
    ppArrows = new QLineEdit(groupBox_4);
    ppArrows->setObjectName(QString::fromUtf8("ppArrows"));
    ppArrows->setGeometry(QRect(268, 102, 123, 20));
    label_5 = new QLabel(groupBox_4);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setGeometry(QRect(10, 128, 63, 20));
    label_8 = new QLabel(groupBox_4);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    label_8->setGeometry(QRect(10, 102, 59, 20));
    ppBombs = new QLineEdit(groupBox_4);
    ppBombs->setObjectName(QString::fromUtf8("ppBombs"));
    ppBombs->setGeometry(QRect(75, 102, 123, 20));
    ppSwordImg = new QLineEdit(groupBox_4);
    ppSwordImg->setObjectName(QString::fromUtf8("ppSwordImg"));
    ppSwordImg->setGeometry(QRect(268, 128, 123, 20));
    ppShieldPower = new QLineEdit(groupBox_4);
    ppShieldPower->setObjectName(QString::fromUtf8("ppShieldPower"));
    ppShieldPower->setGeometry(QRect(75, 154, 123, 20));
    ppShieldImg = new QLineEdit(groupBox_4);
    ppShieldImg->setObjectName(QString::fromUtf8("ppShieldImg"));
    ppShieldImg->setGeometry(QRect(268, 154, 123, 20));
    ppPower = new QLineEdit(groupBox_4);
    ppPower->setObjectName(QString::fromUtf8("ppPower"));
    ppPower->setGeometry(QRect(75, 24, 123, 20));
    ppMaxPower = new QLineEdit(groupBox_4);
    ppMaxPower->setObjectName(QString::fromUtf8("ppMaxPower"));
    ppMaxPower->setGeometry(QRect(268, 24, 123, 20));
    ppAP = new QLineEdit(groupBox_4);
    ppAP->setObjectName(QString::fromUtf8("ppAP"));
    ppAP->setGeometry(QRect(75, 50, 123, 20));
    ppSwordPower = new QLineEdit(groupBox_4);
    ppSwordPower->setObjectName(QString::fromUtf8("ppSwordPower"));
    ppSwordPower->setGeometry(QRect(75, 128, 123, 20));
    ppGender = new QCheckBox(groupBox_4);
    ppGender->setObjectName(QString::fromUtf8("ppGender"));
    ppGender->setGeometry(QRect(11, 181, 122, 18));
    ppWeapons = new QCheckBox(groupBox_4);
    ppWeapons->setObjectName(QString::fromUtf8("ppWeapons"));
    ppWeapons->setGeometry(QRect(139, 181, 123, 18));
    ppSpin = new QCheckBox(groupBox_4);
    ppSpin->setObjectName(QString::fromUtf8("ppSpin"));
    ppSpin->setGeometry(QRect(268, 181, 122, 18));
    groupBox_6 = new QGroupBox(tab_1);
    groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
    groupBox_6->setGeometry(QRect(9, 297, 403, 154));
    gridLayout2 = new QGridLayout(groupBox_6);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(6);
    gridLayout2->setVerticalSpacing(6);
    gridLayout2->setContentsMargins(9, 9, 9, 9);
    label_26 = new QLabel(groupBox_6);
    label_26->setObjectName(QString::fromUtf8("label_26"));

    gridLayout2->addWidget(label_26, 4, 0, 1, 1);

    ppBelt = new QComboBox(groupBox_6);
    ppBelt->setObjectName(QString::fromUtf8("ppBelt"));

    gridLayout2->addWidget(ppBelt, 4, 1, 1, 1);

    ppShoe = new QComboBox(groupBox_6);
    ppShoe->setObjectName(QString::fromUtf8("ppShoe"));

    gridLayout2->addWidget(ppShoe, 3, 3, 1, 1);

    ppSleeve = new QComboBox(groupBox_6);
    ppSleeve->setObjectName(QString::fromUtf8("ppSleeve"));

    gridLayout2->addWidget(ppSleeve, 3, 1, 1, 1);

    ppCoat = new QComboBox(groupBox_6);
    ppCoat->setObjectName(QString::fromUtf8("ppCoat"));

    gridLayout2->addWidget(ppCoat, 2, 3, 1, 1);

    ppBody = new QLineEdit(groupBox_6);
    ppBody->setObjectName(QString::fromUtf8("ppBody"));

    gridLayout2->addWidget(ppBody, 0, 3, 1, 1);

    ppHead = new QLineEdit(groupBox_6);
    ppHead->setObjectName(QString::fromUtf8("ppHead"));

    gridLayout2->addWidget(ppHead, 0, 1, 1, 1);

    ppSkin = new QComboBox(groupBox_6);
    ppSkin->setObjectName(QString::fromUtf8("ppSkin"));

    gridLayout2->addWidget(ppSkin, 2, 1, 1, 1);

    ppHat = new QLineEdit(groupBox_6);
    ppHat->setObjectName(QString::fromUtf8("ppHat"));

    gridLayout2->addWidget(ppHat, 1, 1, 1, 1);

    label_25 = new QLabel(groupBox_6);
    label_25->setObjectName(QString::fromUtf8("label_25"));

    gridLayout2->addWidget(label_25, 3, 2, 1, 1);

    label_24 = new QLabel(groupBox_6);
    label_24->setObjectName(QString::fromUtf8("label_24"));

    gridLayout2->addWidget(label_24, 2, 2, 1, 1);

    label_23 = new QLabel(groupBox_6);
    label_23->setObjectName(QString::fromUtf8("label_23"));

    gridLayout2->addWidget(label_23, 3, 0, 1, 1);

    label_22 = new QLabel(groupBox_6);
    label_22->setObjectName(QString::fromUtf8("label_22"));

    gridLayout2->addWidget(label_22, 2, 0, 1, 1);

    ppAni = new QLineEdit(groupBox_6);
    ppAni->setObjectName(QString::fromUtf8("ppAni"));

    gridLayout2->addWidget(ppAni, 1, 3, 1, 1);

    label_21 = new QLabel(groupBox_6);
    label_21->setObjectName(QString::fromUtf8("label_21"));

    gridLayout2->addWidget(label_21, 1, 2, 1, 1);

    label_20 = new QLabel(groupBox_6);
    label_20->setObjectName(QString::fromUtf8("label_20"));
    label_20->setEnabled(true);

    gridLayout2->addWidget(label_20, 1, 0, 1, 1);

    label_19 = new QLabel(groupBox_6);
    label_19->setObjectName(QString::fromUtf8("label_19"));

    gridLayout2->addWidget(label_19, 0, 2, 1, 1);

    label_18 = new QLabel(groupBox_6);
    label_18->setObjectName(QString::fromUtf8("label_18"));

    gridLayout2->addWidget(label_18, 0, 0, 1, 1);

    tabWidget->addTab(tab_1, QString());
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    groupBox_8 = new QGroupBox(tab_2);
    groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
    groupBox_8->setGeometry(QRect(11, 158, 400, 140));
    gridLayout3 = new QGridLayout(groupBox_8);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    gridLayout3->setHorizontalSpacing(6);
    gridLayout3->setVerticalSpacing(6);
    gridLayout3->setContentsMargins(9, 9, 9, 9);
    paFlags = new QTextEdit(groupBox_8);
    paFlags->setObjectName(QString::fromUtf8("paFlags"));
    paFlags->setLineWrapMode(QTextEdit::NoWrap);

    gridLayout3->addWidget(paFlags, 0, 0, 1, 1);

    groupBox_7 = new QGroupBox(tab_2);
    groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
    groupBox_7->setGeometry(QRect(11, 10, 400, 140));
    gridLayout4 = new QGridLayout(groupBox_7);
    gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
    gridLayout4->setHorizontalSpacing(6);
    gridLayout4->setVerticalSpacing(6);
    gridLayout4->setContentsMargins(9, 9, 9, 9);
    paChests = new QTextEdit(groupBox_7);
    paChests->setObjectName(QString::fromUtf8("paChests"));
    paChests->setLineWrapMode(QTextEdit::NoWrap);

    gridLayout4->addWidget(paChests, 0, 0, 1, 1);

    groupBox_9 = new QGroupBox(tab_2);
    groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
    groupBox_9->setGeometry(QRect(11, 304, 400, 151));
    gridLayout5 = new QGridLayout(groupBox_9);
    gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
    gridLayout5->setHorizontalSpacing(6);
    gridLayout5->setVerticalSpacing(6);
    gridLayout5->setContentsMargins(9, 9, 9, 9);
    paWeapons = new QTextEdit(groupBox_9);
    paWeapons->setObjectName(QString::fromUtf8("paWeapons"));
    paWeapons->setLineWrapMode(QTextEdit::NoWrap);

    gridLayout5->addWidget(paWeapons, 0, 0, 1, 1);

    tabWidget->addTab(tab_2, QString());
    tab_3 = new QWidget();
    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    groupBox_2 = new QGroupBox(tab_3);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(9, 321, 405, 131));
    gridLayout6 = new QGridLayout(groupBox_2);
    gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
    gridLayout6->setHorizontalSpacing(6);
    gridLayout6->setVerticalSpacing(6);
    gridLayout6->setContentsMargins(9, 9, 9, 9);
    prFolders = new QTextEdit(groupBox_2);
    prFolders->setObjectName(QString::fromUtf8("prFolders"));
    prFolders->setLineWrapMode(QTextEdit::NoWrap);

    gridLayout6->addWidget(prFolders, 0, 0, 1, 1);

    groupBox_3 = new QGroupBox(tab_3);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    groupBox_3->setGeometry(QRect(9, 9, 405, 80));
    gridLayout7 = new QGridLayout(groupBox_3);
    gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
    gridLayout7->setHorizontalSpacing(6);
    gridLayout7->setVerticalSpacing(6);
    gridLayout7->setContentsMargins(9, 9, 9, 9);
    prIPRange = new QLineEdit(groupBox_3);
    prIPRange->setObjectName(QString::fromUtf8("prIPRange"));

    gridLayout7->addWidget(prIPRange, 1, 2, 1, 1);

    label = new QLabel(groupBox_3);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout7->addWidget(label, 0, 0, 1, 1);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout7->addItem(spacerItem, 0, 1, 1, 1);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout7->addItem(spacerItem1, 1, 1, 1, 1);

    label_2 = new QLabel(groupBox_3);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout7->addWidget(label_2, 1, 0, 1, 1);

    prAccount = new QLineEdit(groupBox_3);
    prAccount->setObjectName(QString::fromUtf8("prAccount"));

    gridLayout7->addWidget(prAccount, 0, 2, 1, 1);

    groupBox = new QGroupBox(tab_3);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(9, 95, 405, 220));
    gridLayout8 = new QGridLayout(groupBox);
    gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));
    gridLayout8->setHorizontalSpacing(6);
    gridLayout8->setVerticalSpacing(6);
    gridLayout8->setContentsMargins(9, 9, 9, 9);
    pr17 = new QCheckBox(groupBox);
    pr17->setObjectName(QString::fromUtf8("pr17"));

    gridLayout8->addWidget(pr17, 6, 1, 1, 1);

    pr19 = new QCheckBox(groupBox);
    pr19->setObjectName(QString::fromUtf8("pr19"));

    gridLayout8->addWidget(pr19, 8, 1, 1, 1);

    pr1 = new QCheckBox(groupBox);
    pr1->setObjectName(QString::fromUtf8("pr1"));

    gridLayout8->addWidget(pr1, 0, 0, 1, 1);

    pr9 = new QCheckBox(groupBox);
    pr9->setObjectName(QString::fromUtf8("pr9"));

    gridLayout8->addWidget(pr9, 8, 0, 1, 1);

    pr12 = new QCheckBox(groupBox);
    pr12->setObjectName(QString::fromUtf8("pr12"));

    gridLayout8->addWidget(pr12, 1, 1, 1, 1);

    pr16 = new QCheckBox(groupBox);
    pr16->setObjectName(QString::fromUtf8("pr16"));

    gridLayout8->addWidget(pr16, 5, 1, 1, 1);

    pr5 = new QCheckBox(groupBox);
    pr5->setObjectName(QString::fromUtf8("pr5"));

    gridLayout8->addWidget(pr5, 4, 0, 1, 1);

    pr13 = new QCheckBox(groupBox);
    pr13->setObjectName(QString::fromUtf8("pr13"));

    gridLayout8->addWidget(pr13, 2, 1, 1, 1);

    pr14 = new QCheckBox(groupBox);
    pr14->setObjectName(QString::fromUtf8("pr14"));

    gridLayout8->addWidget(pr14, 3, 1, 1, 1);

    pr6 = new QCheckBox(groupBox);
    pr6->setObjectName(QString::fromUtf8("pr6"));

    gridLayout8->addWidget(pr6, 5, 0, 1, 1);

    pr11 = new QCheckBox(groupBox);
    pr11->setObjectName(QString::fromUtf8("pr11"));

    gridLayout8->addWidget(pr11, 0, 1, 1, 1);

    pr18 = new QCheckBox(groupBox);
    pr18->setObjectName(QString::fromUtf8("pr18"));

    gridLayout8->addWidget(pr18, 7, 1, 1, 1);

    pr7 = new QCheckBox(groupBox);
    pr7->setObjectName(QString::fromUtf8("pr7"));

    gridLayout8->addWidget(pr7, 6, 0, 1, 1);

    pr3 = new QCheckBox(groupBox);
    pr3->setObjectName(QString::fromUtf8("pr3"));

    gridLayout8->addWidget(pr3, 2, 0, 1, 1);

    pr15 = new QCheckBox(groupBox);
    pr15->setObjectName(QString::fromUtf8("pr15"));

    gridLayout8->addWidget(pr15, 4, 1, 1, 1);

    pr4 = new QCheckBox(groupBox);
    pr4->setObjectName(QString::fromUtf8("pr4"));

    gridLayout8->addWidget(pr4, 3, 0, 1, 1);

    pr2 = new QCheckBox(groupBox);
    pr2->setObjectName(QString::fromUtf8("pr2"));

    gridLayout8->addWidget(pr2, 1, 0, 1, 1);

    pr8 = new QCheckBox(groupBox);
    pr8->setObjectName(QString::fromUtf8("pr8"));

    gridLayout8->addWidget(pr8, 7, 0, 1, 1);

    pr10 = new QCheckBox(groupBox);
    pr10->setObjectName(QString::fromUtf8("pr10"));

    gridLayout8->addWidget(pr10, 9, 0, 1, 1);

    pr20 = new QCheckBox(groupBox);
    pr20->setObjectName(QString::fromUtf8("pr20"));

    gridLayout8->addWidget(pr20, 9, 1, 1, 1);

    tabWidget->addTab(tab_3, QString());
    tab_4 = new QWidget();
    tab_4->setObjectName(QString::fromUtf8("tab_4"));
    groupBox_10 = new QGroupBox(tab_4);
    groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
    groupBox_10->setGeometry(QRect(10, 10, 401, 151));
    gridLayout9 = new QGridLayout(groupBox_10);
    gridLayout9->setObjectName(QString::fromUtf8("gridLayout9"));
    gridLayout9->setHorizontalSpacing(6);
    gridLayout9->setVerticalSpacing(6);
    gridLayout9->setContentsMargins(9, 9, 9, 9);
    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout9->addItem(spacerItem2, 0, 1, 1, 1);

    label_27 = new QLabel(groupBox_10);
    label_27->setObjectName(QString::fromUtf8("label_27"));

    gridLayout9->addWidget(label_27, 0, 0, 1, 1);

    pcBanned = new QCheckBox(groupBox_10);
    pcBanned->setObjectName(QString::fromUtf8("pcBanned"));

    gridLayout9->addWidget(pcBanned, 0, 2, 1, 1);

    pcBanReason = new QTextEdit(groupBox_10);
    pcBanReason->setObjectName(QString::fromUtf8("pcBanReason"));

    gridLayout9->addWidget(pcBanReason, 2, 0, 1, 3);

    pcBans = new QComboBox(groupBox_10);
    pcBans->setObjectName(QString::fromUtf8("pcBans"));

    gridLayout9->addWidget(pcBans, 1, 0, 1, 3);

    groupBox_11 = new QGroupBox(tab_4);
    groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
    groupBox_11->setGeometry(QRect(10, 170, 401, 281));
    gridLayout10 = new QGridLayout(groupBox_11);
    gridLayout10->setObjectName(QString::fromUtf8("gridLayout10"));
    gridLayout10->setHorizontalSpacing(6);
    gridLayout10->setVerticalSpacing(6);
    gridLayout10->setContentsMargins(9, 9, 9, 9);
    pcComments = new QTextEdit(groupBox_11);
    pcComments->setObjectName(QString::fromUtf8("pcComments"));

    gridLayout10->addWidget(pcComments, 0, 0, 1, 1);

    tabWidget->addTab(tab_4, QString());

    gridLayout->addWidget(tabWidget, 0, 0, 1, 3);

    spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem3, 1, 0, 1, 1);

    pApply = new QPushButton(FViewAccount);
    pApply->setObjectName(QString::fromUtf8("pApply"));

    gridLayout->addWidget(pApply, 1, 1, 1, 1);

    pCancel = new QPushButton(FViewAccount);
    pCancel->setObjectName(QString::fromUtf8("pCancel"));

    gridLayout->addWidget(pCancel, 1, 2, 1, 1);


    retranslateUi(FViewAccount);

    tabWidget->setCurrentIndex(3);


    QMetaObject::connectSlotsByName(FViewAccount);
    } // setupUi

    void retranslateUi(QWidget *FViewAccount)
    {
    FViewAccount->setWindowTitle(QApplication::translate("FViewAccount", "Viewing Account: Joey (127.0.0.1)", 0, QApplication::UnicodeUTF8));
    groupBox_5->setTitle(QApplication::translate("FViewAccount", "Account Location", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("FViewAccount", "Y:  ", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("FViewAccount", "X: ", 0, QApplication::UnicodeUTF8));
    label_15->setText(QApplication::translate("FViewAccount", "Level Name: ", 0, QApplication::UnicodeUTF8));
    groupBox_4->setTitle(QApplication::translate("FViewAccount", "Account Attributes", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("FViewAccount", "Hearts", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("FViewAccount", "Full Hearts", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("FViewAccount", "Alignment", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("FViewAccount", "Magic Points", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("FViewAccount", "Gralats", 0, QApplication::UnicodeUTF8));
    label_11->setText(QApplication::translate("FViewAccount", "Gloves", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("FViewAccount", "Arrows", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("FViewAccount", "Shield Power", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("FViewAccount", "Sword Img.", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("FViewAccount", "Shield Img.", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("FViewAccount", "Sword Power", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("FViewAccount", "Bombs", 0, QApplication::UnicodeUTF8));
    ppGender->setText(QApplication::translate("FViewAccount", "Male (checked)", 0, QApplication::UnicodeUTF8));
    ppWeapons->setText(QApplication::translate("FViewAccount", "Weapons Enabled", 0, QApplication::UnicodeUTF8));
    ppSpin->setText(QApplication::translate("FViewAccount", "Spin Attack", 0, QApplication::UnicodeUTF8));
    groupBox_6->setTitle(QApplication::translate("FViewAccount", "Account Look", 0, QApplication::UnicodeUTF8));
    label_26->setText(QApplication::translate("FViewAccount", "Belt Color", 0, QApplication::UnicodeUTF8));
    label_25->setText(QApplication::translate("FViewAccount", "Shoe Color", 0, QApplication::UnicodeUTF8));
    label_24->setText(QApplication::translate("FViewAccount", "Coat Color", 0, QApplication::UnicodeUTF8));
    label_23->setText(QApplication::translate("FViewAccount", "Sleeve Color", 0, QApplication::UnicodeUTF8));
    label_22->setText(QApplication::translate("FViewAccount", "Skin Color", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("FViewAccount", "Animation", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("FViewAccount", "Hat Img.", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("FViewAccount", "Body Img.", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("FViewAccount", "Head Img.", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("FViewAccount", "Player Properties", 0, QApplication::UnicodeUTF8));
    groupBox_8->setTitle(QApplication::translate("FViewAccount", "Account Flags", 0, QApplication::UnicodeUTF8));
    groupBox_7->setTitle(QApplication::translate("FViewAccount", "Account Chests", 0, QApplication::UnicodeUTF8));
    groupBox_9->setTitle(QApplication::translate("FViewAccount", "Account Weapons", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("FViewAccount", "Player Attributes", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("FViewAccount", "Acccount Folders", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("FViewAccount", "Account Information", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("FViewAccount", "Account Name", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("FViewAccount", "IP Range", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("FViewAccount", "Account Rights", 0, QApplication::UnicodeUTF8));
    pr17->setText(QApplication::translate("FViewAccount", "Change server options", 0, QApplication::UnicodeUTF8));
    pr19->setText(QApplication::translate("FViewAccount", "Edit folder rights", 0, QApplication::UnicodeUTF8));
    pr1->setText(QApplication::translate("FViewAccount", "Warpto x y", 0, QApplication::UnicodeUTF8));
    pr9->setText(QApplication::translate("FViewAccount", "Reset attributes", 0, QApplication::UnicodeUTF8));
    pr12->setText(QApplication::translate("FViewAccount", "Set server flags", 0, QApplication::UnicodeUTF8));
    pr16->setText(QApplication::translate("FViewAccount", "Empty 2", 0, QApplication::UnicodeUTF8));
    pr5->setText(QApplication::translate("FViewAccount", "Disconnect players", 0, QApplication::UnicodeUTF8));
    pr13->setText(QApplication::translate("FViewAccount", "Change rights", 0, QApplication::UnicodeUTF8));
    pr14->setText(QApplication::translate("FViewAccount", "Ban players", 0, QApplication::UnicodeUTF8));
    pr6->setText(QApplication::translate("FViewAccount", "View player attributes", 0, QApplication::UnicodeUTF8));
    pr11->setText(QApplication::translate("FViewAccount", "Empty 1", 0, QApplication::UnicodeUTF8));
    pr18->setText(QApplication::translate("FViewAccount", "Edit folder configuration", 0, QApplication::UnicodeUTF8));
    pr7->setText(QApplication::translate("FViewAccount", "Set player attributes", 0, QApplication::UnicodeUTF8));
    pr3->setText(QApplication::translate("FViewAccount", "Warp players", 0, QApplication::UnicodeUTF8));
    pr15->setText(QApplication::translate("FViewAccount", "Change comments", 0, QApplication::UnicodeUTF8));
    pr4->setText(QApplication::translate("FViewAccount", "Update level", 0, QApplication::UnicodeUTF8));
    pr2->setText(QApplication::translate("FViewAccount", "Warpto player", 0, QApplication::UnicodeUTF8));
    pr8->setText(QApplication::translate("FViewAccount", "Set the own attributes", 0, QApplication::UnicodeUTF8));
    pr10->setText(QApplication::translate("FViewAccount", "Admin messages", 0, QApplication::UnicodeUTF8));
    pr20->setText(QApplication::translate("FViewAccount", "NPC-Control", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("FViewAccount", "Player Rights", 0, QApplication::UnicodeUTF8));
    groupBox_10->setTitle(QApplication::translate("FViewAccount", "Player Ban", 0, QApplication::UnicodeUTF8));
    label_27->setText(QApplication::translate("FViewAccount", "Predefined Ban Reasons", 0, QApplication::UnicodeUTF8));
    pcBanned->setText(QApplication::translate("FViewAccount", "Currently Banned", 0, QApplication::UnicodeUTF8));
    groupBox_11->setTitle(QApplication::translate("FViewAccount", "Player Comments", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("FViewAccount", "Player Comments", 0, QApplication::UnicodeUTF8));
    pApply->setText(QApplication::translate("FViewAccount", "Apply Changes", 0, QApplication::UnicodeUTF8));
    pCancel->setText(QApplication::translate("FViewAccount", "Cancel Changes", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FViewAccount);
    } // retranslateUi

};

namespace Ui {
    class FViewAccount: public Ui_FViewAccount {};
} // namespace Ui

#endif // UI_VIEWACCOUNT_H
