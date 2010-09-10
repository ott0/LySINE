#include <QtCore/QCoreApplication>
#include <clientserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ClientServer *clientServer = new ClientServer();
    clientServer->listen(QHostAddress::Any, 8888);

    return a.exec();
}
