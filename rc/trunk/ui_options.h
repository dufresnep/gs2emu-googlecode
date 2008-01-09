/********************************************************************************
** Form generated from reading ui file 'options.ui'
**
** Created: Sat Aug 25 15:20:36 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QGridLayout *gridLayout2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_4;
    QGridLayout *gridLayout3;
    QLineEdit *lineEdit;
    QLabel *label;
    QSpacerItem *spacerItem;
    QGridLayout *gridLayout4;
    QPushButton *pushButton_1;
    QSpacerItem *spacerItem1;

    void setupUi(QWidget *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(346, 236);
    gridLayout = new QGridLayout(Form);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    groupBox = new QGroupBox(Form);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    gridLayout1 = new QGridLayout(groupBox);
#ifndef Q_OS_MAC
    gridLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout1->setMargin(9);
#endif
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout2 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout2->setMargin(0);
#endif
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    checkBox = new QCheckBox(groupBox);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));

    gridLayout2->addWidget(checkBox, 0, 0, 1, 1);

    checkBox_3 = new QCheckBox(groupBox);
    checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

    gridLayout2->addWidget(checkBox_3, 1, 0, 1, 1);

    checkBox_2 = new QCheckBox(groupBox);
    checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

    gridLayout2->addWidget(checkBox_2, 3, 0, 1, 1);

    checkBox_4 = new QCheckBox(groupBox);
    checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));

    gridLayout2->addWidget(checkBox_4, 2, 0, 1, 1);


    gridLayout1->addLayout(gridLayout2, 0, 0, 1, 1);

    gridLayout3 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout3->setSpacing(6);
#endif
    gridLayout3->setMargin(0);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    lineEdit = new QLineEdit(groupBox);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    gridLayout3->addWidget(lineEdit, 0, 1, 1, 1);

    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout3->addWidget(label, 0, 0, 1, 1);


    gridLayout1->addLayout(gridLayout3, 1, 0, 1, 1);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem, 2, 0, 1, 1);


    gridLayout->addWidget(groupBox, 0, 0, 1, 1);

    gridLayout4 = new QGridLayout();
#ifndef Q_OS_MAC
    gridLayout4->setSpacing(6);
#endif
    gridLayout4->setMargin(0);
    gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
    pushButton_1 = new QPushButton(Form);
    pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
    pushButton_1->setWindowModality(Qt::NonModal);
    pushButton_1->setMinimumSize(QSize(90, 23));
    pushButton_1->setMaximumSize(QSize(90, 23));
    pushButton_1->setAutoDefault(false);
    pushButton_1->setDefault(false);
    pushButton_1->setFlat(false);

    gridLayout4->addWidget(pushButton_1, 0, 1, 1, 1);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout4->addItem(spacerItem1, 0, 0, 1, 1);


    gridLayout->addLayout(gridLayout4, 1, 0, 1, 1);


    retranslateUi(Form);

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("Form", "GroupBox", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("Form", "CheckBox", 0, QApplication::UnicodeUTF8));
    checkBox_3->setText(QApplication::translate("Form", "CheckBox", 0, QApplication::UnicodeUTF8));
    checkBox_2->setText(QApplication::translate("Form", "CheckBox", 0, QApplication::UnicodeUTF8));
    checkBox_4->setText(QApplication::translate("Form", "CheckBox", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Form", "TextLabel", 0, QApplication::UnicodeUTF8));
    pushButton_1->setText(QApplication::translate("Form", "&Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

#endif // UI_OPTIONS_H
