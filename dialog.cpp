#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>

#include "dialogtcpserver.h"
#include "dialogtcpclient.h"
#include "dialogudpclient.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnTcpServer_clicked()
{
    DialogTcpServer *dlg = new DialogTcpServer(this);
    dlg->exec();
}

void Dialog::on_btnTcpClient_clicked()
{
    DialogTcpClient *dlg = new DialogTcpClient(this);
    dlg->exec();
}

void Dialog::on_btnUdpClient_clicked()
{
    DialogUdpClient *dlg = new DialogUdpClient(this);
    dlg->exec();
}
