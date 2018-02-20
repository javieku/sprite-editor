
#include "SpritePreviewWidget.h"
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QRgb>
#include <QVBoxLayout>
#include "ImageLoader.h"

SpritePreviewWidget::SpritePreviewWidget( QWidget* parent )
    : QWidget( parent )
    , _sprite( SpriteSharedPointer( new Sprite( ) ) )
{
    setBackgroundRole( QPalette::Dark );
    setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    // Setup custom widget
    setFixedSize( FRAME_SIZE * 2, FRAME_SIZE * 2 );
    resize( FRAME_SIZE * 2, FRAME_SIZE * 2 );
    setWindowFlags( Qt::CustomizeWindowHint );

    // Update animation
    connect( &_updateViewTimer, SIGNAL( timeout( ) ), this, SLOT( updateSprite( ) ) );
    _updateViewTimer.setInterval( 250 );

    // Create animation button
    _playbackButton = new QPushButton( "Play", this );
    _playbackButton->setGeometry(
        QRect( QPoint( FRAME_SIZE - FRAME_SIZE / 2, FRAME_SIZE * 2 - FRAME_SIZE / 2 ),
               QSize( FRAME_SIZE, FRAME_SIZE / 2 ) ) );
    connect( _playbackButton, SIGNAL( released( ) ), this, SLOT( handlePlayback( ) ) );

    _imageLoader = &( ImageLoader::getInstance( ) );
    QObject::connect( _imageLoader, SIGNAL( spriteLoaded( ) ), this, SLOT( spriteLoaded( ) ) );
    // Load blank canvas
    spriteLoaded( );
}

SpritePreviewWidget::~SpritePreviewWidget( )
{
    // Shared pointer decrease references
    _sprite = NULL;
    if ( _playbackButton )
    {
        delete _playbackButton;
        _playbackButton = 0;
    }
}

void
SpritePreviewWidget::paintEvent( QPaintEvent* event )
{
    QWidget::paintEvent( event );
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );
    _sprite->draw( &painter );
}

void
SpritePreviewWidget::spriteLoaded( )
{
    _sprite = _imageLoader->getSpriteSheet( );
    repaint( );
}

void
SpritePreviewWidget::play( )
{
    _playbackButton->setText( "Pause" );
    _updateViewTimer.start( );
}

void
SpritePreviewWidget::pause( )
{
    _playbackButton->setText( "Play" );
    _updateViewTimer.stop( );
}

void
SpritePreviewWidget::reset( )
{
    _sprite->reset( );
}

void
SpritePreviewWidget::updateSprite( )
{
    _sprite->nextFrame( );
    repaint( );
}

void
SpritePreviewWidget::handlePlayback( )
{
    if ( !_updateViewTimer.isActive( ) )
    {
        play( );
    }
    else
    {
        pause( );
    }
}
