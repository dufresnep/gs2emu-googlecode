/********************************************************************************
** Form generated from reading ui file 'chat.ui'
**
** Created: Sat Aug 25 15:20:36 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QListWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_CRCChat
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit;
    QListWidget *listWidget;

    void setupUi(QWidget *CRCChat)
    {
    if (CRCChat->objectName().isEmpty())
        CRCChat->setObjectName(QString::fromUtf8("CRCChat"));
    CRCChat->resize(454, 194);
    gridLayout = new QGridLayout(CRCChat);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    textEdit = new QTextEdit(CRCChat);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    textEdit->setAcceptRichText(true);
    textEdit->setTextInteractionFlags(Qt::TextBrowserInteraction);

    gridLayout->addWidget(textEdit, 0, 0, 1, 1);

    listWidget = new QListWidget(CRCChat);
    listWidget->setObjectName(QString::fromUtf8("listWidget"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(7));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
    listWidget->setSizePolicy(sizePolicy);
    listWidget->setMinimumSize(QSize(135, 0));
    listWidget->setMaximumSize(QSize(135, 16777215));
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listWidget->setSortingEnabled(true);

    gridLayout->addWidget(listWidget, 0, 1, 1, 1);


    retranslateUi(CRCChat);

    QMetaObject::connectSlotsByName(CRCChat);
    } // setupUi

    void retranslateUi(QWidget *CRCChat)
    {
    CRCChat->setWindowTitle(QApplication::translate("CRCChat", "Form", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CRCChat);
    } // retranslateUi

};

namespace Ui {
    class CRCChat: public Ui_CRCChat {};
} // namespace Ui

#endif // UI_CHAT_H
