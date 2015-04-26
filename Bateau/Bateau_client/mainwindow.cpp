#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labelPalette = QApplication::palette();
    labelPalette.setColor(QPalette::WindowText,Qt::red);
    ui->ConnectionStatusLabel->setPalette(labelPalette);
    Pi = new Connection();

    ui->label_servo_1->setText("0");
    ui->label_servo_2->setText("0");
    ui->label_servo_3->setText("0");
    ui->label_servo_4->setText("0");

    connect(ui->BtConnexion, SIGNAL(clicked()), this, SLOT(Connect()));

    connect(ui->Scroll_servo_1, SIGNAL(valueChanged(int)) ,this, SLOT(Scroll1(int)));
    connect(ui->Scroll_servo_2, SIGNAL(valueChanged(int)) ,this, SLOT(Scroll2(int)));
    connect(ui->Scroll_servo_3, SIGNAL(valueChanged(int)) ,this, SLOT(Scroll3(int)));
    connect(ui->Scroll_servo_4, SIGNAL(valueChanged(int)) ,this, SLOT(Scroll4(int)));
    connect(Pi, SIGNAL(PositionServo(Pos_servo)), this, SLOT(PositionServo(Pos_servo)));
/*
    connect(ui->Bt_send, SIGNAL(clicked()), this, SLOT(EnvoyerMessage()));
    connect(Pi, SIGNAL(DonneeRecu(QString)), this, SLOT(MessageRecu(QString)));
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Connect()
{
    int ConnectionStatus;
    ConnectionStatus = Pi->Connect(ui->adresseIPLineEdit->text(), ui->portLineEdit->text());
    if (ConnectionStatus == 0)
    {
        ui->adresseIPLineEdit->setEnabled(false);
        ui->portLineEdit->setEnabled(false);
        ui->BtConnexion->setEnabled(false);
        ui->ConnectionStatusLabel->setText("Connecté");

        labelPalette.setColor(QPalette::WindowText,Qt::green);
        ui->ConnectionStatusLabel->setPalette(labelPalette);
        connect(Pi, SIGNAL(Deconnect()), this, SLOT(Deconnection()));

        return;
    }

    QMessageBox::warning(this,"Connection","Connection impossible");
}

void MainWindow::Deconnection()
{
    ui->adresseIPLineEdit->setEnabled(true);
    ui->portLineEdit->setEnabled(true);
    ui->BtConnexion->setEnabled(true);
    ui->ConnectionStatusLabel->setText("Deconnecté");
    labelPalette.setColor(QPalette::WindowText,Qt::red);
    ui->ConnectionStatusLabel->setPalette(labelPalette);
}

void MainWindow::EnvoyerMessage()
{
    QString message;
    Trame_t Paquet;
    std::string Conversion;

  //  message = ui->SendText->text();

    Paquet.ms_type = 1;
    Conversion = message.toStdString().c_str();
    Paquet.size = message.length();
    Paquet.Payload = Conversion.c_str();

//    ui->Console->append(message);
    Pi->Send(Paquet);
  //  ui->SendText->clear();

    /*
    int z = 28;
    Paquet.ms_type = 2;
    Paquet.Payload = (const char*)&z;
    Paquet.size = sizeof(z);
    Pi->Send(Paquet);*/

}

void MainWindow::MessageRecu(QString paquet)
{    
 //   ui->Console->append("Message recu : " + paquet);
    //delete paquet.Payload;
}

void MainWindow::PositionServo(Pos_servo servo)
{
    ui->label_servo_1->setText(QString::number(servo.servo_1));
    ui->label_servo_2->setText(QString::number(servo.servo_2));
    ui->label_servo_3->setText(QString::number(servo.servo_3));
    ui->label_servo_4->setText(QString::number(servo.servo_4));
}

void MainWindow::Scroll1(int value)
{
    SendManuelPosition(value,1);
}
void MainWindow::Scroll2(int value)
{
    SendManuelPosition(value,2);
}
void MainWindow::Scroll3(int value)
{
    SendManuelPosition(value,3);
}
void MainWindow::Scroll4(int value)
{
    SendManuelPosition(value,4);
}

void MainWindow::SendManuelPosition(int value, int servo_ID)
{
    Trame_t Paquet;
    QByteArray paquet;
    QDataStream stream(&paquet,QIODevice::ReadWrite);
    stream.setByteOrder(QDataStream::LittleEndian);

    Paquet.ms_type = 3;
    stream << (char)value << (char)servo_ID;

    Paquet.size = paquet.size();
    Paquet.Payload = paquet.data();


    Pi->Send(Paquet);

}
