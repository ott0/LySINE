#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <QStringList>
#include <QTcpServer>


class ClientServer : public QTcpServer
{
    Q_OBJECT

public:
    ClientServer(QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private:

};


#endif // CLIENTSERVER_H
