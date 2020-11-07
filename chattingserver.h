#ifndef CHATTINGSERVER_H
#define CHATTINGSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QHostAddress>

#include "mytcpserver.h"

namespace Ui {
class ChattingServer;
}

class ChattingServer : public QWidget
{
    Q_OBJECT

public:
    explicit ChattingServer(QWidget *parent = 0);
    ~ChattingServer();

    static ChattingServer & getInstance();

    void loadConfig();

    void showInformation(QString information);

private:
    Ui::ChattingServer *ui;
    QString m_IP;
    quint16 m_port;
};

#endif // CHATTINGSERVER_H
