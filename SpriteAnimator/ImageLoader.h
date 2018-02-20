#pragma once

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include "Sprite.h"

/**
 *	This class receives from the GUI new, load and save files events and uses a SpriteSharedPointer
 *to do the proper operation. Notice this pointer is shared by all the widgets in the application:
 *CanvasWidget, SpritePreviewWidget
 */

class ImageLoader : public QObject
{
    Q_OBJECT
public:
    static ImageLoader& getInstance( );

    SpriteSharedPointer getSpriteSheet( ) const;

signals:
    void spriteLoaded( );
    void spriteSaved( );

public slots:
    void load( const QString& fileName );
    void save( const QString& fileName );
    void loadBlankImage( );

private:
    static ImageLoader* _instance;

    SpriteSharedPointer _spriteImage;

    ImageLoader( void );
    ~ImageLoader( void );
    ImageLoader( ImageLoader const& );
    void operator=( ImageLoader const& );
};

#endif  // IMAGELOADER_H
