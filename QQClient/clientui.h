#ifndef CLIENTUI_H
#define CLIENTUI_H

#include <QWidget>

namespace Ui {
class ClientUI;
}

class ClientUI : public QWidget
{
    Q_OBJECT

public:
    explicit ClientUI(QWidget *parent = nullptr);
    ~ClientUI();

private:
    Ui::ClientUI *ui;
};

#endif // CLIENTUI_H
