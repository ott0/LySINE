#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>

class ClientThread : public QThread
{
    Q_OBJECT

public:
    ClientThread(int socketDescriptor, QObject *parent);

    void run();


private slots:
    void readClient();
    void readMysql();
    void writeToFile ( QByteArray in , QByteArray label );

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QTcpSocket *mysqlSocket;
    QTcpSocket *clientSocket;
};

#endif
