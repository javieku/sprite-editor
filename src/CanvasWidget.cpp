
#include "CanvasWidget.h"
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRgb>
#include <QScrollArea>
#include <QScrollbar>
#include "ImageLoader.h"

CanvasWidget::CanvasWidget( QWidget* parent )
    : QWidget( parent )
    , _drawColor( QColor( Qt::black ) )
    , _sprite( SpriteSharedPointer( ) )
    , _nFrame( 0 )
    , _scale( 3.75 )
    , _penWidth( 1 )
    , _canvas( 0 )
{
    // Listen to a possible sprite load event
    _imageLoader = &( ImageLoader::getInstance( ) );
    QObject::connect( _imageLoader, SIGNAL( spriteLoaded( ) ), this, SLOT( spriteLoaded( ) ) );
    // Load blank canvas
    spriteLoaded( );
}

CanvasWidget::~CanvasWidget( )
{
    _sprite = NULL;
}

bool
CanvasWidget::drawPixel( QPoint pos, QColor color, int penWidth )
{
    bool ret = false;
    if ( _canvas )
    {
        QPainter painter( _canvas );
        painter.setPen( QPen( color, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
        painter.setRenderHint( QPainter::Antialiasing );
        int x = pos.x( ) / _scale;
        int y = pos.y( ) / _scale;
        int canvasWidth = _canvas->width( );
        int canvasHeight = _canvas->height( );
        if ( x >= 0 && y <= canvasWidth && y >= 0 && y <= canvasHeight )
        {
            painter.drawPoint( x, y );
            ret = true;
            updateWidget( );
        }
    }
    return ret;
}

bool
CanvasWidget::deletePixel( QPoint pos, int penWidth )
{
    return drawPixel( pos, QColor( Qt::white ), penWidth );
}

void
CanvasWidget::mousePressEvent( QMouseEvent* event )
{
    onMouseEvent( event );
}

void
CanvasWidget::mouseMoveEvent( QMouseEvent* event )
{
    onMouseEvent( event );
}

void
CanvasWidget::onMouseEvent( QMouseEvent* event )
{
    if ( event->buttons( ) & Qt::LeftButton )
    {
        if ( _canvas )
        {
            QPoint pos = event->pos( );
            int x = pos.x( ) / _scale;
            int y = pos.y( ) / _scale;
            QColor prevColor = QColor::fromRgb( _canvas->pixel( x, y ) );
            bool success = drawPixel( pos, _drawColor, _penWidth );
            if ( success )
            {
                Q_EMIT pixelDraw( pos, prevColor, _drawColor, _penWidth );
            }
        }
    }
    else if ( event->buttons( ) & Qt::RightButton )
    {
        deletePixel( event->pos( ), _penWidth );
    }
}

void
CanvasWidget::resizeEvent( QResizeEvent* event )
{
    // TODO
}

QColor
CanvasWidget::drawColor( )
{
    return _drawColor;
}

void
CanvasWidget::setDrawColor( QColor color )
{
    _drawColor = color;
}

void
CanvasWidget::setPenWidth( int newWidth )
{
    _penWidth = newWidth;
}

void
CanvasWidget::paintEvent( QPaintEvent* event )
{
    if ( _canvas )
    {
        QWidget::paintEvent( event );
        QPainter painter( this );
        painter.setRenderHint( QPainter::Antialiasing );
        painter.scale( _scale, _scale );
        QRect dirtyRect = event->rect( );
        painter.drawImage( dirtyRect, *_canvas, dirtyRect );
        drawGrid( &painter );
    }
}

void
CanvasWidget::drawGrid( QPainter* painter )
{
    QPen black( QColor( Qt::black ), 0.25 );
    painter->setPen( black );
    int nLine = 0;
    int w = _canvas->width( );
    int h = _canvas->height( );
    while ( nLine < ( w / FRAME_SIZE ) )
    {
        painter->drawRect( nLine * FRAME_SIZE, 0, h, h );
        nLine++;
    }
}

void
CanvasWidget::wheelEvent( QWheelEvent* event )
{
    if ( event->delta( ) > 0 )
    {
        zoomIn( );
    }
    else
    {
        zoomOut( );
    }
    updateWidget( );
}

void
CanvasWidget::spriteLoaded( )
{
    _sprite = _imageLoader->getSpriteSheet( );
    _canvas = _sprite->toImage( );
    updateWidget( );
}

void
CanvasWidget::zoomIn( )
{
    if ( _scale < 10 )
    {
        scaleImage( 1.25 );
    }
}

void
CanvasWidget::zoomOut( )
{
    if ( _scale > 0.333 )
    {
        scaleImage( 0.8 );
    }
}

void
CanvasWidget::normalSize( )
{
    _scale = 1.0;
    scaleImage( 1 );
}

void
CanvasWidget::scaleImage( double factor )
{
    _scale *= factor;
    resize( _scale * _canvas->size( ) );
}

void
CanvasWidget::updateWidget( )
{
    update( );
}
