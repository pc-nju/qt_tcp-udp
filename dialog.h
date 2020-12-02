#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "netapi/netapifactory.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_btnTcpServer_clicked();

    void on_btnTcpClient_clicked();

    void on_btnUdpClient_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
