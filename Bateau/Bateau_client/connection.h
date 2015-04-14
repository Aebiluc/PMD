#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtNetwork>

typedef struct{
    char size; //Taille du message (Sans l'entete)
    char ms_type;
    const char* Payload;
}Trame_t;

typedef struct{
    int servo_1;
    int servo_2;
    int servo_3;
    int servo_4;
}Pos_servo;


class Connection : public QObject
{
Q_OBJECT

public:
    Connection();

    int Connect(QString adresse_ip, QString port);
    void Send(Trame_t message);
    ~Connection();

private slots:
    void Connecte();
    void Deconnecte();
    void Reception();

signals:
    void DonneeRecu(QString Paquet);
    void PositionServo(Pos_servo servo);
    void Deconnect();

private:
    QTcpSocket *socket;
    QByteArray  Buffer;
    Trame_t Serveur_r;
};

#endif // CONNECTION_H
