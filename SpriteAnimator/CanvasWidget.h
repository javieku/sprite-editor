#pragma once

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QObject>
#include <QWidget>
#include "ImageLoader.h"
#include "Sprite.h"

class QPaintEvent;
class QMouseEvent;
class QLabel;

/**
 *   This widget manages the drawing area. It has a reference to a Sprite object
 *   whose image is modified by drawing pixels using QImage API.
 *   It supports a QScrollArea to navigate through the scene.
 *
 *	Another possibility instead of using this widget would have been to create a QGraphicView and a
 *QGraphicScene where we could draw our images or primitives and it includes an Scrollable area. The
 *problem I had with this approach is that I wasn't sure at the beginning if I could do pixel
 *modification with those high level classes. Now due to time restrictions I can't use this
 *approach.
 *
 *   The main problem with this widget-subclassing approach is the need of using a QScrollArea whose
 *implementation and use is not very clean and requires more time to be implemented. Because it
 *introduces more bugs and unclear code in order to send a functional application I have removed
 *the QScrollArea object.
 */

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    CanvasWidget( QWidget* parent = 0 );
    ~CanvasWidget( );

    // This function will require some structure to pass the configuration to draw the pixel
    bool drawPixel( QPoint pos, QColor color, int penWidth );
    bool deletePixel( QPoint pos, int penWidth );

signals:
    void pixelDraw( QPoint pos, QColor prev, QColor current, int penWidth );

public slots:
    QColor drawColor( );
    void setDrawColor( QColor color );
    void setPenWidth( int newWidth );
    void spriteLoaded( );

protected:
    void paintEvent( QPaintEvent* );
    void mousePressEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* );
    void resizeEvent( QResizeEvent* );
    void wheelEvent( QWheelEvent* );

private:
    QColor _drawColor;
    SpriteSharedPointer _sprite;
    int _nFrame;
    qreal _scale;
    QImage* _canvas;
    QPoint _lastPoint;
    int _penWidth;

    ImageLoader* _imageLoader;

    // Helpers to change scale factor when mouse wheel events are generated
    // This should be complementary with the QScrollbar implementation
    void zoomIn( );
    void zoomOut( );
    void normalSize( );
    void scaleImage( double factor );

    // Centralise update/repaint
    void updateWidget( );

    // All the mouse events have the same functionality in this widget
    void onMouseEvent( QMouseEvent* event );

    void drawGrid( QPainter* painter );
};

#endif  // CANVASWIDGET_H
