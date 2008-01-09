/********************************************************************************
** Form generated from reading ui file 'textedit.ui'
**
** Created: Sat Aug 25 15:20:36 2007
**      by: Qt User Interface Compiler version 4.3.1
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
#include <QtGui/QWidget>

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout1;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout2;
    QFrame *frame;

    void setupUi(QWidget *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(486, 397);
    gridLayout = new QGridLayout(Form);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout1 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout1->setSpacing(6);
#endif
    gridLayout1->setMargin(6);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    pushButton = new QPushButton(Form);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setMinimumSize(QSize(0, 23));
    pushButton->setMaximumSize(QSize(16777215, 23));

    gridLayout1->addWidget(pushButton, 0, 3, 1, 1);

    pushButton_2 = new QPushButton(Form);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setMinimumSize(QSize(0, 23));
    pushButton_2->setMaximumSize(QSize(16777215, 23));

    gridLayout1->addWidget(pushButton_2, 0, 4, 1, 1);

    pushButton_3 = new QPushButton(Form);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setWindowModality(Qt::NonModal);
    pushButton_3->setMinimumSize(QSize(0, 23));
    pushButton_3->setMaximumSize(QSize(16777215, 23));
    pushButton_3->setAutoDefault(false);
    pushButton_3->setDefault(false);
    pushButton_3->setFlat(false);

    gridLayout1->addWidget(pushButton_3, 0, 0, 1, 1);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout1->addItem(spacerItem, 0, 1, 1, 2);


    gridLayout->addLayout(gridLayout1, 1, 0, 1, 1);

    gridLayout2 = new QGridLayout();
    gridLayout2->setSpacing(0);
    gridLayout2->setMargin(0);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    frame = new QFrame(Form);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::WinPanel);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(1);

    gridLayout2->addWidget(frame, 0, 0, 1, 1);


    gridLayout->addLayout(gridLayout2, 0, 0, 1, 1);


    retranslateUi(Form);

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("Form", "&Send", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("Form", "&Close", 0, QApplication::UnicodeUTF8));
    pushButton_3->setText(QApplication::translate("Form", "&Find", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

#endif // UI_TEXTEDIT_H
