#include "clientui.h"
#include "ui_clientui.h"

ClientUI::ClientUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientUI)
{
    ui->setupUi(this);
}

ClientUI::~ClientUI()
{
    delete ui;
}
