/********************************************************************************
** Form generated from reading ui file 'login.ui'
**
** Created: Sat Aug 25 15:20:36 2007
**      by: Qt User Interface Compiler version 4.3.1
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

class Ui_CRCLogin
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
    QSpacerItem *spacerItem1;
    QLineEdit *lNickname;
    QLabel *label;
    QLineEdit *lPassword;
    QLabel *label_2;
    QLabel *label_3;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QSpacerItem *spacerItem4;

    void setupUi(QMainWindow *CRCLogin)
    {
    if (CRCLogin->objectName().isEmpty())
        CRCLogin->setObjectName(QString::fromUtf8("CRCLogin"));
    CRCLogin->resize(266, 231);
    CRCLogin->setWindowIcon(QIcon(QString::fromUtf8(":/RC/remotecontrol.png")));
    centralwidget = new QWidget(CRCLogin);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
#ifndef Q_OS_MAC
    gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayout->setMargin(9);
#endif
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    hboxLayout->setMargin(0);
#endif
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    BConnect = new QPushButton(centralwidget);
    BConnect->setObjectName(QString::fromUtf8("BConnect"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
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
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy1);
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
    lAccount = new QLineEdit(groupBox);
    lAccount->setObjectName(QString::fromUtf8("lAccount"));

    gridLayout2->addWidget(lAccount, 1, 2, 1, 1);

    spacerItem1 = new QSpacerItem(41, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem1, 0, 1, 1, 1);

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

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem2, 1, 1, 1, 1);

    spacerItem3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout2->addItem(spacerItem3, 2, 1, 1, 1);


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

    CRCLogin->setCentralWidget(centralwidget);
    QWidget::setTabOrder(lNickname, lAccount);
    QWidget::setTabOrder(lAccount, lPassword);
    QWidget::setTabOrder(lPassword, checkBox);
    QWidget::setTabOrder(checkBox, checkBox_2);
    QWidget::setTabOrder(checkBox_2, BConnect);
    QWidget::setTabOrder(BConnect, BCancel);

    retranslateUi(CRCLogin);

    QMetaObject::connectSlotsByName(CRCLogin);
    } // setupUi

    void retranslateUi(QMainWindow *CRCLogin)
    {
    CRCLogin->setWindowTitle(QApplication::translate("CRCLogin", "RemoteControl - Login", 0, QApplication::UnicodeUTF8));
    CRCLogin->setStyleSheet(QString());
    BConnect->setText(QApplication::translate("CRCLogin", "&OK", 0, QApplication::UnicodeUTF8));
    BCancel->setText(QApplication::translate("CRCLogin", "&Cancel", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("CRCLogin", "Options", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CRCLogin", "Nickname:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("CRCLogin", "Account:", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("CRCLogin", "Password:", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("CRCLogin", "Don't save password (Disabled)", 0, QApplication::UnicodeUTF8));
    checkBox_2->setText(QApplication::translate("CRCLogin", "Graphical Menu (Disabled)", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CRCLogin);
    } // retranslateUi

};

namespace Ui {
    class CRCLogin: public Ui_CRCLogin {};
} // namespace Ui

#endif // UI_LOGIN_H
