#include "connection.h"

Connection::Connection()
{
;
}


void Connection::Connect()
{
    socket = new QTcpSocket();
    socket->abort();
    socket->connectToHost("192.168.0.7", 9092);
    qDebug() << "Tentative de connection" ;

    connect(socket, SIGNAL(connected()), this, SLOT(Connecte()));
    connect(socket, SIGNAL(disconnected()),this,SLOT(Deconnecte()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(Reception()));
}

Connection::~Connection()
{

}

void Connection::Connecte()
{
    qDebug() << "Connexion réussi";
}

void Connection::Deconnecte()
{
    qDebug() << "Deconnecté";
}

void Connection::Reception()
{
    socket->read(Buffer,255);
    emit DonneeRecu(255,Buffer);
}

void Connection::Send(QString &message)
{
//    QByteArray paquet;
//    QDataStream out(&paquet, QIODevice::WriteOnly);
//    out << message;
    socket->write(message.toStdString().c_str());
}
