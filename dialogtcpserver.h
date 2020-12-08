#ifndef DIALOGTCPSERVER_H
#define DIALOGTCPSERVER_H

#include <QDialog>
#include <QFile>

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
    void slotRead(NetAPI::TcpSessionInfo *sessionInfo, const QByteArray &data, qint64 size);

    void slotBytesWritten(qint64 numBytes); //tcpsocket写入时，触发

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
    QByteArray outBlock;  //写数据缓冲区
    QString message;  //网络传输数据
    qint16 blockSize = 0;  //缓冲区大小
    //----------接收文件 start---------//
    qint64 totalBytes = 0; //传输的总大小
    qint64 bytesReceived = 0;
    qint64 fileNameSize = 0;
    QString fileName; //文件名
    QFile *localFile;
    //----------接收文件 end---------//
};

#endif // DIALOGTCPSERVER_H
