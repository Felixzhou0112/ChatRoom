#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned int uint;

#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed : name existed"

#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed : name or password error or relogin"

#define SET_ROOM_OK "set room succeed"
#define SET_ROOM_FAILED "set room failed"

#define JOIN_ROOM_OK "join room succeed"
#define JOIN_ROOM_FAILED "join room failed"

#define REFRESH_FAILED "refresh failed"

#define SEND_OK "send ok"
#define SEND_FAILED "send failed"

#define CANCEL_ROOM_OK "exit room ok"
#define CANCEL_ROOM_FAILED "exit room failed"

#define LEAVE_ROOM_OK "leave room ok"
#define LEAVE_ROOM_FAILED "leave room failed"


enum MESSAGE_TYPE
{
    MESSAGE_TYPE_MIN=0,

    MESSAGE_TYPE_REGIST_REQUEST,        //注册请求
    MESSAGE_TYPE_REGIST_RESPOND,        //注册回复

    MESSAGE_TYPE_LOGIN_REQUEST,         //登录请求
    MESSAGE_TYPE_LOGIN_RESPOND,         //登录回复

    MESSAGE_TYPE_SET_ROOM_REQUEST,      //建房请求
    MESSAGE_TYPE_SET_ROOM_RESPOND,      //建房回复

    MESSAGE_TYPE_JOIN_ROOM_REQUEST,     //加入房间请求
    MESSAGE_TYPE_JOIN_ROOM_RESPOND,     //加入房间回复

    MESSAGE_TYPE_CLOSE_WIDGET_REQUEST,      //关闭窗口请求
    MESSAGE_TYPE_CLOSE_WIDGET_RESPOND,       //关闭窗口回复

    MESSAGE_TYPE_REFRESH_ROOM_LIST_REQUEST,     //刷新房间列表请求
    MESSAGE_TYPE_REFRESH_ROOM_LIST_RESPOND,     //刷新房间列表回复

    MESSAGE_TYPE_REFRESH_ONLINE_LIST_REQUEST,       //刷新在线列表请求
    MESSAGE_TYPE_REFRESH_ONLINE_LIST_RESPOND,       //刷新在线列表回复

    MESSAGE_TYPE_SEND_MESSAGE_REQUEST,       //发送消息请求
    MESSAGE_TYPE_SEND_MESSAGE_RESPOND,       //发送消息回复

    MESSAGE_TYPE_CLOSE_CLIENT_REQUEST,       //客户端关闭请求
    MESSAGE_TYPE_CLOSE_CLIENT_RESPOND,       //客户端关闭回复

    MESSAGE_TYPE_CLOSE_ROOM_REQUEST,       //房间关闭请求
    MESSAGE_TYPE_CLOSE_ROOM_RESPOND,       //房间关闭回复

    MESSAGE_TYPE_EXIT_ROOM_REQUEST,       //退出房间请求
    MESSAGE_TYPE_EXIT_ROOM_RESPOND,       //退出房间回复

    MESSAGE_TYPE_MAX=0x00ffffff
};

typedef struct protocolDataUnity
{
    uint PDULength;         //协议数据单元总长度
    uint messageType;       //消息类型
    char data[64];               //通讯消息
    char name[32];              //存放名字
    uint messageLength;     //实际消息长度
    int message[];                  //实际消息
}PDU;

PDU *makePDU(uint messageLength);


#endif // PROTOCOL_H
