#include "AddPixelCommand.h"
#include <QPoint>
#include "CanvasWidget.h"

AddPixelCommand::AddPixelCommand( CanvasWidget* canvasWidget,
                                  QPoint position,
                                  QColor prev,
                                  QColor current,
                                  int penWidth,
                                  QUndoCommand* parent )
    : QUndoCommand( parent )
    , _canvasWidget( canvasWidget )
    , _prevColor( prev )
    , _currentColor( current )
    , _penWidth( penWidth )
    , _pos( position )
{
    static int itemCount = 0;
    ++itemCount;
    setText( QObject::tr( "[%1]->[%2] Draw (%3, %4)" )
                 .arg( _prevColor.name( ) )
                 .arg( _currentColor.name( ) )
                 .arg( QString::number( position.x( ) ) )
                 .arg( QString::number( position.y( ) ) ) );
}

AddPixelCommand::~AddPixelCommand( )
{
}

void
AddPixelCommand::undo( )
{
    // penwidth undo must be reviewed
    _canvasWidget->drawPixel( _pos, _prevColor, _penWidth );
}

void
AddPixelCommand::redo( )
{
    _canvasWidget->drawPixel( _pos, _currentColor, _penWidth );
}
