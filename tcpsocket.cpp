#include "tcpsocket.h"
#include "mytcpserver.h"

//构造函数
TcpSocket::TcpSocket()
{
    connect(this, &TcpSocket::readyRead, this, &TcpSocket::receiveMessage);
//    connect(this, SIGNAL(disconnected()), this, SLOT(clientOffline()));
}

//获得用户名函数
QString TcpSocket::getName()
{
    return m_userName;
}


//处理注册请求函数
void TcpSocket::handleRegistRequest(PDU *pdu)
{
    char name[32] = {'\0'};
    char password[32] = {'\0'};
    strncpy(name, pdu->data, 32);
    strncpy(password, pdu->data+32, 32);
    bool ret = OperateDatabase::getInstance().handleRegist(name, password);
    PDU *respond = makePDU(0);
    respond->messageType = MESSAGE_TYPE_REGIST_RESPOND;
    if(ret)
    {
        strcpy(respond->data, REGIST_OK);
    }
    else
    {
        strcpy(respond->data, REGIST_FAILED);
    }
    write((char*)respond, respond->PDULength);
    free(respond);
    respond = NULL;
}

//处理登录请求函数
void TcpSocket::handleLoginRequest(PDU *pdu)
{
    char name[32] = {'\0'};
    char password[32] = {'\0'};
    strncpy(name, pdu->data, 32);
    strncpy(password, pdu->data+32, 32);
    QStringList list = OperateDatabase::getInstance().roomListWhenLogin();

    bool ret = OperateDatabase::getInstance().handleLogin(name, password);
    uint messageLength = (list.size()+1) * 32;
    PDU *respond = makePDU(messageLength);
    respond->messageType = MESSAGE_TYPE_LOGIN_RESPOND;
    for(int i=0; i<list.size(); i++)
    {
        strncpy((char*)(respond->message)+i*32,list.at(i).toStdString().c_str(),list.at(i).size());
    }
    strncpy(respond->name, name, 32);

    if(ret)
    {
        strcpy(respond->data, LOGIN_OK);
        m_userName = name;

    }
    else
    {
        strcpy(respond->data, LOGIN_FAILED);
    }
    write((char*)respond, respond->PDULength);
    free(respond);
    respond = NULL;
}

//处理建房请求函数
void TcpSocket::handleSetRoomRequest()
{
    bool ret = OperateDatabase::getInstance().checkRoomStatus(m_userName.toStdString().c_str());
    if(ret)
    {
        PDU *respond = makePDU(0);
//        strncpy(respond->name, m_userName.toStdString().c_str(), m_userName.size());
        strcpy(respond->data,SET_ROOM_FAILED);
        respond->messageType = MESSAGE_TYPE_SET_ROOM_RESPOND;
        write((char*)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }
    else
    {
        OperateDatabase::getInstance().handleSetRoom(m_userName.toStdString().c_str());
        PDU *respond = makePDU(0);
        strncpy(respond->name, m_userName.toStdString().c_str(), m_userName.size());
        strcpy(respond->data,SET_ROOM_OK);
        respond->messageType = MESSAGE_TYPE_SET_ROOM_RESPOND;
        write((char*)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }

}

//处理加入房间请求函数
void TcpSocket::handleJoinRoomRequest(PDU *pdu)
{
    bool ret = OperateDatabase::getInstance().checkRoomStatus(pdu->name);
    if(ret)
    {
        char targetName[32] = {'\0'};
        char selfName[32] = {'\0'};
        strncpy(targetName, pdu->name, 32);
        strncpy(selfName, m_userName.toStdString().c_str(), m_userName.size());

        QStringList list = OperateDatabase::getInstance().handleJoinRoom(targetName,selfName);
        uint messageLength = list.size() * 32;
        PDU *respond = makePDU(messageLength);
        respond->messageType = MESSAGE_TYPE_JOIN_ROOM_RESPOND;
        strcpy(respond->data, JOIN_ROOM_OK);
        strncpy(respond->name, targetName, 32);

        for(int i=0; i<list.size(); i++)
        {
            strncpy((char*)(respond->message)+i*32,list.at(i).toStdString().c_str(),list.at(i).size());
        }
        write((char*)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }
    else
    {
        PDU * respond = makePDU(0);
        respond->messageType = MESSAGE_TYPE_JOIN_ROOM_RESPOND;
        strcpy(respond->data, JOIN_ROOM_FAILED);
        write((char*)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }

}

//处理刷新房间列表请求函数
void TcpSocket::handleRefreshRoomRequest()
{
    QStringList list = OperateDatabase::getInstance().handleRefreshRoom();
    uint messageLength = list.size()*32;
    PDU *respond = makePDU(messageLength);
    respond->messageType = MESSAGE_TYPE_REFRESH_ROOM_LIST_RESPOND;
    for(int i=0; i<list.size(); i++)
    {
        strncpy((char*)(respond->message)+i*32, list.at(i).toStdString().c_str(), list.at(i).size());
    }
    write((char*)respond, respond->PDULength);
    free(respond);
    respond = NULL;
}

//处理刷新房间在线人员请求函数
void TcpSocket::handleRefreshOnlineRequest(PDU *pdu)
{
    char roomName[32] = {'\0'};
    strncpy(roomName, pdu->name, 32);
    bool ret = OperateDatabase::getInstance().checkRoomStatus(roomName);
    if(ret)
    {
        QStringList list = OperateDatabase::getInstance().handleRefreshOnline(roomName);
        uint length = list.size()*32;
        PDU *respond = makePDU(length);
        respond->messageType = MESSAGE_TYPE_REFRESH_ONLINE_LIST_RESPOND;
        for(int i=0; i<list.size(); i++)
        {
            strncpy((char*)(respond->message)+i*32, list.at(i).toStdString().c_str(), list.at(i).size());
        }

        write((char*)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }
    else
    {
        PDU *respond = makePDU(0);
        respond->messageType = MESSAGE_TYPE_REFRESH_ONLINE_LIST_RESPOND;
        strcpy(respond->data, REFRESH_FAILED);
        write((char*)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }

}

//处理发送信息请求函数
void TcpSocket::handleSendMessageRequest(PDU *pdu)
{

    char roomName[32] = {'\0'};
    strncpy(roomName, pdu->name, 32);
    bool ret = OperateDatabase::getInstance().checkRoomStatus(roomName);
    if(ret)
    {
        //这里利用刷新列表函数得到该房间的所有用户
//        qDebug() << "room exist when send message";
        QStringList list = OperateDatabase::getInstance().handleRefreshOnline(roomName);
        PDU *respond = makePDU(pdu->messageLength);
        strncpy((char*)(respond->message), (char*)(pdu->message), pdu->messageLength);
        QString sender = m_userName;
        strncpy(respond->name, sender.toStdString().c_str(), sender.size());
//        qDebug() << respond->name << " send " << (char*)(respond->message);
        strcpy(respond->data, SEND_OK);
        respond->messageType = MESSAGE_TYPE_SEND_MESSAGE_RESPOND;
        MyTcpServer::getInstance().resend(list, respond);
    }
    else
    {
        PDU * respond = makePDU(0);
        respond->messageType = MESSAGE_TYPE_SEND_MESSAGE_RESPOND;
        strcpy(respond->data, SEND_FAILED);
        write((char*)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }

}

//处理退出房间请求函数
void TcpSocket::handleExitRoomRequest(PDU *pdu)
{
    QString userName = getName();
    if(0 == strcmp(pdu->name, userName.toStdString().c_str()))
    {
//        qDebug() << "room name is--->" << pdu->name;
        OperateDatabase::getInstance().cancelRoom(pdu->name);
        PDU *respond = makePDU(0);
        respond->messageType = MESSAGE_TYPE_EXIT_ROOM_RESPOND;
        strcpy(respond->data, CANCEL_ROOM_OK);
        write((char *)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }
    else
    {
//        qDebug() << "userName is--->" << userName;
        OperateDatabase::getInstance().leaveRoom(userName.toStdString().c_str());
        PDU *respond = makePDU(0);
        respond->messageType = MESSAGE_TYPE_EXIT_ROOM_RESPOND;
        strncpy(respond->name, userName.toStdString().c_str(), userName.size());
//        qDebug() << respond->name << "be saved";
        strcpy(respond->data, LEAVE_ROOM_OK);
        write((char *)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }
}

//处理房间关闭函数
void TcpSocket::handleRoomClose(QString roomName, QString userName)
{
    if(0 == strcmp(roomName.toStdString().c_str(), userName.toStdString().c_str()))
    {
        OperateDatabase::getInstance().cancelRoom(roomName.toStdString().c_str());
        PDU *respond = makePDU(0);
        respond->messageType = MESSAGE_TYPE_CLOSE_ROOM_RESPOND;
        write((char *)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }
    else
    {
        OperateDatabase::getInstance().leaveRoom(userName.toStdString().c_str());
        PDU *respond = makePDU(0);
        respond->messageType = MESSAGE_TYPE_CLOSE_ROOM_RESPOND;
        write((char *)respond, respond->PDULength);
        free(respond);
        respond = NULL;
    }
}

//处理窗口（主界面）关闭函数
void TcpSocket::handleWidgetClose(QString name)
{
    OperateDatabase::getInstance().handleCloseWidget(name.toStdString().c_str());
    PDU *respond = makePDU(0);
    respond->messageType = MESSAGE_TYPE_CLOSE_WIDGET_RESPOND;
    qDebug() << "set the respond--->"<< MESSAGE_TYPE_CLOSE_WIDGET_RESPOND;
    write((char *)respond, respond->PDULength);
    free(respond);
    respond = NULL;
}

//客户端接收数据函数
void TcpSocket::receiveMessage()
{
    uint PDULength = 0;
    this->read((char *)&PDULength, sizeof(uint));
    uint messageLength = PDULength - sizeof(PDU);
    PDU * pdu = makePDU(messageLength);
    this->read((char *)pdu + sizeof(uint), PDULength - sizeof(uint));
    switch(pdu->messageType)
    {
    case MESSAGE_TYPE_REGIST_REQUEST:       //注册请求
    {
        handleRegistRequest(pdu);
        break;
    }
    case MESSAGE_TYPE_LOGIN_REQUEST:        //登录请求
    {
        handleLoginRequest(pdu);
        break;
    }
    case MESSAGE_TYPE_SET_ROOM_REQUEST:         //建房请求
    {
        handleSetRoomRequest();
        break;
    }
    case MESSAGE_TYPE_JOIN_ROOM_REQUEST:            //进房请求
    {
        handleJoinRoomRequest(pdu);
        break;
    }
    case MESSAGE_TYPE_REFRESH_ROOM_LIST_REQUEST:        //刷新房间列表请求
    {
        handleRefreshRoomRequest();
        break;
    }
    case MESSAGE_TYPE_REFRESH_ONLINE_LIST_REQUEST:          //刷新在线成员请求
    {
        handleRefreshOnlineRequest(pdu);
        break;
    }
    case MESSAGE_TYPE_SEND_MESSAGE_REQUEST:         //发送消息请求
    {
        handleSendMessageRequest(pdu);
        break;
    }
    case MESSAGE_TYPE_CLOSE_CLIENT_REQUEST:         //关闭客户端请求
    {
        clientOffline();

        break;
    }
    case MESSAGE_TYPE_EXIT_ROOM_REQUEST:            //退出房间请求
    {
        handleExitRoomRequest(pdu);
        break;
    }
    case MESSAGE_TYPE_CLOSE_ROOM_REQUEST:           //关闭房间请求
    {
        handleRoomClose(pdu->name, m_userName);
        break;
    }
    case MESSAGE_TYPE_CLOSE_WIDGET_REQUEST:         //关闭窗口请求
    {
        qDebug() << "got the request for close widget";

        handleWidgetClose(m_userName);
        qDebug() << "I sent "<< m_userName << "to the function";
        break;
    }
    default:
    {
        break;
    }
    }
    free(pdu);
    pdu = NULL;
}

//处理客户端下线函数
void TcpSocket::clientOffline()
{
    OperateDatabase::getInstance().handleOffline(m_userName.toStdString().c_str());
    emit offline(this);
}
