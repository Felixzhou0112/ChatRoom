#include "chattingserver.h"
#include "operatedatabase.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OperateDatabase::getInstance().initial();
    ChattingServer::getInstance().show();

    return a.exec();
}
