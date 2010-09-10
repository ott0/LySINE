#include "clientserver.h"
#include "clientthread.h"

#include <stdlib.h>


ClientServer::ClientServer(QObject *parent)
    : QTcpServer(parent)
{

}

void ClientServer::incomingConnection(int socketDescriptor)
{
    ClientThread *thread = new ClientThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
