#ifndef SESSIONTHREADS_H
#define SESSIONTHREADS_H

#include "../netapinamespace.h"

#include <QObject>
#include <mutex>
#include <unordered_map>
#include <memory>

NETAPI_NAMESPACE_BEGIN

class TcpThread;
class TcpSession;

class TcpSessionThreads : public QObject
{
    Q_OBJECT

public:
    TcpSessionThreads();
    ~TcpSessionThreads();
    //启动线程池
    bool start(uint32_t threadNum);
    //关闭
    void stop();
    //获取最小会话数线程
    TcpThread *pickMinThread();
    //获取会话数
    std::vector<uint32_t> getSessionSize() const;
    //根据系统类型不同而不同，32位系统为qint32、64位系统为qint64
    std::shared_ptr<TcpSession> createSession(qintptr handle);

    void addSession(std::shared_ptr<TcpSession> session);

private slots:
    //会话断开
    void slotSessionDisConnected(void *id);

private:
    std::vector<TcpThread*> threadList;
    //互斥量
    std::mutex lock;
    //会话列表（void*：接收任意类型的指针）
    std::unordered_map<void*, std::shared_ptr<TcpSession>> sessionList;
    bool isRunning = false;
};

NETAPI_NAMESPACE_END
#endif // SESSIONTHREADS_H
