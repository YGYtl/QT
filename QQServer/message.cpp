#include "message.h"

Message::Message()
{
    strUserName = ".";
    strUserPwd = ".";
    strMsgType = ".";
    strMsgText = ".";
    strInetAddress = ".";
    Port = 0;
}

void Message::setUserName(QString userName){
    this->strUserName = userName;
}
void Message::setMsgType(QString msgType){
    this->strMsgType = msgType;
}
void Message::setUserPwd(QString userPwd){
    this->strUserPwd = userPwd;
}
void Message::setMsgText(QString msgText){
    this->strMsgText = msgText;
}
void Message::setInetAddress(QString InetAddress){
    this->strInetAddress = InetAddress;
}
void Message::setPort(quint16 port){
    this->Port = port;
}
QString Message::getUserName(){
    return this->strUserName;
}
QString Message::getUserPwd(){
    return this->strUserPwd;
}
QString Message::getMsgType(){
    return this->strMsgType;
}
QString Message::getMsgText(){
    return this->strMsgText;
}
QString Message::getInetAddress(){
    return this->strInetAddress;
}
quint16 Message::getPort(){
    return this->Port;
}

QDataStream& operator<<(QDataStream& out, const Message& obj){
    out << obj.strInetAddress;
    out << obj.strMsgText;
    out << obj.Port;
    out << obj.strMsgType;
    out << obj.strUserName;
    out << obj.strUserPwd;
    return out;
}

QDataStream& operator>>(QDataStream& in, Message& msg){
    // 依次读取各个成员变量

   QString inetAddress;
   QString text;
   quint16 port;
   QString msgType;
   QString userName;
   QString userPwd;

   in >> inetAddress;
   in >> text;
   in >> port;
   in >> msgType;
   in >> userName;
   in >> userPwd;

   msg.setInetAddress(inetAddress);
   msg.setMsgText(text);
   msg.setPort(port);
   msg.setMsgType(msgType);
   msg.setUserName(userName);
   msg.setUserPwd(userPwd);

   return in;
}
