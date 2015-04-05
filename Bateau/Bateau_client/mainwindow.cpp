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
    connect(Pi, SIGNAL(DonneeRecu(int,const char*)), this, SLOT(MessageRecu(int,const char*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::EnvoyerMessage()
{
    QString message;
    message = ui->SendText->text();
    Pi->Send(message);
    ui->SendText->clear();
}

void MainWindow::MessageRecu(int size, const char* Buffer)
{
    const char* buffer = new char[size];
    buffer = Buffer;
    String retour(Buffer);
    ui->Console->append("Message recu : " +retour );
}

