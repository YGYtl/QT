#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include"widget.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr, bool flag = true);
    ~Register();

private:
    Ui::Register *ui;
    QTcpSocket* tcpSocket;
    bool flag;

};

#endif // REGISTER_H
