#ifndef MESSAGE_H
#define MESSAGE_H
#include<QString>
#include<QDataStream>

class Message
{
public:
    Message();
public:
    void setUserName(QString userName);
    void setMsgType(QString msgType);
    void setUserPwd(QString userPwd);
    void setMsgText(QString msgText);
    void setInetAddress(QString InetAddress);
    void setPort(quint16 port);
    QString getUserName();
    QString getUserPwd();
    QString getMsgType();
    QString getMsgText();
    QString getInetAddress();
    quint16 getPort();
    friend QDataStream& operator<<(QDataStream& out, const Message& obj);
private:
    QString strUserName;
    QString strUserPwd;
    QString strMsgType;
    QString strMsgText;
    QString strInetAddress;
    quint16 Port;
};

#endif // MESSAGE_H
