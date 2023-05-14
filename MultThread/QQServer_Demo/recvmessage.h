#ifndef RECVMESSAGE_H
#define RECVMESSAGE_H

#include <QObject>
#include <QThread>
#include <QRunnable>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QMutex>
#include "message.h"
#include "membermanager.h"

class RecvMessage : public QThread
{
    Q_OBJECT
public:
    explicit RecvMessage(QTcpSocket* tcp, QObject *parent = nullptr);

    void dataReceived();

protected:
    void run() override;
signals:
    void appendText(QString text);
    void threadStarted();
    void threadFinished();
    void exit();
    void deleteTcpSocket(QTcpSocket* tcp);
    void addTcpSocket(QTcpSocket* tcp);
    void sendAllPeople(QByteArray text);

public slots:

private:
    QTcpSocket* m_tcp;
    MemberManager Mm;
    QList<QTcpSocket*> m_tcpList;
    QMutex m_mutex;//定义互斥锁
};

#endif // RECVMESSAGE_H
