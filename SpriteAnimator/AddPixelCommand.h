#pragma once

#include <QColor>
#include <QtWidgets>

class CanvasWidget;

/**
 *	QUndoCommand subclass to store pixel operation on a QUndoStack.
 *	Simply stores position, previous and applied colors to perform do and undo operations.
 *
 *	Instead of saving individually position or color it should store a class that represents the
 *full status of the widget. To facilitate adding new elements in the future.
 *
 */

class AddPixelCommand : public QUndoCommand
{
public:
    AddPixelCommand( CanvasWidget* canvasWidget,
                     QPoint position,
                     QColor prev,
                     QColor current,
                     int penWidth,
                     QUndoCommand* parent = 0 );
    ~AddPixelCommand( );

    void undo( ) Q_DECL_OVERRIDE;
    void redo( ) Q_DECL_OVERRIDE;

private:
    CanvasWidget* _canvasWidget;
    QPoint _pos;
    QColor _prevColor;
    QColor _currentColor;
    int _penWidth;
};
