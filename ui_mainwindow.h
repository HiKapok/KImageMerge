/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QPushButton *btnOpenDir;
    QPushButton *btnSelectPath;
    QPushButton *btnMergeImage;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *editHOverlap;
    QLabel *label_2;
    QLineEdit *editVOverlap;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_3;
    QLineEdit *editHCount;
    QLabel *label_4;
    QLineEdit *editVCount;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *cbShowLine;
    QCheckBox *cbSaveColor;
    QVBoxLayout *verticalLayout_6;
    QRadioButton *rbForward;
    QRadioButton *rbBackward;
    QRadioButton *rbSymmetry;
    QRadioButton *rbCenterReflect;
    QVBoxLayout *verticalLayout;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(393, 251);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 30, 311, 194));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        btnOpenDir = new QPushButton(layoutWidget);
        btnOpenDir->setObjectName(QStringLiteral("btnOpenDir"));

        verticalLayout_3->addWidget(btnOpenDir);

        btnSelectPath = new QPushButton(layoutWidget);
        btnSelectPath->setObjectName(QStringLiteral("btnSelectPath"));

        verticalLayout_3->addWidget(btnSelectPath);

        btnMergeImage = new QPushButton(layoutWidget);
        btnMergeImage->setObjectName(QStringLiteral("btnMergeImage"));

        verticalLayout_3->addWidget(btnMergeImage);


        horizontalLayout->addLayout(verticalLayout_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        editHOverlap = new QLineEdit(layoutWidget);
        editHOverlap->setObjectName(QStringLiteral("editHOverlap"));

        verticalLayout_2->addWidget(editHOverlap);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        editVOverlap = new QLineEdit(layoutWidget);
        editVOverlap->setObjectName(QStringLiteral("editVOverlap"));

        verticalLayout_2->addWidget(editVOverlap);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_5->addWidget(label_3);

        editHCount = new QLineEdit(layoutWidget);
        editHCount->setObjectName(QStringLiteral("editHCount"));

        verticalLayout_5->addWidget(editHCount);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_5->addWidget(label_4);

        editVCount = new QLineEdit(layoutWidget);
        editVCount->setObjectName(QStringLiteral("editVCount"));

        verticalLayout_5->addWidget(editVCount);


        horizontalLayout->addLayout(verticalLayout_5);


        verticalLayout_4->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        cbShowLine = new QCheckBox(layoutWidget);
        cbShowLine->setObjectName(QStringLiteral("cbShowLine"));

        verticalLayout_7->addWidget(cbShowLine);

        cbSaveColor = new QCheckBox(layoutWidget);
        cbSaveColor->setObjectName(QStringLiteral("cbSaveColor"));

        verticalLayout_7->addWidget(cbSaveColor);


        horizontalLayout_2->addLayout(verticalLayout_7);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        rbForward = new QRadioButton(layoutWidget);
        rbForward->setObjectName(QStringLiteral("rbForward"));

        verticalLayout_6->addWidget(rbForward);

        rbBackward = new QRadioButton(layoutWidget);
        rbBackward->setObjectName(QStringLiteral("rbBackward"));

        verticalLayout_6->addWidget(rbBackward);

        rbSymmetry = new QRadioButton(layoutWidget);
        rbSymmetry->setObjectName(QStringLiteral("rbSymmetry"));

        verticalLayout_6->addWidget(rbSymmetry);

        rbCenterReflect = new QRadioButton(layoutWidget);
        rbCenterReflect->setObjectName(QStringLiteral("rbCenterReflect"));

        verticalLayout_6->addWidget(rbCenterReflect);


        horizontalLayout_2->addLayout(verticalLayout_6);


        verticalLayout_4->addLayout(horizontalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        verticalLayout_4->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        btnOpenDir->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\347\233\256\345\275\225", 0));
        btnSelectPath->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\344\277\235\345\255\230\350\267\257\345\276\204", 0));
        btnMergeImage->setText(QApplication::translate("MainWindow", "\345\220\210\345\271\266\345\233\276\345\203\217", 0));
        label->setText(QApplication::translate("MainWindow", "\346\260\264\345\271\263\351\207\215\345\217\240", 0));
        label_2->setText(QApplication::translate("MainWindow", "\345\236\202\347\233\264\351\207\215\345\217\240", 0));
        label_3->setText(QApplication::translate("MainWindow", "\350\210\252\345\220\221\347\272\277\344\270\252\346\225\260", 0));
        label_4->setText(QApplication::translate("MainWindow", "\344\277\257\344\273\260\347\272\277\344\270\252\346\225\260", 0));
        cbShowLine->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\345\216\237\345\247\213\345\233\276\345\203\217\350\276\271\347\225\214", 0));
        cbSaveColor->setText(QApplication::translate("MainWindow", "\350\276\223\345\207\27224\344\275\215\345\275\251\350\211\262", 0));
        rbForward->setText(QApplication::translate("MainWindow", "\344\273\216\345\211\215\345\276\200\345\220\216\350\246\206\347\233\226", 0));
        rbBackward->setText(QApplication::translate("MainWindow", "\344\273\216\345\220\216\345\276\200\345\211\215\350\246\206\347\233\226", 0));
        rbSymmetry->setText(QApplication::translate("MainWindow", "\345\257\271\347\247\260\350\246\206\347\233\226", 0));
        rbCenterReflect->setText(QApplication::translate("MainWindow", "\344\273\216\344\270\255\351\227\264\345\220\221\344\270\244\344\276\247", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
