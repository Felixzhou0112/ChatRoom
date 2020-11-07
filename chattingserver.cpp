#include "chattingserver.h"
#include "ui_chattingserver.h"


//服务器构造函数
ChattingServer::ChattingServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChattingServer)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_IP), m_port);
}

//服务器析构函数
ChattingServer::~ChattingServer()
{
    delete ui;
}

//服务器的单例函数
ChattingServer &ChattingServer::getInstance()
{
    static ChattingServer instance;
    return instance;
}

//服务器加载配置文件函数
void ChattingServer::loadConfig()
{
    QFile file(":/server.config");

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        QString newData = data.toStdString().c_str();
        file.close();
        newData.replace("\r\n", " ");
        QStringList List = newData.split(" ");
        m_IP = List.at(0);
        m_port = List.at(1).toUShort();
//       qDebug() << m_IP << m_port;
    }
    else
    {
        QMessageBox::critical(this, "open config", "open config failed");
    }
}

//服务器消息显示函数
void ChattingServer::showInformation(QString information)
{
    ui->textEdit->append(information);
}
