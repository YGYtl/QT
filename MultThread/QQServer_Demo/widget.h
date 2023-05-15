#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThreadPool>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    void SendAllClient(QByteArray replyData);
    ~Widget();

signals:
    void adPerson(QString strmemberlist);

private:
    Ui::Widget *ui;
    QTcpServer* m_server;
    QList<QTcpSocket*> m_tcpList;
    QList<QString> memberList;
    QString strMemberList;
};




#endif // WIDGET_H
