#ifndef DIALOGTCPCLIENT_H
#define DIALOGTCPCLIENT_H

#include <QDialog>
#include <QFile>

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

    void slotRead(NetAPI::TcpSessionInfo *sessionInfo, const QByteArray &data, qint64 size);

    void slotBytesWritten(qint64 numBytes); //tcpsocket写入时，触发

    void slotDisconnect();
    //----------网络部分 end---------//

    void on_btnConnect_clicked();

    void on_btnDisconnect_clicked();

    void on_btnSend_clicked();

    void on_btnClear_clicked();

    void on_btnSelectFile_clicked();

    void on_btnSendFile_clicked();

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
    //----------发送文件 end---------//
    QFile *localFile; //文件
    qint64 totalBytes; //传输的总大小
    qint64 bytesWritten; //已传输文件的大小
    qint64 bytesToWrite; //待传输的文件的大小
    qint64 payloadSize; //载荷大小
    QString fileName; //文件名
    //----------发送文件 end---------//
};

#endif // DIALOGTCPCLIENT_H
