#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SpriteAnimator.h"

class SpriteAnimator : public QMainWindow
{
    Q_OBJECT

public:
    SpriteAnimator( QWidget* parent = Q_NULLPTR );

private:
    Ui::SpriteAnimatorClass ui;
};
