#include "widget.h"
#include "ui_widget.h"
#include "message.h"
#include <QDebug>
#include<QMessageBox>


Widget::Widget(QWidget *parent, bool fl) :
    QWidget(parent),
    ui(new Ui::Widget),
    flag(fl)
{
    ui->setupUi(this);//登录窗口

    connect(ui->RegisterButton,&QPushButton::clicked,this, [=](){
        this->hide();
        Register* dialog = new Register;
        dialog->show();
        delete dialog;
    });

    connect(ui->LoginButton, &QPushButton::clicked, this, [=](){
        QString name = ui->UserNameText->text().trimmed();
        QString pwd = ui->UserPwdText->text().trimmed();
        UserName = name;

        // 构造登录消息
        Message msg;
        msg.setUserName(name);
        msg.setUserPwd(pwd);
        msg.setInetAddress(QString("127.0.0.1"));
        msg.setPort(8888);
        msg.setMsgType("MD_LOGIN");

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
            connect(tcpSocket, &QTcpSocket::readyRead, this, [=]() {
                if(flag){
                    // 收到服务器端发送的消息
                    QByteArray ServerResponseData = tcpSocket->readAll();
                    QDataStream in(&ServerResponseData, QIODevice::ReadOnly);
                    Message ServerResponse;
                    in >> ServerResponse;

                    if(ServerResponse.getMsgType() == "MD_LOGIN_FAILD"){
                        QMessageBox::critical(nullptr,
                                "Login Error",
                                ServerResponse.getMsgText());
                    }
                    else{
                        this->hide();
                        flag = false;
                        ClientDialog = new Client(nullptr, tcpSocket, UserName);
                        ClientDialog->show();
                    }
                }
            });
        } else {
            qDebug() << "连接失败" << tcpSocket->error();
        }
    });
}

Widget::~Widget()
{
    tcpSocket->close();
    delete tcpSocket;
    delete ClientDialog;
    delete ui;
}
