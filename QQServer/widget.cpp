#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include"membermanager.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->StartButton, &QPushButton::clicked, [=]() {
        ui->StartButton->setEnabled(false);

        QString IpAddress = ui->IPText->text();
        QString Port = ui->PortText->text();
        bool ok;
        int value = Port.toInt(&ok);
        if (ok && value >= 0 && value <= 0xFFFF) {
            quint16 port = static_cast<quint16>(value);
            m_server = new QTcpServer(this); // 声明成员变量
            QHostAddress address(IpAddress);

            if (!m_server->listen(address, port)) { // 监听端口失败
                qDebug() << "Listen failed";
                return;
            }

            ui->ShowMessageText->setText(QString("服务器正在监听..."));
            connect(m_server, &QTcpServer::newConnection, this, [=]() { // 使用this指针访问类成员变量和函数
                client = m_server->nextPendingConnection();
                qDebug() << "Client connected.";

                connect(client, &QTcpSocket::readyRead, this, [=]() {
                    QByteArray data = client->readAll();
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
                            client->write(replyData);
                            client->flush();
                        }
                        else{
                            reply.setMsgType("MD_LOGIN_SUCCESS");
                            reply.setMsgText("Login successful");
                            out << reply;
                            client->write(replyData);
                            client->flush();

                            ui->ShowMessageText->append(QString(msg.getUserName()+" has come in."));
                            ClientList.push_back(client);
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
                        client->write(replyData);
                        client->flush();
                    }
                    else if(msg.getMsgType() == "MD_TALK"){
                        ui->ShowMessageText->append(QString(msg.getUserName()+": "+msg.getMsgText()));

                        reply.setMsgType("MD_TALK");//转发给其他用户
                        reply.setMsgText(QString(msg.getUserName()+": "+msg.getMsgText()));
                        out << reply;
                        qDebug() << replyData;
                        for(auto it = ClientList.begin(); it!=ClientList.end(); it++){
                            (*it)->write(replyData);
                            (*it)->flush();
                        }
                    }
                    else if(msg.getMsgType() == "MD_QUIT"){
                        qDebug() << ClientList.size();
                         for(auto it = ClientList.begin(); it!=ClientList.end(); it++){
                            if(*it == client){

                                reply.setMsgType("MD_TALK");//转发给其他用户，有用户退出
                                reply.setMsgText(QString(msg.getUserName()+" user has already logged out!"));
                                ui->ShowMessageText->append(QString(msg.getUserName()+" user has already logged out!"));
                                qDebug() << QString(msg.getUserName()+" user has already logged out!");
                                out << reply;

                                qDebug() << ClientList.size();
                                for(auto iter=ClientList.begin(); iter!=ClientList.end(); iter++){
                                    if(*iter!=client){
                                        (*iter)->write(replyData);
                                        (*iter)->flush();
                                    }
                                }

                                QByteArray replyData2;
                                QDataStream out2(&replyData2, QIODevice::WriteOnly);
                                Message reply2;
                                reply2.setMsgType("MD_QUIT");
                                reply2.setInetAddress("q1");
                                reply2.setMsgText("q2");
                                reply2.setUserName("q3");
                                reply2.setUserPwd("q4");
                                out2 << reply2;
                                qDebug() << replyData2;
                                client->write(replyData2);
                                client->flush();

                                auto tmp = it;
                                tmp++;
                                ClientList.erase(it);
                                it = tmp;
                                break;
                            }
                        }
                        return;
                    }
                });
            });
        }
        else {
            qDebug() << "Port Error";
        }
    });
}

Widget::~Widget()
{
    delete m_server;
    for(auto it=ClientList.begin(); it!=ClientList.end(); it++){
        auto tmp = it;
        tmp++;
        ClientList.erase(it);
        it = tmp;
    }
    delete ui;
}

