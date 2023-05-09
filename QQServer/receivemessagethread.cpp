#include "receivemessagethread.h"

ReceiveMessageThread::ReceiveMessageThread(QTcpSocket* socket) : m_socket(socket)
{

}

void ReceiveMessageThread::run()
{
    //处理消息并发送信号
    while(true){

    }
}
