#include "message.h"

Message::Message()
{

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
    out << obj.strInetAddress << obj.strMsgText << obj.strMsgType
         << obj.strUserName << obj.strUserPwd;
    return out;
}
