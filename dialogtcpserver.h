#ifndef DIALOGTCPSERVER_H
#define DIALOGTCPSERVER_H

#include <QDialog>

#include "netapi/netapifactory.h"

namespace Ui {
class DialogTcpServer;
}

class DialogTcpServer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTcpServer(QWidget *parent = 0);
    ~DialogTcpServer();

private slots:
    //----------网络部分 start---------//
    void slotRead(NetAPI::TcpSessionInfo *sessionInfo, const QByteArray &data, int size);

    void slotConnect();

    void slotDisconnect();
    //----------网络部分 end---------//

    void on_btStart_clicked();

    void on_btStop_clicked();

    void on_btClear_clicked();

private:
    void initStyle();

private:
    Ui::DialogTcpServer *ui;
    NetAPI::TcpServer *server; //tcp server 端
    NetAPI::TcpSessionInfoList *sessionList; //session 集合
    QString message;  //网络传输数据
    qint16 blockSize = 0;  //缓冲区大小
};

#endif // DIALOGTCPSERVER_H
