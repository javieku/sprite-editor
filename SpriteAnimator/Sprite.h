#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include <QPixMap>
#include <QString>
#include "SharedDataPointer.h"

const int FRAME_SIZE = 64;

/**
 *	This class represents a spritesheet. It can be drawn frame by frame reset to an empty
 *	representation to be painted. It can also save and load image from a supported format. It's
 *	one of the few classes that it's used as SharedPointer instead of a raw pointer.
 *
 *   Sadly it has some limitations in the current implementation:
 *   It supports just 1 row in the sprite sheet.
 *
 *	Ideally we could read a JSON or any other file representation that would give us the meaning of
 *the frames in the sprite sheet and with a sprite factory class we could generate different sprite
 *objects from several animations on the same spritesheet.
 *
 **/

class Sprite : public QSharedData
{
public:
    Sprite( );
    Sprite( const QString& fileName );

    ~Sprite( );

    void draw( QPainter* painter );
    void nextFrame( );
    void reset( );

    QImage* toImage( ) const;

    void load( const QString& fileName );
    void save( const QString& fileName );

private:
    QString _fileName;
    QImage* _spriteImage;

    QPoint _pos;
    int _currentFrame;

    int _nRow;
    int _nCol;

    void updateRowAndCol( );
    void clear( );
};

SHARED_DATA_POINTER_DECLARE_TYPES( Sprite )

#endif  // SPRITE_H