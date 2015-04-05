#include "connection.h"

Connection::Connection()
{
;
}

void Connection::Connect()
{
    socket = new QTcpSocket();
    socket->abort();
    socket->connectToHost("192.168.1.58", 9091);
    qDebug() << "Tentative de connection" ;

    Serveur_r.size = 0;

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
    if(Serveur_r.size == 0)
    {
        if(socket->bytesAvailable() < 2)
            return;

        socket->read(&Serveur_r.size,1);
        socket->read(&Serveur_r.ms_type,1);

        qDebug() << "Message de taille " << (int)Serveur_r.size << " en attente";
        qDebug() << "De type " << (int)Serveur_r.ms_type;

    }

    if(Serveur_r.size != 0)
    {
        if(socket->bytesAvailable() < Serveur_r.size)
            return;

        Buffer.clear();
        Buffer.resize(Serveur_r.size);
        Buffer = socket->read(Serveur_r.size);
        qDebug() << "Contenu du buffer " << Buffer;
    }


    if (Serveur_r.ms_type == 1)
    {
        QString msg(Buffer);
        Serveur_r.size = 0;
        emit DonneeRecu(msg);
    }
    else if (Serveur_r.ms_type == 2)
    {
        Pos_servo servo;
        /*QDataStream flux(&Buffer,QIODevice::ReadWrite);

        flux >> servo.servo_1 ;
        flux >> servo.servo_2 ;
        flux >> servo.servo_3 ;
        flux >> servo.servo_4 ;*/
        servo.servo_1 = *Buffer;

        Serveur_r.size = 0;
        emit PositionServo(servo);
    }
}

void Connection::Send(Trame_t message)
{
    QByteArray Sender;

    const char* pms_type = &message.ms_type;
    const char* psize = &message.size;

    Sender.append(psize,1);
    Sender.append(pms_type,1);
    Sender.append(message.Payload,message.size);

    socket->write(Sender);
}
