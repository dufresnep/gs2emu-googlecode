/********************************************************************************
** Form generated from reading ui file 'login.ui'
**
** Created: Thu May 1 02:47:56 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

class Ui_FLoginWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *BConnect;
    QPushButton *BCancel;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QGridLayout *gridLayout2;
    QLineEdit *lAccount;
    QLineEdit *lNickname;
    QLabel *label;
    QLineEdit *lPassword;
    QLabel *label_2;
    QLabel *label_3;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QSpacerItem *spacerItem4;

    void setupUi(QMainWindow *FLoginWindow)
    {
    if (FLoginWindow->objectName().isEmpty())
        FLoginWindow->setObjectName(QString::fromUtf8("FLoginWindow"));
    FLoginWindow->resize(266, 231);
    FLoginWindow->setWindowIcon(QIcon(QString::fromUtf8(":/RC/remotecontrol.png")));
    centralwidget = new QWidget(FLoginWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(6);
    gridLayout->setVerticalSpacing(6);
    gridLayout->setContentsMargins(9, 9, 9, 9);
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    spacerItem = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    BConnect = new QPushButton(centralwidget);
    BConnect->setObjectName(QString::fromUtf8("BConnect"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(BConnect->sizePolicy().hasHeightForWidth());
    BConnect->setSizePolicy(sizePolicy);

    hboxLayout->addWidget(BConnect);

    BCancel = new QPushButton(centralwidget);
    BCancel->setObjectName(QString::fromUtf8("BCancel"));
    sizePolicy.setHeightForWidth(BCancel->sizePolicy().hasHeightForWidth());
    BCancel->setSizePolicy(sizePolicy);

    hboxLayout->addWidget(BCancel);


    gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);

    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy1);
    gridLayout1 = new QGridLayout(groupBox);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setHorizontalSpacing(6);
    gridLayout1->setVerticalSpacing(6);
    gridLayout1->setContentsMargins(9, 9, 9, 9);
    gridLayout2 = new QGridLayout();
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setHorizontalSpacing(6);
    gridLayout2->setVerticalSpacing(6);
    gridLayout2->setContentsMargins(0, 0, 0, 0);
    lAccount = new QLineEdit(groupBox);
    lAccount->setObjectName(QString::fromUtf8("lAccount"));

    gridLayout2->addWidget(lAccount, 1, 2, 1, 1);

    lNickname = new QLineEdit(groupBox);
    lNickname->setObjectName(QString::fromUtf8("lNickname"));

    gridLayout2->addWidget(lNickname, 0, 2, 1, 1);

    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));
    sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy);

    gridLayout2->addWidget(label, 0, 0, 1, 1);

    lPassword = new QLineEdit(groupBox);
    lPassword->setObjectName(QString::fromUtf8("lPassword"));
    lPassword->setEchoMode(QLineEdit::Password);

    gridLayout2->addWidget(lPassword, 2, 2, 1, 1);

    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy);

    gridLayout2->addWidget(label_2, 1, 0, 1, 1);

    label_3 = new QLabel(groupBox);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
    label_3->setSizePolicy(sizePolicy);

    gridLayout2->addWidget(label_3, 2, 0, 1, 1);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem1, 1, 1, 1, 1);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem2, 2, 1, 1, 1);

    spacerItem3 = new QSpacerItem(41, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem3, 0, 1, 1, 1);


    gridLayout1->addLayout(gridLayout2, 0, 0, 1, 1);

    checkBox = new QCheckBox(groupBox);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));

    gridLayout1->addWidget(checkBox, 1, 0, 1, 1);

    checkBox_2 = new QCheckBox(groupBox);
    checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
    checkBox_2->setCheckable(true);
    checkBox_2->setChecked(true);

    gridLayout1->addWidget(checkBox_2, 2, 0, 1, 1);

    spacerItem4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem4, 3, 0, 1, 1);


    gridLayout->addWidget(groupBox, 0, 0, 1, 1);

    FLoginWindow->setCentralWidget(centralwidget);
    QWidget::setTabOrder(lNickname, lAccount);
    QWidget::setTabOrder(lAccount, lPassword);
    QWidget::setTabOrder(lPassword, checkBox);
    QWidget::setTabOrder(checkBox, checkBox_2);
    QWidget::setTabOrder(checkBox_2, BConnect);
    QWidget::setTabOrder(BConnect, BCancel);

    retranslateUi(FLoginWindow);
    QObject::connect(BCancel, SIGNAL(released()), FLoginWindow, SLOT(close()));

    QMetaObject::connectSlotsByName(FLoginWindow);
    } // setupUi

    void retranslateUi(QMainWindow *FLoginWindow)
    {
    FLoginWindow->setWindowTitle(QApplication::translate("FLoginWindow", "RemoteControl - Login", 0, QApplication::UnicodeUTF8));
    FLoginWindow->setStyleSheet(QString());
    BConnect->setText(QApplication::translate("FLoginWindow", "&OK", 0, QApplication::UnicodeUTF8));
    BCancel->setText(QApplication::translate("FLoginWindow", "&Cancel", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("FLoginWindow", "Options", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("FLoginWindow", "Nickname:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("FLoginWindow", "Account:", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("FLoginWindow", "Password:", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("FLoginWindow", "Don't save password (Disabled)", 0, QApplication::UnicodeUTF8));
    checkBox_2->setText(QApplication::translate("FLoginWindow", "Graphical Menu (Disabled)", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FLoginWindow);
    } // retranslateUi

};

namespace Ui {
    class FLoginWindow: public Ui_FLoginWindow {};
} // namespace Ui

#endif // UI_LOGIN_H
