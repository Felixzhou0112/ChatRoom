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

    MESSAGE_TYPE_REGIST_REQUEST,        //ע������
    MESSAGE_TYPE_REGIST_RESPOND,        //ע��ظ�

    MESSAGE_TYPE_LOGIN_REQUEST,         //��¼����
    MESSAGE_TYPE_LOGIN_RESPOND,         //��¼�ظ�

    MESSAGE_TYPE_SET_ROOM_REQUEST,      //��������
    MESSAGE_TYPE_SET_ROOM_RESPOND,      //�����ظ�

    MESSAGE_TYPE_JOIN_ROOM_REQUEST,     //���뷿������
    MESSAGE_TYPE_JOIN_ROOM_RESPOND,     //���뷿��ظ�

    MESSAGE_TYPE_CLOSE_WIDGET_REQUEST,      //�رմ�������
    MESSAGE_TYPE_CLOSE_WIDGET_RESPOND,       //�رմ��ڻظ�

    MESSAGE_TYPE_REFRESH_ROOM_LIST_REQUEST,     //ˢ�·����б�����
    MESSAGE_TYPE_REFRESH_ROOM_LIST_RESPOND,     //ˢ�·����б�ظ�

    MESSAGE_TYPE_REFRESH_ONLINE_LIST_REQUEST,       //ˢ�������б�����
    MESSAGE_TYPE_REFRESH_ONLINE_LIST_RESPOND,       //ˢ�������б�ظ�

    MESSAGE_TYPE_SEND_MESSAGE_REQUEST,       //������Ϣ����
    MESSAGE_TYPE_SEND_MESSAGE_RESPOND,       //������Ϣ�ظ�

    MESSAGE_TYPE_CLOSE_CLIENT_REQUEST,       //�ͻ��˹ر�����
    MESSAGE_TYPE_CLOSE_CLIENT_RESPOND,       //�ͻ��˹رջظ�

    MESSAGE_TYPE_CLOSE_ROOM_REQUEST,       //����ر�����
    MESSAGE_TYPE_CLOSE_ROOM_RESPOND,       //����رջظ�

    MESSAGE_TYPE_EXIT_ROOM_REQUEST,       //�˳���������
    MESSAGE_TYPE_EXIT_ROOM_RESPOND,       //�˳�����ظ�

    MESSAGE_TYPE_MAX=0x00ffffff
};

typedef struct protocolDataUnity
{
    uint PDULength;         //Э�����ݵ�Ԫ�ܳ���
    uint messageType;       //��Ϣ����
    char data[64];               //ͨѶ��Ϣ
    char name[32];              //�������
    uint messageLength;     //ʵ����Ϣ����
    int message[];                  //ʵ����Ϣ
}PDU;

PDU *makePDU(uint messageLength);


#endif // PROTOCOL_H
