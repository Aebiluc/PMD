#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPalette>
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
    void Connect();
    void Deconnection();
    void Scroll1(int value);
    void Scroll2(int value);
    void Scroll3(int value);
    void Scroll4(int value);

private:  
    void SendManuelPosition(int value, int servo_ID);

    Ui::MainWindow *ui;
    Connection *Pi;
    QPalette labelPalette;

};

#endif // MAINWINDOW_H
