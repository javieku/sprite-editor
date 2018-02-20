#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow( );

protected:
    void closeEvent( QCloseEvent* );

public slots:
    void colorPickTriggered( );

private:
    MainWindowPrivate* d;
};

#endif  // MAINWINDOW_H
