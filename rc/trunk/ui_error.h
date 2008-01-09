/********************************************************************************
** Form generated from reading ui file 'error.ui'
**
** Created: Sat Jul 28 18:15:25 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ERROR_H
#define UI_ERROR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

class Ui_CRCError
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QPushButton *pOK;
    QSpacerItem *spacerItem1;
    QLabel *lError;

    void setupUi(QMainWindow *CRCError)
    {
    CRCError->setObjectName(QString::fromUtf8("CRCError"));
    CRCError->setWindowIcon(QIcon(QString::fromUtf8("remotecontrol.png")));
    centralwidget = new QWidget(CRCError);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    spacerItem = new QSpacerItem(20, 161, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(spacerItem, 0, 2, 2, 1);

    pOK = new QPushButton(centralwidget);
    pOK->setObjectName(QString::fromUtf8("pOK"));

    gridLayout->addWidget(pOK, 1, 1, 1, 1);

    spacerItem1 = new QSpacerItem(281, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem1, 1, 0, 1, 1);

    lError = new QLabel(centralwidget);
    lError->setObjectName(QString::fromUtf8("lError"));
    lError->setTextFormat(Qt::AutoText);

    gridLayout->addWidget(lError, 0, 0, 1, 2);

    CRCError->setCentralWidget(centralwidget);

    retranslateUi(CRCError);

    QSize size(400, 100);
    size = size.expandedTo(CRCError->minimumSizeHint());
    CRCError->resize(size);


    QMetaObject::connectSlotsByName(CRCError);
    } // setupUi

    void retranslateUi(QMainWindow *CRCError)
    {
    CRCError->setWindowTitle(QApplication::translate("CRCError", "Error", 0, QApplication::UnicodeUTF8));
    pOK->setText(QApplication::translate("CRCError", "&OK", 0, QApplication::UnicodeUTF8));
    lError->setText(QApplication::translate("CRCError", "<center>Error</center>", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CRCError);
    } // retranslateUi

};

namespace Ui {
    class CRCError: public Ui_CRCError {};
} // namespace Ui

#endif // UI_ERROR_H
