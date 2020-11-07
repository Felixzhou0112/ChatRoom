#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>
#include "tcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();

    static MyTcpServer& getInstance();

    void incomingConnection(qintptr socketDescriptor);

    void resend(QStringList namelist, PDU *pdu);

public slots:
    void deleteSocket(TcpSocket *socket);

private:
    QList<TcpSocket *> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
