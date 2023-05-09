#include "widget.h"
#include "ui_widget.h"
#include <QDebug>


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
            QHostAddress address("192.168.230.51");
            qDebug() << address;
            qDebug() << port;

            if (!m_server->listen(QHostAddress::LocalHost, port)) { // 监听端口失败
                qDebug() << "Listen failed";
                return;
            }

            ui->ShowMessageText->setText(QString("服务器正在监听..."));
            connect(m_server, &QTcpServer::newConnection, this, [=]() { // 使用this指针访问类成员变量和函数
                QTcpSocket* client = m_server->nextPendingConnection();
                qDebug() << "Client connected.";

                client->write("Welcome to the server!");
                connect(client, &QTcpSocket::readyRead, this, [=]() {
                    QByteArray data = client->readAll();
                    QDataStream in(&data, QIODevice::ReadOnly);
                    Message msg;
                    in >> msg;


                });

                client->write("Hello from server!");
                client->flush();
            });
        }
        else {
            qDebug() << "Port Error";
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

