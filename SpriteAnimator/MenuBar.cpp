#include "MenuBar.h"
#include "AddPixelCommand.h"
#include "CanvasWidget.h"
#include "ImageLoader.h"
#include "mainwindow.h"

MenuBar::MenuBar( MainWindow* owner )
    : QMenuBar( )
    , _owner( owner )
    , _modified( false )
{
    createActions( );
    createMenus( );
    createStatusBar( );
    createUndoView( );

    setCurrentFile( "" );

    _imageLoader = &( ImageLoader::getInstance( ) );

    // Notify to image loader when and what image to load
    QObject::connect( this, SIGNAL( loadImage( QString ) ), _imageLoader, SLOT( load( QString ) ) );

    // Notify to image loader when and what image to save
    QObject::connect( this, SIGNAL( saveImage( QString ) ), _imageLoader, SLOT( save( QString ) ) );

    // Notify to image loader to unload the current image so the canvas will become empty
    QObject::connect( this, SIGNAL( loadBlankImage( ) ), _imageLoader, SLOT( loadBlankImage( ) ) );
}

MenuBar::~MenuBar( void )
{
}

void
MenuBar::closeEvent( QCloseEvent* event )
{
    if ( maybeSave( ) )
    {
        event->accept( );
    }
    else
    {
        event->ignore( );
    }
}

void
MenuBar::newCanvas( )
{
    if ( maybeSave( ) )
    {
        _modified = false;
        setCurrentFile( "" );
        _undoStack->clear( );
        Q_EMIT loadBlankImage( );
    }
}

void
MenuBar::open( )
{
    if ( maybeSave( ) )
    {
        _modified = false;
        _undoStack->clear( );
        setCurrentFile( "" );

        QFileDialog dialog( this );
        dialog.setWindowModality( Qt::WindowModal );
        dialog.setAcceptMode( QFileDialog::AcceptSave );

        QStringList typeFilters;
        foreach ( const QByteArray& mimeTypeName, QImageWriter::supportedMimeTypes( ) )
        {
            typeFilters.append( mimeTypeName );
        }
        typeFilters.sort( );

        dialog.setAcceptMode( QFileDialog::AcceptOpen );
        dialog.setMimeTypeFilters( typeFilters );
        dialog.selectMimeTypeFilter( "image/png" );

        QStringList files;
        if ( dialog.exec( ) )
            files = dialog.selectedFiles( );

        if ( !files.isEmpty( ) )
            loadFile( files.at( 0 ) );
    }
}

bool
MenuBar::save( )
{
    if ( curFile.isEmpty( ) )
    {
        return saveAs( );
    }
    else
    {
        return saveFile( curFile );
    }
}

bool
MenuBar::saveAs( )
{
    QFileDialog dialog( this );
    dialog.setWindowModality( Qt::WindowModal );
    dialog.setAcceptMode( QFileDialog::AcceptSave );

    QStringList typeFilters;
    foreach ( const QByteArray& mimeTypeName, QImageWriter::supportedMimeTypes( ) )
    {
        typeFilters.append( mimeTypeName );
    }

    typeFilters.sort( );
    dialog.setAcceptMode( QFileDialog::AcceptOpen );
    dialog.setMimeTypeFilters( typeFilters );
    dialog.selectMimeTypeFilter( "image/png" );

    QStringList files;
    if ( dialog.exec( ) )
        files = dialog.selectedFiles( );
    else
        return false;

    return saveFile( files.at( 0 ) );
}

void
MenuBar::about( )
{
    QMessageBox::about(
        this, tr( "About Animator" ),
        tr( "Simple application to edit sprite sheets\n Controls: \n+Right click to draw a pixel "
            "\n+Left click to delete a pixel\n+Mouse wheel to zoom on the painting area\n You can "
            "select the color and the width of the pixel. You can also load files and save them on "
            "different formats." ) );
}

void
MenuBar::canvasWasModified( )
{
    setWindowModified( _modified );
}

void
MenuBar::createActions( )
{
    newAct = new QAction( QIcon( ":/images/new.png" ), tr( "&New" ), this );
    newAct->setShortcuts( QKeySequence::New );
    newAct->setStatusTip( tr( "Create a new file" ) );
    connect( newAct, SIGNAL( triggered( ) ), this, SLOT( newCanvas( ) ) );

    openAct = new QAction( QIcon( ":/images/open.png" ), tr( "&Open..." ), this );
    openAct->setShortcuts( QKeySequence::Open );
    openAct->setStatusTip( tr( "Open an existing file" ) );
    connect( openAct, SIGNAL( triggered( ) ), this, SLOT( open( ) ) );

    saveAct = new QAction( QIcon( ":/images/save.png" ), tr( "&Save" ), this );
    saveAct->setShortcuts( QKeySequence::Save );
    saveAct->setStatusTip( tr( "Save the sprite to disk" ) );
    connect( saveAct, SIGNAL( triggered( ) ), this, SLOT( save( ) ) );

    saveAsAct = new QAction( tr( "Save &As..." ), this );
    saveAsAct->setShortcuts( QKeySequence::SaveAs );
    saveAsAct->setStatusTip( tr( "Save the sprite under a new name" ) );
    connect( saveAsAct, SIGNAL( triggered( ) ), this, SLOT( saveAs( ) ) );

    exitAct = new QAction( tr( "E&xit" ), this );
    exitAct->setShortcuts( QKeySequence::Quit );
    exitAct->setStatusTip( tr( "Exit Animator" ) );
    connect( exitAct, SIGNAL( triggered( ) ), _owner, SLOT( close( ) ) );

    aboutAct = new QAction( tr( "&About" ), this );
    aboutAct->setStatusTip( tr( "Show the Animator's About box" ) );
    connect( aboutAct, SIGNAL( triggered( ) ), this, SLOT( about( ) ) );

    aboutQtAct = new QAction( tr( "About &Qt" ), this );
    aboutQtAct->setStatusTip( tr( "Show the Qt library's About box" ) );
    connect( aboutQtAct, SIGNAL( triggered( ) ), qApp, SLOT( aboutQt( ) ) );

    _undoStack = new QUndoStack( _owner );

    undoAction = _undoStack->createUndoAction( _owner, tr( "&Undo" ) );
    saveAct->setStatusTip( tr( "Undo the last operation" ) );
    undoAction->setShortcuts( QKeySequence::Undo );

    redoAction = _undoStack->createRedoAction( _owner, tr( "&Redo" ) );
    saveAct->setStatusTip( tr( "Redo the last operation" ) );
    redoAction->setShortcuts( QKeySequence::Redo );
}

void
MenuBar::createMenus( )
{
    fileMenu = this->addMenu( tr( "&File" ) );
    fileMenu->addAction( newAct );
    fileMenu->addAction( openAct );
    fileMenu->addAction( saveAct );
    fileMenu->addAction( saveAsAct );
    fileMenu->addSeparator( );
    fileMenu->addAction( exitAct );

    editMenu = this->addMenu( tr( "&Edit" ) );
    editMenu->addAction( undoAction );
    editMenu->addAction( redoAction );
    this->addSeparator( );

    helpMenu = this->addMenu( tr( "&Help" ) );
    helpMenu->addAction( aboutAct );
    helpMenu->addAction( aboutQtAct );
}

void
MenuBar::createStatusBar( )
{
    _owner->statusBar( )->showMessage( tr( "Ready" ) );
}

bool
MenuBar::maybeSave( )
{
    if ( _modified )
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(
            this, tr( "Animator" ),
            tr( "The sprite has been modified.\n"
                "Do you want to save your changes?" ),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        if ( ret == QMessageBox::Save )
            return save( );
        else if ( ret == QMessageBox::Cancel )
            return false;
    }
    return true;
}

void
MenuBar::loadFile( const QString& fileName )
{
    QImageReader reader( fileName );

    // Not allow to load not supported files
    if ( !reader.canRead( ) )
    {
        QMessageBox::warning(
            this, tr( "Animator" ),
            tr( "Cannot read image %1:\n%2." ).arg( fileName ).arg( reader.errorString( ) ) );
        return;
    }

    // Not allow to load spritesheet whose frame size is not supported
    QSize size = reader.size( );
    if ( ( size.width( ) % FRAME_SIZE ) != 0 )
    {
        QMessageBox::warning( this, tr( "Animator" ),
                              tr( "Image resolution is not valid %1:\n%2." )
                                  .arg( fileName )
                                  .arg( reader.errorString( ) ) );
        return;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor( Qt::WaitCursor );
#endif
    Q_EMIT loadImage( fileName );
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor( );
#endif

    setCurrentFile( fileName );
    _owner->statusBar( )->showMessage( tr( "File loaded" ), 2000 );
}

bool
MenuBar::saveFile( const QString& fileName )
{
    QFile file( fileName );
    if ( !file.open( QFile::WriteOnly | QFile::Text ) )
    {
        QMessageBox::warning(
            this, tr( "Animator" ),
            tr( "Cannot write file %1:\n%2." ).arg( fileName ).arg( file.errorString( ) ) );
        return false;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor( Qt::WaitCursor );
#endif
    Q_EMIT saveImage( fileName );
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor( );
#endif

    setCurrentFile( fileName );
    _owner->statusBar( )->showMessage( tr( "File saved" ), 2000 );
    _modified = false;
    return true;
}

void
MenuBar::setCurrentFile( const QString& fileName )
{
    curFile = fileName;
    setWindowModified( false );

    QString shownName = curFile;
    if ( curFile.isEmpty( ) )
        shownName = "untitled.png";
    _owner->setWindowFilePath( shownName );
}

QString
MenuBar::strippedName( const QString& fullFileName )
{
    return QFileInfo( fullFileName ).fileName( );
}

void
MenuBar::createUndoView( )
{
    _undoView = new QUndoView( _undoStack );
    _undoView->setWindowTitle( tr( "Command List" ) );
    _undoView->show( );
    _undoView->setAttribute( Qt::WA_QuitOnClose, false );
}

void
MenuBar::pixelDraw( QPoint pos, QColor prevColor, QColor currentColor, int penWidth )
{
    QObject* obj = sender( );
    CanvasWidget* canvasWidget = dynamic_cast< CanvasWidget* >( sender( ) );
    if ( canvasWidget )
    {
        QUndoCommand* addCommand
            = new AddPixelCommand( canvasWidget, pos, prevColor, currentColor, penWidth );
        _undoStack->push( addCommand );
        _modified = true;
    }
}
