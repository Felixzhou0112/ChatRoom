#include "operatedatabase.h"


//构造函数
OperateDatabase::OperateDatabase(QObject *parent) : QObject(parent)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
}

//单例模式函数
OperateDatabase &OperateDatabase::getInstance()
{
    static OperateDatabase instance;
    return instance;
}

//数据库初始化函数
void OperateDatabase::initial()
{
    m_database.setHostName("localhost");
    m_database.setDatabaseName("E:\\QT_C\\ChatRoom\\ChattingServer\\chattingroom.db");
    if(m_database.open())
    {
        QSqlQuery query;
        query.exec("select * from userInformation");
        while(query.next())
        {
            QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
//            qDebug() << data;
        }
    }
    else
    {
        QMessageBox::critical(NULL, "this", "open database", "open failed");
    }
}

//数据库处理注册函数
bool OperateDatabase::handleRegist(const char *name, const char *password)
{
    if(NULL == name || NULL == password)
    {
//        qDebug() << "name || password is NULL";
        return false;
    }
    QString data = QString("insert into userInformation(name, password) values(\'%1\',\'%2\')").arg(name).arg(password);
    QSqlQuery query;
    return query.exec(data);
}

//数据库处理登录函数
bool OperateDatabase::handleLogin(const char *name, const char *password)
{
    if(NULL == name || NULL == password)
    {
//        qDebug() << "name || password is NULL";
        return false;
    }
    QString data = QString("select * from userInformation where name=\'%1\' and password=\'%2\' and online=0").arg(name).arg(password);
    QSqlQuery query;
    query.exec(data);
    if(query.next())
    {
        QString data = QString("update userInformation set online=1 where name=\'%1\' and password=\'%2\'").arg(name).arg(password);
        QSqlQuery query;
        query.exec(data);
        return true;
    }
    else
    {
        return false;
    }
}

//数据库处理下线函数
void OperateDatabase::handleOffline(const char *name)
{
    if(NULL == name)
    {
//        qDebug() << "name is NULL";
        return;
    }
    QString userData = QString("update userInformation set online=0 , room=0 where name=\'%1\'").arg(name);
    QSqlQuery query;
    query.exec(userData);
    QString roomData = QString("delete from roomlist");
    query.exec(roomData);
}

//数据库处理建房函数
void OperateDatabase::handleSetRoom(const char *name)
{
    if(NULL == name)
    {
        return;
    }
    QString data = QString("update userInformation set room=1 where name=\'%1\'").arg(name);
    QSqlQuery query;
    query.exec(data);
}

//处理加入房间函数
QStringList OperateDatabase::handleJoinRoom(const char *targetRoomName, const char *selfName)
{
    QString insertData = QString("insert into roomlist (id, memberId, memberName,roomName)"
                           " values((select id from userInformation where name=\'%1\'),"
                           "(select id from userInformation where name=\'%2\'),\'%3\',\'%4\')").arg(targetRoomName).arg(selfName).arg(selfName).arg(targetRoomName);
    QSqlQuery query;
    query.exec(insertData);
    QString selectData = QString("select memberName from roomlist where id=(select id from userInformation where name=\'%1\')").arg(targetRoomName);
    query.exec(selectData);
    QStringList result;
    result.clear();
    while(query.next())
    {
        result.append(query.value(0).toString());
//        qDebug() << "database-->" << query.value(0).toString();
    }
    result.append(targetRoomName);
    return result;
}

//处理主界面刷新房间列表的函数
QStringList OperateDatabase::handleRefreshRoom()
{
    QString data = QString("select name from userInformation where online=1 and room=1");
    QSqlQuery query;
    query.exec(data);
    QStringList result;
    result.clear();
    while(query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

//处理房间界面刷新在线人数列表的函数
QStringList OperateDatabase::handleRefreshOnline(const char *roomName)              //roomName=\'%1\'
{
    QString selectData = QString("select memberName from roomlist where roomName=\'%1\'").arg(roomName);
//    qDebug() << selectData;
    QSqlQuery query;
    query.exec(selectData);
    QStringList result;
    result.clear();
    while(query.next())
    {
        result.append(query.value(0).toString());
//        qDebug() << "database refresh--->" << query.value(0).toString();
    }
    result.append(roomName);
    return result;
}

//处理登录时显示已有房间列表的函数
QStringList OperateDatabase::roomListWhenLogin()
{
    QString data = QString("select name from userInformation where online=1 and room=1");
    QSqlQuery query;
    query.exec(data);
    QStringList result;
    result.clear();
    while(query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

//解散房间的函数
void OperateDatabase::cancelRoom(const char *roomName)
{
    QString roomData = QString("delete from roomlist where roomName=\'%1\'").arg(roomName);
    QSqlQuery query;
    query.exec(roomData);
    QString userData = QString("update userInformation set room=0 where name=\'%1\'").arg(roomName);
    query.exec(userData);
}

void OperateDatabase::leaveRoom(const char *memberName)
{
    QString data = QString("delete from roomlist where memberName=\'%1\'").arg(memberName);
    QSqlQuery query;
    query.exec(data);
}

//检查房间状态函数
bool OperateDatabase::checkRoomStatus(const char *roomName)
{
    QString data = QString("select room from userInformation where name=\'%1\'").arg(roomName);
//    qDebug() << data;
    QSqlQuery query;
    query.exec(data);
//    QVariant temp = query.value(0);
    if(query.first())
    {
        if(1 == query.value(0).toInt())
        {
//            qDebug() << "room status--->" << query.value(0).toInt();
            return true;
        }
        else
        {
//            qDebug() << "room status--->" << query.value(0).toInt();
            return false;
        }
    }
    else
    {
        return false;
    }
}

//处理窗口（主界面）关闭函数
void OperateDatabase::handleCloseWidget(const char *name)
{
    QString data = QString("update userInformation set online=0 where name=\'%1\'").arg(name);
    QSqlQuery query;
    query.exec(data);
}

//析构函数，关闭数据库
OperateDatabase::~OperateDatabase()
{
    m_database.close();
}
