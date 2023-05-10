#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include<QNetworkProxyFactory>
#include"client.h"
#include"register.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr, bool flag = true);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket* tcpSocket;
    QString UserName;
    Client* ClientDialog;
    bool flag;
};

#endif // WIDGET_H
