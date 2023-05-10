#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include "message.h"

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    Client(QWidget *parent = nullptr, QTcpSocket* tcpSocket = nullptr, QString username = "");
    void SendMessage(QString MsgType);
    ~Client();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Client *ui;
    QTcpSocket* m_tcpSocket;
    QString UserName;
};

#endif // CLIENT_H
