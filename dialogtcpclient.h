#ifndef DIALOGTCPCLIENT_H
#define DIALOGTCPCLIENT_H

#include <QDialog>

#include "netapi/netapifactory.h"

namespace Ui {
class DialogTcpClient;
}

class DialogTcpClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTcpClient(QWidget *parent = 0);
    ~DialogTcpClient();

private slots:
    //----------网络部分 start---------//
    void slotConnect();

    void slotRead(NetAPI::TcpSessionInfo *sessionInfo, const QByteArray &data, int size);

    void slotDisconnect();
    //----------网络部分 end---------//

    void on_btnConnect_clicked();

    void on_btnDisconnect_clicked();

    void on_btnSend_clicked();

    void on_btnClear_clicked();

private:
    void initStyle();
    void writeLog(const QString &msg);

private:
    Ui::DialogTcpClient *ui;
    NetAPI::TcpClientManager *clientManager;
    NetAPI::TcpSessionInfo *sessionInfo;
    QByteArray outBlock;  //写数据缓冲区
    QString message;  //网络传输数据
    qint16 blockSize = 0;  //缓冲区大小
};

#endif // DIALOGTCPCLIENT_H
