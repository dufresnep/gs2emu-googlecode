/********************************************************************************
** Form generated from reading ui file 'rctab.ui'
**
** Created: Thu May 1 02:47:56 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_RCTAB_H
#define UI_RCTAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_FRCTab
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *FRCTab)
    {
    if (FRCTab->objectName().isEmpty())
        FRCTab->setObjectName(QString::fromUtf8("FRCTab"));
    FRCTab->resize(454, 190);
    gridLayout = new QGridLayout(FRCTab);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    textEdit = new QTextEdit(FRCTab);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setFrameShape(QFrame::NoFrame);
    textEdit->setFrameShadow(QFrame::Plain);
    textEdit->setLineWidth(0);
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    textEdit->setAcceptRichText(true);
    textEdit->setTextInteractionFlags(Qt::TextBrowserInteraction);

    gridLayout->addWidget(textEdit, 0, 0, 1, 1);


    retranslateUi(FRCTab);

    QMetaObject::connectSlotsByName(FRCTab);
    } // setupUi

    void retranslateUi(QWidget *FRCTab)
    {
    FRCTab->setWindowTitle(QApplication::translate("FRCTab", "Form", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FRCTab);
    } // retranslateUi

};

namespace Ui {
    class FRCTab: public Ui_FRCTab {};
} // namespace Ui

#endif // UI_RCTAB_H
