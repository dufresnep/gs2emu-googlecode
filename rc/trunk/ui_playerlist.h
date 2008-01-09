/********************************************************************************
** Form generated from reading ui file 'playerlist.ui'
**
** Created: Sat Aug 25 15:20:36 2007
**      by: Qt User Interface Compiler version 4.3.1
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
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout2;
    QTreeWidget *treeWidget;

    void setupUi(QWidget *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(436, 313);
    gridLayout = new QGridLayout(Form);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout1->setMargin(0);
#endif
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    pushButton = new QPushButton(Form);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    gridLayout1->addWidget(pushButton, 0, 0, 1, 1);

    pushButton_3 = new QPushButton(Form);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

    gridLayout1->addWidget(pushButton_3, 0, 3, 1, 1);

    pushButton_2 = new QPushButton(Form);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    gridLayout1->addWidget(pushButton_2, 0, 1, 1, 1);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem, 0, 2, 1, 1);


    gridLayout->addLayout(gridLayout1, 1, 0, 1, 1);

    gridLayout2 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout2->setSpacing(6);
#endif
    gridLayout2->setMargin(0);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    treeWidget = new QTreeWidget(Form);
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


    retranslateUi(Form);

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("Form", "&Admin Message to All", 0, QApplication::UnicodeUTF8));
    pushButton_3->setText(QApplication::translate("Form", "&Close", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("Form", "&Private Message to All", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("Form", "1", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(1, QApplication::translate("Form", "1", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(2, QApplication::translate("Form", "3", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(3, QApplication::translate("Form", "4", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

#endif // UI_PLAYERLIST_H
