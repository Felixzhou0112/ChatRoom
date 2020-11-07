#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QDebug>

#include "operatedatabase.h"
#include "protocol.h"


class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket();

    QString getName();

    void handleRegistRequest(PDU *pdu);

    void handleLoginRequest(PDU *pdu);

    void handleSetRoomRequest();

    void handleJoinRoomRequest(PDU *pdu);

    void handleRefreshRoomRequest();

    void handleRefreshOnlineRequest(PDU *pdu);

    void handleSendMessageRequest(PDU *pdu);

    void handleExitRoomRequest(PDU *pdu);

    void handleRoomClose(QString roomName, QString userName);

    void handleWidgetClose(QString name);

signals:

    void offline(TcpSocket * socket);

public slots:
    void receiveMessage();

    void clientOffline();


private:
    QString m_userName;
};

#endif // TCPSOCKET_H
