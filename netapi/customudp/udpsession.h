#ifndef UDPSESSION_H
#define UDPSESSION_H

#include "../netapinamespace.h"

#include <QUdpSocket>
#include <functional>

NETAPI_NAMESPACE_BEGIN

class UdpThread;

class UdpSession : public QUdpSocket
{
    Q_OBJECT

public:
    UdpSession(UdpThread *thread, QObject *parent = Q_NULLPTR);
    ~UdpSession();

    void disConnect();
    void doWrite(const QByteArray &data, const QString &targetHost, int targetPort);
    void connectToServer(quint16 port);

public:
    //断开连接回调，将来可能要用
    std::function<void(void*)> onDisconnected = nullptr;

signals:
    void signalConnect(quint16 port);
    void signalRead(const QByteArray &data, int len);
    void signalDisConnected(void *);
    void signalDoWrite(const QByteArray &, const QString &, int);
    void signalDoDisConnect();

private slots:
    //连接
    void slotConnect(quint16 port);
    //开始读数据
    void slotStartRead();
    //断开连接回调
    void slotDisConnected();
    //写数据
    void slotDoWrite(const QByteArray &data, const QString &targetHost, int targetPort);
    //断开连接
    void slotDoDisconnect();


private:
    UdpThread *thread = nullptr;
    QByteArray buffer = nullptr;
};

NETAPI_NAMESPACE_END

#endif // UDPSESSION_H
