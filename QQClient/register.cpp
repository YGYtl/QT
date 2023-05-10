#include "register.h"
#include "ui_register.h"
#include<QMessageBox>
#include<QPushButton>
#include"message.h"
#include"widget.h"

Register::Register(QWidget *parent, bool fl) :
    QWidget(parent),
    ui(new Ui::Register),
    flag(fl)
{
    ui->setupUi(this);

    connect(ui->RegisterButton,&QPushButton::clicked,[=](){
        QString name = ui->UserNameText->text().trimmed();
        QString pwd = ui->UserPwdText->text().trimmed();

        // 构造登录消息
        Message msg;
        msg.setUserName(name);
        msg.setUserPwd(pwd);
        msg.setInetAddress(QString("127.0.0.1"));
        msg.setPort(8888);
        msg.setMsgType("MD_REGISTER");

        QHostAddress localAddress("192.168.230.51");
        tcpSocket = new QTcpSocket(this); // 声明成员变量
        quint16 port(123);

        // 建立连接
        tcpSocket->connectToHost(QHostAddress::LocalHost, port);
        if (tcpSocket->waitForConnected()) {
            // 发送登录消息
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << msg;
            tcpSocket->write(data);//把用户信息发到服务器端
            tcpSocket->flush();
            connect(tcpSocket, &QTcpSocket::readyRead, [=]() {
                if(flag){
                    // 收到服务器端发送的消息
                    QByteArray ServerResponseData = tcpSocket->readAll();
                    QDataStream in(&ServerResponseData, QIODevice::ReadOnly);
                    Message ServerResponse;
                    in >> ServerResponse;
                    qDebug() << ServerResponse.getMsgText();
                    qDebug() << ServerResponse.getInetAddress();
                    qDebug() << ServerResponse.getMsgType();
                    qDebug() << ServerResponse.getUserName();
                    qDebug() << ServerResponse.getUserPwd();

                    if(ServerResponse.getMsgType() == "MD_REGISTER_FAILD"){
                        QMessageBox::critical(nullptr,
                                "Register Error",
                                ServerResponse.getMsgText());
                    }
                    else{
                        flag = false;
                        QMessageBox::information(nullptr,
                                "Register Success",
                                ServerResponse.getMsgText());
                        this->hide();
                        Widget* LoginDialog = new Widget;
                        LoginDialog->show();
                        delete LoginDialog;
                    }
                }
            });
        } else {
            qDebug() << "连接失败" << tcpSocket->error();
        }
    });
}

Register::~Register()
{
    tcpSocket->close();
    delete tcpSocket;
    delete ui;
}
