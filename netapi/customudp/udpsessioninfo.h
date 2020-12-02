#ifndef UDPSESSIONINFO_H
#define UDPSESSIONINFO_H

#include "../netapinamespace.h"

#include <QObject>
#include <memory>

#include "udpsession.h"

NETAPI_NAMESPACE_BEGIN
class UdpSessionInfo : public QObject
{
    Q_OBJECT

signals:
    void signalRead(UdpSessionInfo *, const QByteArray &, int);
    void signalConnect();
    void signalDisconnect();

public:
    UdpSessionInfo(std::shared_ptr<UdpSession> session);
    ~UdpSessionInfo();
    //绑定端口
    void connectServer(quint16 port);
    //解绑端口
    void disconnectServer();
    //发送数据默认加密
    void doWrite(const QByteArray &buffer, const QString &targetIp, quint16 port);

public:
    std::function<void(void*)> onDisConnected = nullptr;

private slots:
    void slotRead(const QByteArray &data, int size);
    void slotDisconnected();

private:
    std::shared_ptr<UdpSession> session;
};
NETAPI_NAMESPACE_END
#endif // UDPSESSIONINFO_H
