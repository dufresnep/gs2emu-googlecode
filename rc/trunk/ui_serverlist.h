/********************************************************************************
** Form generated from reading ui file 'serverlist.ui'
**
** Created: Sat Aug 25 15:20:36 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SERVERLIST_H
#define UI_SERVERLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

class Ui_CRCSList
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout2;
    QLabel *label_3;
    QPushButton *pOpenURL;
    QLineEdit *eVersion;
    QLineEdit *eLanguage;
    QLabel *lLanguage;
    QLabel *lVersion;
    QLabel *lWebsite;
    QTextEdit *eDescription;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QLineEdit *eHomepage;
    QTreeWidget *treeWidget;
    QGridLayout *gridLayout3;
    QPushButton *pConnect;
    QPushButton *pRefresh;
    QSpacerItem *spacerItem2;

    void setupUi(QMainWindow *CRCSList)
    {
    if (CRCSList->objectName().isEmpty())
        CRCSList->setObjectName(QString::fromUtf8("CRCSList"));
    CRCSList->resize(457, 352);
    CRCSList->setWindowIcon(QIcon(QString::fromUtf8(":/RC/remotecontrol.png")));
    centralwidget = new QWidget(CRCSList);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    gridLayout1 = new QGridLayout(groupBox);
#ifndef Q_OS_MAC
    gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout1->setMargin(9);
#endif
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    groupBox_2 = new QGroupBox(groupBox);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(5));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
    groupBox_2->setSizePolicy(sizePolicy);
    groupBox_2->setMinimumSize(QSize(220, 240));
    groupBox_2->setMaximumSize(QSize(220, 16777215));
    gridLayout2 = new QGridLayout(groupBox_2);
#ifndef Q_OS_MAC
    gridLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout2->setMargin(9);
#endif
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    label_3 = new QLabel(groupBox_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout2->addWidget(label_3, 2, 0, 1, 3);

    pOpenURL = new QPushButton(groupBox_2);
    pOpenURL->setObjectName(QString::fromUtf8("pOpenURL"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(pOpenURL->sizePolicy().hasHeightForWidth());
    pOpenURL->setSizePolicy(sizePolicy1);
    pOpenURL->setMinimumSize(QSize(20, 0));
    pOpenURL->setMaximumSize(QSize(20, 16777215));

    gridLayout2->addWidget(pOpenURL, 4, 5, 1, 1);

    eVersion = new QLineEdit(groupBox_2);
    eVersion->setObjectName(QString::fromUtf8("eVersion"));
    sizePolicy1.setHeightForWidth(eVersion->sizePolicy().hasHeightForWidth());
    eVersion->setSizePolicy(sizePolicy1);
    eVersion->setMinimumSize(QSize(110, 0));
    eVersion->setMaximumSize(QSize(110, 16777215));

    gridLayout2->addWidget(eVersion, 1, 2, 1, 4);

    eLanguage = new QLineEdit(groupBox_2);
    eLanguage->setObjectName(QString::fromUtf8("eLanguage"));
    sizePolicy1.setHeightForWidth(eLanguage->sizePolicy().hasHeightForWidth());
    eLanguage->setSizePolicy(sizePolicy1);
    eLanguage->setMinimumSize(QSize(110, 0));
    eLanguage->setMaximumSize(QSize(110, 16777215));

    gridLayout2->addWidget(eLanguage, 0, 2, 1, 4);

    lLanguage = new QLabel(groupBox_2);
    lLanguage->setObjectName(QString::fromUtf8("lLanguage"));

    gridLayout2->addWidget(lLanguage, 0, 0, 1, 1);

    lVersion = new QLabel(groupBox_2);
    lVersion->setObjectName(QString::fromUtf8("lVersion"));

    gridLayout2->addWidget(lVersion, 1, 0, 1, 1);

    lWebsite = new QLabel(groupBox_2);
    lWebsite->setObjectName(QString::fromUtf8("lWebsite"));
    sizePolicy.setHeightForWidth(lWebsite->sizePolicy().hasHeightForWidth());
    lWebsite->setSizePolicy(sizePolicy);
    lWebsite->setMinimumSize(QSize(50, 0));
    lWebsite->setMaximumSize(QSize(50, 16777215));

    gridLayout2->addWidget(lWebsite, 4, 0, 1, 1);

    eDescription = new QTextEdit(groupBox_2);
    eDescription->setObjectName(QString::fromUtf8("eDescription"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(7));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(eDescription->sizePolicy().hasHeightForWidth());
    eDescription->setSizePolicy(sizePolicy2);
    eDescription->setMinimumSize(QSize(200, 0));
    eDescription->setMaximumSize(QSize(200, 16777215));

    gridLayout2->addWidget(eDescription, 3, 0, 1, 6);

    spacerItem = new QSpacerItem(30, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem, 1, 1, 1, 1);

    spacerItem1 = new QSpacerItem(30, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem1, 0, 1, 1, 1);

    eHomepage = new QLineEdit(groupBox_2);
    eHomepage->setObjectName(QString::fromUtf8("eHomepage"));
    sizePolicy1.setHeightForWidth(eHomepage->sizePolicy().hasHeightForWidth());
    eHomepage->setSizePolicy(sizePolicy1);
    eHomepage->setMinimumSize(QSize(125, 0));
    eHomepage->setMaximumSize(QSize(125, 16777215));

    gridLayout2->addWidget(eHomepage, 4, 1, 1, 2);


    gridLayout1->addWidget(groupBox_2, 0, 1, 1, 1);

    treeWidget = new QTreeWidget(groupBox);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setLineWidth(1);
    treeWidget->setMidLineWidth(0);
    treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    treeWidget->setIndentation(20);
    treeWidget->setRootIsDecorated(false);
    treeWidget->setUniformRowHeights(true);
    treeWidget->setItemsExpandable(false);
    treeWidget->setSortingEnabled(true);
    treeWidget->setColumnCount(2);

    gridLayout1->addWidget(treeWidget, 0, 0, 2, 1);


    gridLayout->addWidget(groupBox, 0, 0, 1, 1);

    gridLayout3 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout3->setSpacing(6);
#endif
    gridLayout3->setMargin(0);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    pConnect = new QPushButton(centralwidget);
    pConnect->setObjectName(QString::fromUtf8("pConnect"));

    gridLayout3->addWidget(pConnect, 0, 2, 1, 1);

    pRefresh = new QPushButton(centralwidget);
    pRefresh->setObjectName(QString::fromUtf8("pRefresh"));

    gridLayout3->addWidget(pRefresh, 0, 1, 1, 1);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout3->addItem(spacerItem2, 0, 0, 1, 1);


    gridLayout->addLayout(gridLayout3, 1, 0, 1, 1);

    CRCSList->setCentralWidget(centralwidget);
    QWidget::setTabOrder(eLanguage, eVersion);
    QWidget::setTabOrder(eVersion, eDescription);
    QWidget::setTabOrder(eDescription, eHomepage);
    QWidget::setTabOrder(eHomepage, pOpenURL);
    QWidget::setTabOrder(pOpenURL, pRefresh);
    QWidget::setTabOrder(pRefresh, pConnect);
    QWidget::setTabOrder(pConnect, treeWidget);

    retranslateUi(CRCSList);

    QMetaObject::connectSlotsByName(CRCSList);
    } // setupUi

    void retranslateUi(QMainWindow *CRCSList)
    {
    CRCSList->setWindowTitle(QApplication::translate("CRCSList", "RemoteControl - Serverlist", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("CRCSList", "Servers", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("CRCSList", "Server Info", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("CRCSList", "Description:", 0, QApplication::UnicodeUTF8));
    pOpenURL->setText(QApplication::translate("CRCSList", ">", 0, QApplication::UnicodeUTF8));
    lLanguage->setText(QApplication::translate("CRCSList", "Language", 0, QApplication::UnicodeUTF8));
    lVersion->setText(QApplication::translate("CRCSList", "Version", 0, QApplication::UnicodeUTF8));
    lWebsite->setText(QApplication::translate("CRCSList", "Website", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("CRCSList", "1", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(1, QApplication::translate("CRCSList", "1", 0, QApplication::UnicodeUTF8));
    pConnect->setText(QApplication::translate("CRCSList", "&Connect", 0, QApplication::UnicodeUTF8));
    pRefresh->setText(QApplication::translate("CRCSList", "&Refresh", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CRCSList);
    } // retranslateUi

};

namespace Ui {
    class CRCSList: public Ui_CRCSList {};
} // namespace Ui

#endif // UI_SERVERLIST_H
