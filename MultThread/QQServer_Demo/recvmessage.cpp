#include "recvmessage.h"

RecvMessage::RecvMessage(QTcpSocket* tcp, QObject *parent) : QThread(parent),
    m_tcp(tcp)
{

}

void RecvMessage::dataReceived()
{
    m_mutex.lock();
    QByteArray data = m_tcp->readAll();
    Mm = new MemberManager();
    m_mutex.unlock();

    QDataStream in(&data, QIODevice::ReadOnly);
    Message msg;
    in >> msg;

    QString UserName = msg.getUserName();
    qDebug() << UserName;

    QByteArray replyData;
    QDataStream out(&replyData, QIODevice::WriteOnly);
    Message reply;


    if(msg.getMsgType() == "MD_LOGIN"){
        m_mutex.lock();
        if(Mm->QueryMember(msg) == 2 || Mm->QueryMember(msg) == 3){
            reply.setMsgType("MD_LOGIN_FAILD");
            if(Mm->QueryMember(msg) == 2){
                reply.setMsgText("Invalid username or password.");
            }
            else{
                reply.setMsgText("The user has not registered");
            }
            out << reply;
            m_tcp->write(replyData);
            m_tcp->waitForBytesWritten();
        }
        else{

            QByteArray replyData2;
            QDataStream out2(&replyData2, QIODevice::WriteOnly);
            Message reply2;
            reply2.setMsgType("MD_TALK");
            reply2.setMsgText(QString(msg.getUserName()+" has come in."));
            out2 << reply2;
            qDebug() << replyData2;
            emit sendAllPeople(replyData2);//转发给其他人，有人进来了


            reply.setMsgType("MD_LOGIN_SUCCESS");
            reply.setMsgText("Login successful");
            out << reply;
            m_tcp->write(replyData);
            m_tcp->waitForBytesWritten();
            emit appendText(QString(msg.getUserName()+" has come in."));
            emit addTcpSocket(m_tcp);


            emit addPerson(msg.getUserName());//添加到主线程的memberlist
            QByteArray replyData3;
            QDataStream out3(&replyData3, QIODevice::WriteOnly);
            Message reply3;
            reply3.setMsgType("MD_UPDATELIST");
            reply3.setMsgText(strMemberlist);
            out3 << reply3;
            qDebug() << "---" << strMemberlist;
            emit sendAllPeople(replyData3);//转发给其他人，更新在线用户列表
        }
        m_mutex.unlock();
    }
    else if(msg.getMsgType() == "MD_REGISTER"){
        m_mutex.lock();
        if(Mm->QueryMember(msg) == 3){//用户不存在，可以注册
            reply.setMsgType("MD_REGISTER_SUCESS");
            reply.setMsgText("UserName is available for registration");
            Mm->AddMember(msg);//将用户添加到数据库表
        }
        else{
            reply.setMsgType("MD_REGISTER_FAILD");
            reply.setMsgText("UserName alright exits");
        }

        out << reply;
        m_tcp->write(replyData);
        m_tcp->waitForBytesWritten();
        m_mutex.unlock();
    }
    else if(msg.getMsgType() == "MD_TALK"){
        m_mutex.lock();
        emit appendText(QString(msg.getUserName()+": "+msg.getMsgText()));

        reply.setMsgType("MD_TALK");//转发给其他用户
        reply.setMsgText(QString(msg.getUserName()+": "+msg.getMsgText()));
        out << reply;
        qDebug() << replyData;
        emit sendAllPeople(replyData);

        m_mutex.unlock();
    }
    else if(msg.getMsgType() == "MD_QUIT"){
        m_mutex.lock();
        reply.setMsgType("MD_QUIT_PEOQ");//转发给其他用户，有用户退出
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


        emit deletePerson(msg.getUserName());//从主线程的memberlist删除该用户
        QByteArray replyData3;
        QDataStream out3(&replyData3, QIODevice::WriteOnly);
        Message reply3;
        reply3.setMsgType("MD_UPDATELIST");
        reply3.setMsgText(strMemberlist);
        out3 << reply3;
        emit sendAllPeople(replyData3);//转发给其他人，更新在线用户列表

        m_mutex.unlock();
    }
}

void RecvMessage::DadPeron(QString strmemberlist){
    this->strMemberlist = strmemberlist;
}

void RecvMessage::run()
{
    connect(m_tcp, &QTcpSocket::readyRead, this, &RecvMessage::dataReceived);
}
