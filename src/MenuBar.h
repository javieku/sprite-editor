#pragma once

#ifndef MENUBAR_H
#define MENUBAR_H

#include <QImage>
#include <QtWidgets>
#include "mainwindow.h"

class ImageLoader;

/**
 *	This class manages the menu bar components on the main window.
 *
 *   + Notify load and save files using a ImageLoader class.
 *
 *	+ Redo and undo commands using a QUndoStack object. This functionality may be moved to another
 *class more related with the commands that are being stored.
 *
 *   Menu structure:
 *	+ File: New, Load, Save, Save As and Exit
 *   + Edit: Undo, redo
 *   + Help: About, About Qt
 */

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    MenuBar( MainWindow* owner );
    ~MenuBar( );

signals:
    void loadImage( const QString& imageName );
    void saveImage( const QString& imageName );
    void loadBlankImage( );

public slots:
    void pixelDraw( QPoint pos, QColor prev, QColor current, int penWidth );

protected:
    void closeEvent( QCloseEvent* event ) Q_DECL_OVERRIDE;

private slots:
    void newCanvas( );
    void open( );
    bool save( );
    bool saveAs( );
    void about( );
    void canvasWasModified( );

private:
    void createActions( );
    void createMenus( );
    void createStatusBar( );
    bool maybeSave( );
    void loadFile( const QString& fileName );
    bool saveFile( const QString& fileName );
    void setCurrentFile( const QString& fileName );
    QString strippedName( const QString& fullFileName );

    void createUndoView( );

    MainWindow* _owner;

    QString curFile;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;
    QAction* aboutAct;
    QAction* aboutQtAct;

    QUndoStack* _undoStack;
    QUndoView* _undoView;
    QAction* undoAction;
    QAction* redoAction;

    ImageLoader* _imageLoader;

    bool _modified;
};

#endif  // MENUBAR_H