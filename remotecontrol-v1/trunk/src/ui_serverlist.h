/********************************************************************************
** Form generated from reading ui file 'serverlist.ui'
**
** Created: Thu May 1 02:47:56 2008
**      by: Qt User Interface Compiler version 4.3.4
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

class Ui_FServerWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout2;
    QLabel *label_3;
    QLineEdit *eVersion;
    QLineEdit *eLanguage;
    QLabel *lLanguage;
    QLabel *lVersion;
    QTextEdit *eDescription;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QLabel *label;
    QLineEdit *eHomepage;
    QPushButton *pWebsite;
    QTreeWidget *treeWidget;
    QGridLayout *gridLayout3;
    QPushButton *pConnect;
    QPushButton *pRefresh;
    QSpacerItem *spacerItem2;

    void setupUi(QMainWindow *FServerWindow)
    {
    if (FServerWindow->objectName().isEmpty())
        FServerWindow->setObjectName(QString::fromUtf8("FServerWindow"));
    FServerWindow->resize(457, 352);
    FServerWindow->setWindowIcon(QIcon());
    centralwidget = new QWidget(FServerWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(6);
    gridLayout->setVerticalSpacing(6);
    gridLayout->setContentsMargins(9, 9, 9, 9);
    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    gridLayout1 = new QGridLayout(groupBox);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(6);
    gridLayout1->setVerticalSpacing(6);
    gridLayout1->setContentsMargins(9, 9, 9, 9);
    groupBox_2 = new QGroupBox(groupBox);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
    groupBox_2->setSizePolicy(sizePolicy);
    groupBox_2->setMinimumSize(QSize(220, 240));
    groupBox_2->setMaximumSize(QSize(220, 16777215));
    gridLayout2 = new QGridLayout(groupBox_2);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(6);
    gridLayout2->setVerticalSpacing(6);
    gridLayout2->setContentsMargins(9, 9, 9, 9);
    label_3 = new QLabel(groupBox_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout2->addWidget(label_3, 2, 0, 1, 3);

    eVersion = new QLineEdit(groupBox_2);
    eVersion->setObjectName(QString::fromUtf8("eVersion"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
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

    eDescription = new QTextEdit(groupBox_2);
    eDescription->setObjectName(QString::fromUtf8("eDescription"));
    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
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

    label = new QLabel(groupBox_2);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout2->addWidget(label, 4, 0, 1, 1);

    eHomepage = new QLineEdit(groupBox_2);
    eHomepage->setObjectName(QString::fromUtf8("eHomepage"));

    gridLayout2->addWidget(eHomepage, 4, 1, 1, 4);

    pWebsite = new QPushButton(groupBox_2);
    pWebsite->setObjectName(QString::fromUtf8("pWebsite"));
    pWebsite->setMinimumSize(QSize(24, 23));
    pWebsite->setMaximumSize(QSize(24, 23));

    gridLayout2->addWidget(pWebsite, 4, 5, 1, 1);


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
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    gridLayout3->setHorizontalSpacing(6);
    gridLayout3->setVerticalSpacing(6);
    gridLayout3->setContentsMargins(0, 0, 0, 0);
    pConnect = new QPushButton(centralwidget);
    pConnect->setObjectName(QString::fromUtf8("pConnect"));

    gridLayout3->addWidget(pConnect, 0, 2, 1, 1);

    pRefresh = new QPushButton(centralwidget);
    pRefresh->setObjectName(QString::fromUtf8("pRefresh"));

    gridLayout3->addWidget(pRefresh, 0, 1, 1, 1);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout3->addItem(spacerItem2, 0, 0, 1, 1);


    gridLayout->addLayout(gridLayout3, 1, 0, 1, 1);

    FServerWindow->setCentralWidget(centralwidget);
    QWidget::setTabOrder(eLanguage, eVersion);
    QWidget::setTabOrder(eVersion, eDescription);
    QWidget::setTabOrder(eDescription, pRefresh);
    QWidget::setTabOrder(pRefresh, pConnect);
    QWidget::setTabOrder(pConnect, treeWidget);

    retranslateUi(FServerWindow);

    QMetaObject::connectSlotsByName(FServerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *FServerWindow)
    {
    FServerWindow->setWindowTitle(QApplication::translate("FServerWindow", "RemoteControl - Serverlist", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("FServerWindow", "Servers", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("FServerWindow", "Server Info", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("FServerWindow", "Description:", 0, QApplication::UnicodeUTF8));
    lLanguage->setText(QApplication::translate("FServerWindow", "Language", 0, QApplication::UnicodeUTF8));
    lVersion->setText(QApplication::translate("FServerWindow", "Version", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("FServerWindow", "Website", 0, QApplication::UnicodeUTF8));
    pWebsite->setText(QApplication::translate("FServerWindow", ">", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("FServerWindow", "1", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(1, QApplication::translate("FServerWindow", "1", 0, QApplication::UnicodeUTF8));
    pConnect->setText(QApplication::translate("FServerWindow", "&Connect", 0, QApplication::UnicodeUTF8));
    pRefresh->setText(QApplication::translate("FServerWindow", "&Refresh", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FServerWindow);
    } // retranslateUi

};

namespace Ui {
    class FServerWindow: public Ui_FServerWindow {};
} // namespace Ui

#endif // UI_SERVERLIST_H
