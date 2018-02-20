
#include "Sprite.h"
#include <QFile>
#include <QPainter>

Sprite::Sprite( )
    : _spriteImage( NULL )
    , _fileName( "" )
    , _pos( 0, 0 )
    , _currentFrame( 0 )
    , _nRow( 0 )
    , _nCol( 0 )
{
    clear( );
}

Sprite::Sprite( const QString& fileName )
    : _fileName( fileName )
    , _pos( 0, 0 )
    , _currentFrame( 0 )
    , _spriteImage( new QImage( _fileName ) )
    , _nRow( 0 )
    , _nCol( 0 )
{
    updateRowAndCol( );
}

void
Sprite::clear( )
{
    if ( _spriteImage )
    {
        delete _spriteImage;
        _spriteImage = 0;
    }
    _spriteImage = new QImage( FRAME_SIZE * 7, FRAME_SIZE * 1, QImage::Format_RGBA8888 );
    _spriteImage->fill( QColor( Qt::white ).rgb( ) );
    updateRowAndCol( );
}

Sprite::~Sprite( )
{
    if ( _spriteImage )
    {
        delete _spriteImage;
        _spriteImage = 0;
    }
}

void
Sprite::load( const QString& fileName )
{
    if ( _spriteImage )
    {
        delete _spriteImage;
        _spriteImage = NULL;
    }

    _fileName = fileName;
    _spriteImage = new QImage( fileName );
    // Restriction just read the first row
    _spriteImage = new QImage( _spriteImage->copy( QRect( 0, 0, _spriteImage->width( ), 64 ) ) );
    updateRowAndCol( );
}

void
Sprite::save( const QString& fileName )
{
    if ( _spriteImage )
    {
        QFile file( fileName );
        file.open( QIODevice::WriteOnly );
        _spriteImage->save( &file );
    }
}

void
Sprite::draw( QPainter* painter )
{
    if ( _spriteImage != NULL )
    {
        int centerPainterX = ( painter->device( )->width( ) / 2 ) - FRAME_SIZE / 2;
        int centerPainterY = ( painter->device( )->height( ) / 2 ) - FRAME_SIZE / 2;
        painter->drawImage( centerPainterX, centerPainterY - 10, *_spriteImage,
                            _currentFrame * FRAME_SIZE, 0, FRAME_SIZE, FRAME_SIZE );
    }
}

void
Sprite::nextFrame( )
{
    // following variable keeps track which frame to show from sprite sheet. Simplification just 1
    // row in the sprite sheet
    _currentFrame++;
    if ( _currentFrame >= _nCol )
        _currentFrame = 0;
}

void
Sprite::reset( )
{
    _currentFrame = 0;
    clear( );
}

QImage*
Sprite::toImage( ) const
{
    return _spriteImage;
}

void
Sprite::updateRowAndCol( )
{
    int spritesheetW = _spriteImage->width( );
    int spritesheetH = _spriteImage->height( );
    _nCol = spritesheetW / FRAME_SIZE;
    _nRow = spritesheetH / FRAME_SIZE;
}
