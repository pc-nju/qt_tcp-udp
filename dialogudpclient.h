#ifndef DIALOGUDPCLIENT_H
#define DIALOGUDPCLIENT_H

#include <QDialog>

#include "netapi/netapifactory.h"

namespace Ui {
class DialogUdpClient;
}

class DialogUdpClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUdpClient(QWidget *parent = 0);
    ~DialogUdpClient();

private slots:
    //----------网络部分 start---------//
    void slotConnect();

    void slotRead(NetAPI::UdpSessionInfo *sessionInfo, const QByteArray &data, int size);

    void slotDisconnect();
    //----------网络部分 end---------//

    void on_btnConnect_clicked();

    void on_btnDisConnect_clicked();

    void on_btnSend_clicked();

    void on_btnClear_clicked();

private:
    void initStyle();
    void writeLog(const QString &msg);

private:
    Ui::DialogUdpClient *ui;
    NetAPI::UdpClientManager *clientManager;
    NetAPI::UdpSessionInfo *sessionInfo;
};

#endif // DIALOGUDPCLIENT_H
