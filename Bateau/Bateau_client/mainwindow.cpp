#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->Console->setText("Bonjour");
//    ui->Console->append("Salut");

    Pi = new Connection();
    Pi->Connect();

    connect(ui->Bt_send, SIGNAL(clicked()), this, SLOT(EnvoyerMessage()));
    connect(Pi, SIGNAL(DonneeRecu(QString)), this, SLOT(MessageRecu(QString)));
    connect(Pi, SIGNAL(PositionServo(Pos_servo)), this, SLOT(PositionServo(Pos_servo)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::EnvoyerMessage()
{
    QString message;
    Trame_t Paquet;
    std::string Conversion;

    message = ui->SendText->text();

    Paquet.ms_type = 1;
    Conversion = message.toStdString().c_str();
    Paquet.size = message.length();
    Paquet.Payload = Conversion.c_str();

    ui->Console->append(message);
    Pi->Send(Paquet);
    ui->SendText->clear();

    /*
    int z = 28;
    Paquet.ms_type = 2;
    Paquet.Payload = (const char*)&z;
    Paquet.size = sizeof(z);
    Pi->Send(Paquet);*/

}

void MainWindow::MessageRecu(QString paquet)
{    
    ui->Console->append("Message recu : " + paquet);
    //delete paquet.Payload;
}

void MainWindow::PositionServo(Pos_servo servo)
{
    QString Afficher;
    Afficher = QString::number(servo.servo_1);

    ui->Console->append(Afficher);
}
