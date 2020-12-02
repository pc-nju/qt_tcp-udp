#ifndef UDPSESSIONTHREADS_H
#define UDPSESSIONTHREADS_H

#include "../netapinamespace.h"

#include <QObject>
#include <mutex>
#include <unordered_map>
#include <memory>

NETAPI_NAMESPACE_BEGIN

class UdpThread;
class UdpSession;

class UdpSessionThreads : public QObject
{
    Q_OBJECT

public:
    UdpSessionThreads();
    ~UdpSessionThreads();
    //启动线程池
    bool start(uint32_t threadNum);
    //关闭
    void stop();
    //获取最小会话数线程
    UdpThread *pickMinThread();
    //获取会话数
    std::vector<uint32_t> getSessionSize() const;

    std::shared_ptr<UdpSession> createSession();

    void addSession(std::shared_ptr<UdpSession> session);

private slots:
    //会话断开
    void slotSessionDisConnected(void *id);

private:
    std::vector<UdpThread*> threadList;
    //互斥量
    std::mutex lock;
    //会话列表（void*：接收任意类型的指针）
    std::unordered_map<void*, std::shared_ptr<UdpSession>> sessionList;
    bool isRunning = false;
};

NETAPI_NAMESPACE_END

#endif // UDPSESSIONTHREADS_H
