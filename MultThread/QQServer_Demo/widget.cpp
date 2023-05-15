#include "widget.h"
#include "ui_widget.h"
#include "recvmessage.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, [=]() {
        ui->pushButton->setEnabled(false);

        m_server = new QTcpServer(this);
        QString IpAddress = ui->ipText->text();
        unsigned short port = ui->portText->text().toUShort();
        if (!m_server->listen(QHostAddress::Any, port)) {
            qCritical() << "Failed to start server:" << m_server->errorString();
        }
        else{
            qDebug() << "Server started on port" << m_server->serverPort();
            ui->showText->setText(QString("服务器正在监听..."));
            connect(m_server, &QTcpServer::newConnection, this, [=]() { // 使用this指针访问类成员变量和函数
                QTcpSocket* m_client = m_server->nextPendingConnection();
                qDebug() << "Client connected.";

                RecvMessage* subThread = new RecvMessage(m_client);
                void(RecvMessage::* rptr)(QString) = &RecvMessage::appendText;
                connect(subThread, rptr, this, [=](QString text){
                    ui->showText->append(text);
                });

                connect(subThread, &RecvMessage::addTcpSocket, this, [=](QTcpSocket* tc){
                    m_tcpList.push_back(tc);
                });

                connect(subThread, &RecvMessage::sendAllPeople, this, &Widget::SendAllClient);

                connect(subThread, &RecvMessage::deleteTcpSocket, this, [=](QTcpSocket* tcp){
                    tcp->disconnectFromHost();
                    for(auto it = m_tcpList.begin(); it!=m_tcpList.end(); it++){
                        if(*it == tcp){
                            auto tmp = it;
                            tmp++;
                            m_tcpList.erase(it);
                            it = tmp;
                            break;
                        }
                    }
                });

                connect(this, &Widget::adPerson, subThread, &RecvMessage::DadPeron);

                connect(subThread, &RecvMessage::addPerson, this, [=](QString name){
                   memberList.push_back(name);
                   QString str = "";
                   for(auto it = memberList.begin(); it!=memberList.end(); it++){
                       str += *it;
                       str += ":";
                   }
                   strMemberList = str;
                   emit adPerson(strMemberList);
                });

                connect(subThread, &RecvMessage::deletePerson, this, [=](QString name){
                    QString str = "";
                    QList<QString> newMemberList;
                    for (const auto& member : memberList) {
                        if (member == name) {
                            continue;
                        }
                        newMemberList.push_back(member);
                        str += member + ":";
                    }
                    memberList = newMemberList;
                    strMemberList = str;
                    emit adPerson(strMemberList);
                });


                subThread->start();
            });
        }
    });
}

void Widget::SendAllClient(QByteArray replyData){
    for(auto it = m_tcpList.begin(); it!=m_tcpList.end(); it++){
        (*it)->write(replyData);
        (*it)->waitForBytesWritten();
    }
}

Widget::~Widget()
{
    delete m_server;
    delete ui;
}
