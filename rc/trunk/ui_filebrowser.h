/********************************************************************************
** Form generated from reading ui file 'filebrowser.ui'
**
** Created: Tue Sep 4 18:28:24 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FILEBROWSER_H
#define UI_FILEBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

class Ui_CRCBrowser
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QFrame *frame;
    QGridLayout *gridLayout1;
    QTreeWidget *treeWidget;
    QTreeWidget *treeWidget_2;
    QTextEdit *textEdit;

    void setupUi(QWidget *CRCBrowser)
    {
    if (CRCBrowser->objectName().isEmpty())
        CRCBrowser->setObjectName(QString::fromUtf8("CRCBrowser"));
    CRCBrowser->resize(701, 504);
    gridLayout = new QGridLayout(CRCBrowser);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(6);
    gridLayout->setContentsMargins(0, 6, 0, 0);
    label = new QLabel(CRCBrowser);
    label->setObjectName(QString::fromUtf8("label"));
    label->setMaximumSize(QSize(16777215, 23));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    frame = new QFrame(CRCBrowser);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    gridLayout1 = new QGridLayout(frame);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(0);
    gridLayout1->setVerticalSpacing(0);
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    treeWidget = new QTreeWidget(frame);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setMaximumSize(QSize(175, 16777215));
    treeWidget->setRootIsDecorated(true);
    treeWidget->setColumnCount(0);

    gridLayout1->addWidget(treeWidget, 0, 0, 1, 1);

    treeWidget_2 = new QTreeWidget(frame);
    treeWidget_2->setObjectName(QString::fromUtf8("treeWidget_2"));
    treeWidget_2->setIndentation(20);
    treeWidget_2->setRootIsDecorated(false);
    treeWidget_2->setUniformRowHeights(true);
    treeWidget_2->setItemsExpandable(false);
    treeWidget_2->setSortingEnabled(true);
    treeWidget_2->setColumnCount(4);

    gridLayout1->addWidget(treeWidget_2, 0, 1, 1, 1);

    textEdit = new QTextEdit(frame);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setMaximumSize(QSize(16777215, 125));

    gridLayout1->addWidget(textEdit, 1, 0, 1, 2);


    gridLayout->addWidget(frame, 1, 0, 1, 1);


    retranslateUi(CRCBrowser);

    QMetaObject::connectSlotsByName(CRCBrowser);
    } // setupUi

    void retranslateUi(QWidget *CRCBrowser)
    {
    CRCBrowser->setWindowTitle(QApplication::translate("CRCBrowser", "Form", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CRCBrowser", "Current folder: ./", 0, QApplication::UnicodeUTF8));
    treeWidget_2->headerItem()->setText(0, QApplication::translate("CRCBrowser", "Filename", 0, QApplication::UnicodeUTF8));
    treeWidget_2->headerItem()->setText(1, QApplication::translate("CRCBrowser", "Rights", 0, QApplication::UnicodeUTF8));
    treeWidget_2->headerItem()->setText(2, QApplication::translate("CRCBrowser", "Size", 0, QApplication::UnicodeUTF8));
    treeWidget_2->headerItem()->setText(3, QApplication::translate("CRCBrowser", "Modified", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CRCBrowser);
    } // retranslateUi

};

namespace Ui {
    class CRCBrowser: public Ui_CRCBrowser {};
} // namespace Ui

#endif // UI_FILEBROWSER_H
