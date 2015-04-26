#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void EnvoyerMessage();
    void MessageRecu(QString paquet);
    void PositionServo(Pos_servo);

private:  
    Ui::MainWindow *ui;
    Connection *Pi;

};

#endif // MAINWINDOW_H
