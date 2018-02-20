
#ifndef SPRITEPREVIEWWIDGET_H
#define SPRITEPREVIEWWIDGET_H

#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include "Sprite.h"

class QPushButton;
class QLabel;
class ImageLoader;

/**
 *   This widget draws on a separate window each frame in the spritesheet using a timer and
 * overrriding QWidget's paint method. It uses the same QImage object as CanvasWidget so that we can
 * see the canvas changes in the preview.
 */

class SpritePreviewWidget : public QWidget
{
    Q_OBJECT
public:
    SpritePreviewWidget( QWidget* parent = 0 );
    ~SpritePreviewWidget( );

    void play( );
    void pause( );
    void reset( );

public slots:
    void spriteLoaded( );

protected:
    void paintEvent( QPaintEvent* );

private:
    QPoint oldPos;
    SpriteSharedPointer _sprite;
    QTimer _updateViewTimer;
    QPushButton* _playbackButton;
    QLabel* _imageLabel;

    ImageLoader* _imageLoader;

private slots:
    void updateSprite( );
    void handlePlayback( );
};

#endif  // SPRITEPREVIEWWIDGET_H