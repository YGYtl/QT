#include "membermanager.h"

MemberManager::MemberManager()
{
}
bool MemberManager::AddMember(Message msg){
    QString name = msg.getUserName();
    QString pwd = msg.getUserPwd();
    query.prepare("INSERT INTO usertable (UserName, UserPassword) VALUES (:name, :pwd)");
    query.bindValue(":name", name);
    query.bindValue(":pwd", pwd);
    query.exec();
    return true;
}
bool MemberManager::DeleteMember(Message msg){
    return true;
}
bool MemberManager::UpdateMember(Message msg){
    return true;
}
int MemberManager::QueryMember(Message msg){
    QString name = msg.getUserName();
    QString pwd = msg.getUserPwd();
    query.exec("SELECT * FROM usertable WHERE UserName='" + name + "'");
    while(query.next())
    {
        if(query.value(2).toString() != pwd){
            return 2;
        }
        else{
            return 1;
        }
    }
    return 3;
}
