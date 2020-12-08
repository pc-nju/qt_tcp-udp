#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include "../netapinamespace.h"
#include "tcpsession.h"

#include <QObject>
#include <memory>

NETAPI_NAMESPACE_BEGIN

class TcpSessionInfo : public QObject
{
    Q_OBJECT

signals:
    void signalRead(TcpSessionInfo *, const QByteArray &, qint64);
    void signalBytesWritten(qint64 numBytes);
    void signalConnect();
    void signalDisconnect();

public:
    TcpSessionInfo(std::shared_ptr<TcpSession> session);
    ~TcpSessionInfo();
    //连接到服务端
    void connectServer(const QString &host, quint16 port);
    //断开连接
    void disconnectServer();
    //发送数据默认加密
    void doWrite(const QByteArray &data);

public:
    std::function<void(void*)> onDisConnected = nullptr;

private slots:
    void slotRead(const QByteArray &data, qint64 size);
    void slotBytesWritten(qint64 numBytes);  //tcpsocket写入时，触发
    void slotDisconnected();

private:
    std::shared_ptr<TcpSession> session;
};
NETAPI_NAMESPACE_END
#endif // SESSIONINFO_H
