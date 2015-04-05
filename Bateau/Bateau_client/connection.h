#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtNetwork>


class Connection : public QObject
{
Q_OBJECT

public:
    Connection();

    void Connect();
    void Send(QString &message);
    ~Connection();

private slots:
    void Connecte();
    void Deconnecte();
    void Reception();

signals:
    void DonneeRecu(int size, const char*);

private:
    QTcpSocket *socket;
    char Buffer[255];

};

#endif // CONNECTION_H
