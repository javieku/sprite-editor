#include "ImageLoader.h"

#include <QFile>

ImageLoader* ImageLoader::_instance = 0;

ImageLoader::ImageLoader( )
{
    _spriteImage = SpriteSharedPointer( new Sprite( ) );
}

ImageLoader::~ImageLoader( )
{
    if ( _instance )
    {
        delete _instance;
    }
}

ImageLoader&
ImageLoader::getInstance( )
{
    static ImageLoader instance;
    return instance;
}

void
ImageLoader::load( const QString& fileName )
{
    _spriteImage->load( fileName );
    Q_EMIT spriteLoaded( );
}

void
ImageLoader::save( const QString& fileName )
{
    _spriteImage->save( fileName );
    Q_EMIT spriteSaved( );
}

void
ImageLoader::loadBlankImage( )
{
    _spriteImage->reset( );
    Q_EMIT spriteLoaded( );
}

SpriteSharedPointer
ImageLoader::getSpriteSheet( ) const
{
    return _spriteImage;
}
