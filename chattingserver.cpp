#include "chattingserver.h"
#include "ui_chattingserver.h"


//���������캯��
ChattingServer::ChattingServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChattingServer)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_IP), m_port);
}

//��������������
ChattingServer::~ChattingServer()
{
    delete ui;
}

//�������ĵ�������
ChattingServer &ChattingServer::getInstance()
{
    static ChattingServer instance;
    return instance;
}

//���������������ļ�����
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

//��������Ϣ��ʾ����
void ChattingServer::showInformation(QString information)
{
    ui->textEdit->append(information);
}
