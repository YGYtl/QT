#include "client.h"
#include "ui_client.h"
#include<QKeyEvent>

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
}


Client::Client(QWidget *parent, QTcpSocket* tcpSocket, QString username):QWidget(parent),
    ui(new Ui::Client),
    m_tcpSocket(tcpSocket),
    UserName(username)
{
    ui->setupUi(this);

    connect(ui->sendButton,&QPushButton::clicked,[=](){
        SendMessage("MD_TALK");
    });

    connect(tcpSocket, &QTcpSocket::readyRead,this, [=](){
        // 收到服务器端发送的消息
        QByteArray data = tcpSocket->readAll();
        QDataStream in(&data, QIODevice::ReadOnly);
        Message msg;
        in >> msg;

        qDebug() << msg.getMsgType();
        qDebug() << msg.getMsgText();
        ui->showTalkText->append(msg.getMsgText());
    });

    connect(ui->loyoutButton,&QPushButton::clicked,[=](){
        SendMessage("MD_QUIT");
    });
}

void Client::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        SendMessage("MD_TALK");
    } else {
        QWidget::keyPressEvent(event);
    }
}

void Client::SendMessage(QString MsgType)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    Message msg;
    msg.setUserName(UserName);
    msg.setMsgType(MsgType);
    if(ui->textEdit->text() != QString("")){
        msg.setMsgText(ui->textEdit->text());
        out << msg;
        m_tcpSocket->write(data);
        m_tcpSocket->flush();
        ui->textEdit->clear();
    }
}

Client::~Client()
{
    delete m_tcpSocket;
    delete ui;
}
