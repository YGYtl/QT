#ifndef MEMBERMANAGER_H
#define MEMBERMANAGER_H
#include"databaseconnection.h"
#include"message.h"
#include<QSqlQuery>

class MemberManager
{
public:
    MemberManager();
    bool AddMember(Message msg);
    bool DeleteMember(Message msg);
    bool UpdateMember(Message msg);
    int QueryMember(Message msg);

private:
    DatabaseConnection db;
    QSqlQuery query;
};

#endif // MEMBERMANAGER_H
