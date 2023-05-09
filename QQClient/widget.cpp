#include "widget.h"
#include "ui_widget.h"
#include "register.h"
#include "message.h"
#include <QDebug>
#include "register.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->RegisterButton,&QPushButton::clicked,[=](){
        this->close();
        Register* dialog = new Register;
        dialog->show();
    });

    connect(ui->LoginButton, &QPushButton::clicked, [=](){
        QString name = ui->UserNameText->text().trimmed();
        QString pwd = ui->UserPwdText->text().trimmed();

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
            qDebug() << "连接成功";
            // 发送登录消息
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << msg;
            tcpSocket->write(data);
            tcpSocket->flush();
            connect(tcpSocket, &QTcpSocket::readyRead, [=]() {
                // 收到服务器端发送的消息
                QByteArray response = tcpSocket->readAll();
                qDebug() << "Received message from server:" << response;

                // 构造回复消息
                Message replyMsg;
                replyMsg.setUserName(name);
                replyMsg.setUserPwd(pwd);
                replyMsg.setInetAddress(QString("127.0.0.1"));
                replyMsg.setPort(8888);
                replyMsg.setMsgType("MD_REPLY");

                // 回复消息给服务器
                QByteArray replyData;
                QDataStream replyStream(&replyData, QIODevice::WriteOnly);
                replyStream << replyMsg;
                tcpSocket->write(replyData);
                tcpSocket->flush();
            });
        } else {
            qDebug() << "连接失败" << tcpSocket->error();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}
