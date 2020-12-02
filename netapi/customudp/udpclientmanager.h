#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "../netapinamespace.h"

#include "udpsession.h"
#include "udpsessionthreads.h"

#include <QObject>
#include <QThread>
#include <memory>
#include <thread>

NETAPI_NAMESPACE_BEGIN

struct UdpServerData {
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

class UdpClientManager : public QObject
{
    Q_OBJECT

public:
    UdpClientManager();
    ~UdpClientManager();

    bool start(UdpServerData &conf);
    void stop();

    std::vector<uint32_t> getSessionSize() const;

    //创建一个客户端
    std::shared_ptr<UdpSession> createClient();

    bool getIsRunning() const;
private:
    bool isRunning = false;
    UdpSessionThreads sessionThreads;
};

NETAPI_NAMESPACE_END

#endif // UDPSERVER_H
