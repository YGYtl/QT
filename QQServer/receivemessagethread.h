#ifndef RECEIVEMESSAGETHREAD_H
#define RECEIVEMESSAGETHREAD_H

#include <QObject>
#include <QRunnable>
#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

class ReceiveMessageThread : public QRunnable
{
    Q_OBJECT
public:
    explicit ReceiveMessageThread(QTcpSocket* socket);
    void run() override;

signals:
    void message(const QString &msg);

public slots:
private:
    QTcpSocket* m_socket;
};

#endif // RECEIVEMESSAGETHREAD_H
