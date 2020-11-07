#ifndef OPERATEDATABASE_H
#define OPERATEDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QDebug>
#include <QStringList>


class OperateDatabase : public QObject
{
    Q_OBJECT
public:
    explicit OperateDatabase(QObject *parent = 0);

    ~OperateDatabase();

    static OperateDatabase &getInstance();

    void initial();

    bool handleRegist(const char *name, const char *password);

    bool handleLogin(const char *name, const char *password);

    void handleOffline(const char *name);

    void handleSetRoom(const char *name);

    QStringList handleJoinRoom(const char *targetRoomName, const char *selfName);

    QStringList handleRefreshRoom();

    QStringList handleRefreshOnline(const char *roomName);

    QStringList roomListWhenLogin();

    void cancelRoom(const char * roomName);

    void leaveRoom(const char *roomName);

    //查看房间状态函数
    bool checkRoomStatus(const char* roomName);

    void handleCloseWidget(const char* name);


signals:

public slots:

private:
    QSqlDatabase m_database;
};

#endif // OPERATEDATABASE_H
