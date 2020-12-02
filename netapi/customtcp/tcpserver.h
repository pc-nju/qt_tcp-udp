#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "tcpsession.h"
#include "tcpsessionthreads.h"
#include "../netapinamespace.h"

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <memory>
#include <thread>

NETAPI_NAMESPACE_BEGIN

struct TcpServerData {
    uint16_t port;
    uint32_t threadNum;
    //验证逻辑，不合法则设定初始值
    void verify() {
        if (port == 0) {
            port = 9090;
        }
        if (threadNum) {
            //检测硬件并发特性
            threadNum = std::thread::hardware_concurrency();
        }
        if (threadNum == 0) {
            //检测硬件并发特性
            threadNum = 1;
        }
    }
};

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = Q_NULLPTR);
    ~TcpServer();

    bool start(TcpServerData &conf);
    void stop();

    std::vector<uint32_t> getSessionSize() const;

public:
    //新连接回调
    std::function<void(std::shared_ptr<TcpSession> &tcpSession)> onAccepted = nullptr;

    bool getIsRunning() const;

protected:
    virtual void incomingConnection(qintptr handle);

private:
    bool isRunning = false;
    TcpSessionThreads sessionThreads;
};

NETAPI_NAMESPACE_END

#endif // TCPSERVER_H
