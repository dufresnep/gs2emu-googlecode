/********************************************************************************
** Form generated from reading ui file 'textedit.ui'
**
** Created: Thu May 1 02:47:56 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TEXTEDIT_H
#define UI_TEXTEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_FTextEdit
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QPushButton *BSend;
    QPushButton *BClose;
    QPushButton *BFind;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout2;
    QFrame *frame;
    QGridLayout *gridLayout3;
    QTextEdit *textEdit;

    void setupUi(QWidget *FTextEdit)
    {
    if (FTextEdit->objectName().isEmpty())
        FTextEdit->setObjectName(QString::fromUtf8("FTextEdit"));
    FTextEdit->resize(478, 397);
    gridLayout = new QGridLayout(FTextEdit);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout1 = new QGridLayout();
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(6);
    gridLayout1->setVerticalSpacing(6);
    gridLayout1->setContentsMargins(6, 6, 6, 6);
    BSend = new QPushButton(FTextEdit);
    BSend->setObjectName(QString::fromUtf8("BSend"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(BSend->sizePolicy().hasHeightForWidth());
    BSend->setSizePolicy(sizePolicy);
    BSend->setMinimumSize(QSize(0, 23));
    BSend->setMaximumSize(QSize(16777215, 23));

    gridLayout1->addWidget(BSend, 0, 3, 1, 1);

    BClose = new QPushButton(FTextEdit);
    BClose->setObjectName(QString::fromUtf8("BClose"));
    sizePolicy.setHeightForWidth(BClose->sizePolicy().hasHeightForWidth());
    BClose->setSizePolicy(sizePolicy);
    BClose->setMinimumSize(QSize(0, 23));
    BClose->setMaximumSize(QSize(16777215, 23));

    gridLayout1->addWidget(BClose, 0, 4, 1, 1);

    BFind = new QPushButton(FTextEdit);
    BFind->setObjectName(QString::fromUtf8("BFind"));
    BFind->setWindowModality(Qt::NonModal);
    sizePolicy.setHeightForWidth(BFind->sizePolicy().hasHeightForWidth());
    BFind->setSizePolicy(sizePolicy);
    BFind->setMinimumSize(QSize(0, 23));
    BFind->setMaximumSize(QSize(16777215, 23));
    BFind->setAutoDefault(false);
    BFind->setDefault(false);
    BFind->setFlat(false);

    gridLayout1->addWidget(BFind, 0, 0, 1, 1);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem, 0, 1, 1, 2);


    gridLayout->addLayout(gridLayout1, 1, 0, 1, 1);

    gridLayout2 = new QGridLayout();
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(0);
    gridLayout2->setVerticalSpacing(0);
    gridLayout2->setContentsMargins(0, 0, 0, 0);
    frame = new QFrame(FTextEdit);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::WinPanel);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(1);
    gridLayout3 = new QGridLayout(frame);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    gridLayout3->setHorizontalSpacing(0);
    gridLayout3->setVerticalSpacing(0);
    gridLayout3->setContentsMargins(0, 0, 0, 0);
    textEdit = new QTextEdit(frame);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setLineWrapMode(QTextEdit::NoWrap);
    textEdit->setTabStopWidth(80);
    textEdit->setAcceptRichText(false);

    gridLayout3->addWidget(textEdit, 0, 0, 1, 1);


    gridLayout2->addWidget(frame, 0, 0, 1, 1);


    gridLayout->addLayout(gridLayout2, 0, 0, 1, 1);


    retranslateUi(FTextEdit);

    QMetaObject::connectSlotsByName(FTextEdit);
    } // setupUi

    void retranslateUi(QWidget *FTextEdit)
    {
    FTextEdit->setWindowTitle(QApplication::translate("FTextEdit", "Form", 0, QApplication::UnicodeUTF8));
    BSend->setText(QApplication::translate("FTextEdit", "&Send", 0, QApplication::UnicodeUTF8));
    BClose->setText(QApplication::translate("FTextEdit", "&Close", 0, QApplication::UnicodeUTF8));
    BFind->setText(QApplication::translate("FTextEdit", "&Find", 0, QApplication::UnicodeUTF8));
    textEdit->setHtml(QApplication::translate("FTextEdit", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Sans Serif'; font-size:9pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FTextEdit);
    } // retranslateUi

};

namespace Ui {
    class FTextEdit: public Ui_FTextEdit {};
} // namespace Ui

#endif // UI_TEXTEDIT_H
