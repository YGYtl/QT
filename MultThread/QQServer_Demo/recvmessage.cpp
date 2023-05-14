#include "recvmessage.h"

RecvMessage::RecvMessage(QTcpSocket* tcp, QObject *parent) : QThread(parent),
    m_tcp(tcp)
{

}

void RecvMessage::dataReceived()
{
    QByteArray data = m_tcp->readAll();
    QDataStream in(&data, QIODevice::ReadOnly);
    Message msg;
    in >> msg;

    QString UserName = msg.getUserName();
    qDebug() << UserName;

    QByteArray replyData;
    QDataStream out(&replyData, QIODevice::WriteOnly);
    Message reply;


    if(msg.getMsgType() == "MD_LOGIN"){
        if(Mm.QueryMember(msg) == 2 || Mm.QueryMember(msg) == 3){
            reply.setMsgType("MD_LOGIN_FAILD");
            if(Mm.QueryMember(msg) == 2){
                reply.setMsgText("Invalid username or password.");
            }
            else{
                reply.setMsgText("The user has not registered");
            }
            out << reply;
            m_mutex.lock();
            m_tcp->write(replyData);
            m_tcp->waitForBytesWritten();
            m_mutex.unlock();
        }
        else{
            reply.setMsgType("MD_LOGIN_SUCCESS");
            reply.setMsgText("Login successful");
            out << reply;
            m_mutex.lock();
            m_tcp->write(replyData);
            m_tcp->waitForBytesWritten();
            m_mutex.unlock();

            emit appendText(QString(msg.getUserName()+" has come in."));
            m_mutex.lock();
            emit addTcpSocket(m_tcp);
            m_mutex.unlock();
        }
    }
    else if(msg.getMsgType() == "MD_REGISTER"){
        if(Mm.QueryMember(msg) == 3){//用户不存在，可以注册
            reply.setMsgType("MD_REGISTER_SUCESS");
            reply.setMsgText("UserName is available for registration");
            Mm.AddMember(msg);//将用户添加到数据库表
        }
        else{
            reply.setMsgType("MD_REGISTER_FAILD");
            reply.setMsgText("UserName alright exits");
        }
        out << reply;
        m_mutex.lock();
        m_tcp->write(replyData);
        m_tcp->waitForBytesWritten();
        m_mutex.unlock();
    }
    else if(msg.getMsgType() == "MD_TALK"){
        emit appendText(QString(msg.getUserName()+": "+msg.getMsgText()));

        reply.setMsgType("MD_TALK");//转发给其他用户
        reply.setMsgText(QString(msg.getUserName()+": "+msg.getMsgText()));
        out << reply;
        qDebug() << replyData;
        m_mutex.lock();
        emit sendAllPeople(replyData);
        m_mutex.unlock();
    }
    else if(msg.getMsgType() == "MD_QUIT"){
        m_mutex.lock();

        reply.setMsgType("MD_TALK");//转发给其他用户，有用户退出
        reply.setMsgText(QString(msg.getUserName()+" user has already logged out!"));
        emit appendText(QString(msg.getUserName()+" user has already logged out!"));
        out << reply;
        emit sendAllPeople(replyData);


        QByteArray replyData2;
        QDataStream out2(&replyData2, QIODevice::WriteOnly);
        Message reply2;
        reply2.setMsgType("MD_QUIT");
        out2 << reply2;
        qDebug() << replyData2;
        m_tcp->write(replyData2);
        m_tcp->waitForBytesWritten();

        emit deleteTcpSocket(m_tcp);
        m_mutex.unlock();
    }
    emit exit();
}

void RecvMessage::run()
{
    connect(m_tcp, &QTcpSocket::readyRead, this, &RecvMessage::dataReceived);
}
