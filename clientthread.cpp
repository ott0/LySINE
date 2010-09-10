/*
LySINE Data Layer: a public 2-dimensional mysql table
Idea: anyone can add new fields and new records. no one can delete anything.
Allowed Queries: use + show + describe + select + insert
+ alter table lysine add [field_name field_type]
Prohibited Terms: not null
Host: ott0.com Port: 8888 User: root Pass: libre Database: future Table: lysine
Status: currently implemented as a TCP wrapper for mysqld with Qt.
if there are performance issues this will change.
*/

#include "clientthread.h"
#include <QtNetwork>

ClientThread::ClientThread(int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
}

void ClientThread::run()
{

    mysqlSocket = new QTcpSocket ( this );
    mysqlSocket -> connectToHost( "localhost" , 3306 );
    connect ( mysqlSocket , SIGNAL ( readyRead ()), this , SLOT ( readMysql ()));

    clientSocket = new QTcpSocket ( this );
    clientSocket->setSocketDescriptor(socketDescriptor);
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readClient()));

    this->exec();
}


void ClientThread :: readClient()
{
    QByteArray in ;
    in = clientSocket -> readAll ();

    QByteArray s;
    s.resize(4);
    s[0] = 0x0;
    s[1] = 0x0;
    s[2] = 0x0;
    s[3] = 0x3;

    int queryIndex = in.indexOf(s);

    if (queryIndex != -1)
    {
        queryIndex += 4;

        QString inQS = in.mid(queryIndex, -1);

        QRegExp selectRegExp("select", Qt::CaseInsensitive);
        QRegExp alterRegExp("alter table lysine add", Qt::CaseInsensitive);
        QRegExp showRegExp("show", Qt::CaseInsensitive);
        QRegExp useRegExp("use", Qt::CaseInsensitive);
        QRegExp describeRegExp("describe", Qt::CaseInsensitive);
        QRegExp insertRegExp("insert", Qt::CaseInsensitive);

        QRegExp notnullRegExp("not null", Qt::CaseInsensitive);

        if (!(
            ( selectRegExp.indexIn(inQS, 0) == 0 ) ||
            ( alterRegExp.indexIn(inQS, 0) == 0 ) ||
            ( showRegExp.indexIn(inQS, 0) == 0 ) ||
            ( useRegExp.indexIn(inQS, 0) == 0 ) ||
            ( describeRegExp.indexIn(inQS, 0) == 0 ) ||
            ( insertRegExp.indexIn(inQS, 0) == 0 ))
            ||( notnullRegExp.indexIn(inQS, 0) > 0 )){
            in = "asdf" ;  //clean this sup
        }
    }

    mysqlSocket -> write ( in );
    writeToFile ( in , "Request" );

}

void ClientThread :: readMysql()
{
    QByteArray response = mysqlSocket -> readAll ();
    clientSocket -> write ( response );
    writeToFile ( response , "Reply" );
}

void ClientThread :: writeToFile ( QByteArray in , QByteArray label )
{
    QFile file1 ( "db" );
    file1 . open ( QIODevice :: Append | QIODevice :: Text );
    file1 . write ( "\n\n" + label + ": " );

    for (int i = 0; i<in.length(); i++)
    {
        file1 . write ( in.mid(i,1) + in.mid(i,1).toHex() + " " );
    }
    file1 . write ( "\n---\n" + in );
}
