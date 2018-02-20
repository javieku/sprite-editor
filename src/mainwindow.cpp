#include "mainwindow.h"
#include <QColorDialog>
#include <QLabel>
#include <QLayout>
#include <QStatusBar>
#include <QToolBar>
#include "AddPixelCommand.h"
#include "CanvasWidget.h"
#include "MenuBar.h"
#include "SpritePreviewWidget.h"

class MainWindowPrivate
{
public:
    MainWindowPrivate( MainWindow* owner );
    ~MainWindowPrivate( );

    void pickColor( );

    void createWindowComponents( );

private:
    CanvasWidget* _canvasWidget;
    SpritePreviewWidget* _spritePreviewWidget;
    MenuBar* _menuBar;
    MainWindow* _owner;
    QSlider* _slider;
    QToolBar* _mainToolBar;
    QStatusBar* _statusBar;
    QWidget* _centralWidget;
    QAction* _actionColorPick;
};

MainWindowPrivate::MainWindowPrivate( MainWindow* owner )
    : _menuBar( new MenuBar( owner ) )
    , _owner( owner )
    , _spritePreviewWidget( new SpritePreviewWidget( ) )
    , _canvasWidget( new CanvasWidget( owner ) )
{
    QRect screenGeometry = QApplication::desktop( )->screenGeometry( );

    // Initialise main window components
    createWindowComponents( );

    // Setup main window widgets
    _centralWidget->setLayout( new QVBoxLayout( _owner ) );
    _centralWidget->layout( )->addWidget( _canvasWidget );
    _centralWidget->setBackgroundRole( QPalette::Dark );

    // Setup main window
    _owner->setWindowIcon( QIcon( "indi.bmp" ) );
    _owner->layout( )->setMenuBar( _menuBar );
    _owner->setWindowTitle( "Animator" );
    _owner->setGeometry( QRect( 0, 0, screenGeometry.width( ), 512 ) );
    _owner->setFixedSize( QSize( screenGeometry.width( ), 512 ) );

    // Move the main window to the center of the screen
    int centerX = ( screenGeometry.width( ) - _owner->width( ) ) / 2;
    int centerY = ( screenGeometry.height( ) - _owner->height( ) ) / 2;
    _owner->move( centerX, centerY );

    // Sprite preview window
    _spritePreviewWidget->move( centerX + ( _owner->width( ) / 2 ),
                                centerY + _owner->height( ) + 20 );
    _spritePreviewWidget->show( );

    // QUndostack is defined in menu bar and needs to know when a pixel was drawn
    QObject::connect( _canvasWidget, SIGNAL( pixelDraw( QPoint, QColor, QColor, int ) ), _menuBar,
                      SLOT( pixelDraw( QPoint, QColor, QColor, int ) ) );
}

void
MainWindowPrivate::createWindowComponents( )
{
    // Setup central widget
    _centralWidget = new QWidget( _owner );
    _owner->setCentralWidget( _centralWidget );

    // Setup pick color action from the toolbar
    _actionColorPick = new QAction( _owner );
    _actionColorPick->setText( QApplication::translate( "MainWindow", "Color Pick", 0 ) );
    _actionColorPick->setToolTip( QApplication::translate( "MainWindow", "Choose a color", 0 ) );
    QObject::connect( _actionColorPick, SIGNAL( triggered( ) ), _owner,
                      SLOT( colorPickTriggered( ) ) );

    _slider = new QSlider( Qt::Horizontal );
    _slider->setTickInterval( 1 );
    _slider->setMaximumWidth( 400 );
    _slider->setTickPosition( QSlider::TicksAbove );
    _slider->setSingleStep( 0.5 );
    _slider->setMinimum( 1 );
    _slider->setMaximum( 5 );
    QObject::connect( _slider, SIGNAL( valueChanged( int ) ), _canvasWidget,
                      SLOT( setPenWidth( int ) ) );

    // Setup toolbar
    _mainToolBar = new QToolBar( _owner );
    _mainToolBar->setLayout( new QVBoxLayout( _owner ) );
    _mainToolBar->addAction( _actionColorPick );
    _mainToolBar->addSeparator( );
    _mainToolBar->addWidget( _slider );
    _owner->addToolBar( Qt::TopToolBarArea, _mainToolBar );

    // Setup statusbar
    _statusBar = new QStatusBar( _owner );
    _owner->setStatusBar( _statusBar );
}

void
MainWindowPrivate::pickColor( )
{
    QColor color = QColorDialog::getColor( _canvasWidget->drawColor( ), _owner,
                                           QString( "Select a draw color" ),
                                           QColorDialog::ShowAlphaChannel );
    _canvasWidget->setDrawColor( color );
}

MainWindowPrivate::~MainWindowPrivate( )
{
    delete _spritePreviewWidget;
    delete _menuBar;
    delete _canvasWidget;
    delete _mainToolBar;
    delete _statusBar;
    delete _centralWidget;
}

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , d( new MainWindowPrivate( this ) )
{
}

MainWindow::~MainWindow( )
{
    delete d;
}

void
MainWindow::colorPickTriggered( )
{
    d->pickColor( );
}

void
MainWindow::closeEvent( QCloseEvent* event )
{
    qApp->quit( );
}