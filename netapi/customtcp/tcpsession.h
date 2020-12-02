#ifndef TCPSESSION_H
#define TCPSESSION_H

#include "../netapinamespace.h"

#include <QTcpSocket>
#include <functional>

NETAPI_NAMESPACE_BEGIN
class TcpThread;

class TcpSession : public QTcpSocket
{
    Q_OBJECT

public:
    TcpSession(TcpThread *thread, QObject *parent = Q_NULLPTR);
    ~TcpSession();

    void disConnect();
    void doWrite(const QByteArray &data);
    void connectToServer(const QString &host, quint16 port);

public:
    //断开连接回调，将来可能要用
    std::function<void(void*)> onDisconnected = nullptr;

signals:
    void signalConnect(const QString &host, quint16 port);
    void signalRead(const QByteArray &data, int size);
    void signalDisConnected(void *);
    void signalDoWrite(const QByteArray &data);
    void signalDoDisConnect();

private slots:
    //连接
    void slotConnect(const QString &host, quint16 port);
    //开始读数据
    void slotStartRead();
    //断开连接回调
    void slotDisConnected();
    //写数据
    void slotDoWrite(const QByteArray &data);
    //断开连接
    void slotDoDisconnect();


private:
    TcpThread *thread = nullptr;
    QByteArray buffer = nullptr;
};
NETAPI_NAMESPACE_END
#endif // TCPSESSION_H
