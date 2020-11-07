#include <QDebug>
#include "mytcpserver.h"
#include "chattingserver.h"


//���캯��
MyTcpServer::MyTcpServer()
{

}

//��������
MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

//��д  incomingConnection()  ����
//�������ӵĿͻ���  QTcpSocket  ת�����Զ����  socke  ���б�������

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    TcpSocket *pTcpSocket = new TcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);
    QString note = QString("IP: %1 , port: %2  connected").arg(pTcpSocket->peerAddress().toString()).arg(pTcpSocket->peerPort());
    ChattingServer::getInstance().showInformation(note);
    connect(pTcpSocket, SIGNAL(offline(TcpSocket*)), this, SLOT(deleteSocket(TcpSocket*)));
}

//��Ϣת������
void MyTcpServer::resend(QStringList namelist, PDU *pdu)
{
    for(int i=0; i<namelist.size(); i++)
    {
        for(int j=0; j<m_tcpSocketList.size(); j++)
        {
            if(namelist.at(i) == m_tcpSocketList.at(j)->getName())
            {
                qDebug() << m_tcpSocketList.at(j)->getName() << "will got message";
                m_tcpSocketList.at(j)->write((char*)pdu,pdu->PDULength);
            }
        }
    }
}

//�ͻ�������ʱ  ɾ����Ӧ  socket  �ĺ���
void MyTcpServer::deleteSocket(TcpSocket *socket)
{
    QList<TcpSocket *>::iterator iter = m_tcpSocketList.begin();
    for(; iter != m_tcpSocketList.end(); iter++)
    {
        if(socket == *iter)
        {
            m_tcpSocketList.erase(iter);
            socket->close();
            delete socket;
            break;
        }
    }
//    for(int i=0; i<m_tcpSocketList.size(); i++)
//    {
//        qDebug() << m_tcpSocketList.at(i)->getName();
//    }
}
