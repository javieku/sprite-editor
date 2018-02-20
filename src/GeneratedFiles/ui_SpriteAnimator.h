/********************************************************************************
** Form generated from reading UI file 'SpriteAnimator.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPRITEANIMATOR_H
#define UI_SPRITEANIMATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpriteAnimatorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SpriteAnimatorClass)
    {
        if (SpriteAnimatorClass->objectName().isEmpty())
            SpriteAnimatorClass->setObjectName(QStringLiteral("SpriteAnimatorClass"));
        SpriteAnimatorClass->resize(600, 400);
        menuBar = new QMenuBar(SpriteAnimatorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        SpriteAnimatorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SpriteAnimatorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SpriteAnimatorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SpriteAnimatorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SpriteAnimatorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SpriteAnimatorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SpriteAnimatorClass->setStatusBar(statusBar);

        retranslateUi(SpriteAnimatorClass);

        QMetaObject::connectSlotsByName(SpriteAnimatorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SpriteAnimatorClass)
    {
        SpriteAnimatorClass->setWindowTitle(QApplication::translate("SpriteAnimatorClass", "SpriteAnimator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpriteAnimatorClass: public Ui_SpriteAnimatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPRITEANIMATOR_H
