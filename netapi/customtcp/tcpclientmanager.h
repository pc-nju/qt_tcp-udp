#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "tcpsessionthreads.h"
#include "../netapinamespace.h"

#include <QObject>
#include <memory>

NETAPI_NAMESPACE_BEGIN

class TcpSession;

class TcpClientManager
{
public:
    TcpClientManager() = default;
    ~TcpClientManager();

    //启动,默认启动一个线程，非线程安全
    bool start(uint32_t threadNum = 1);
    //关闭 非线程安全
    void stop();
    //获取会话数
    std::vector<uint32_t> getSessionSize() const;
    //创建一个客户端
    std::shared_ptr<TcpSession> createClient();

private:
    //线程会话池
    TcpSessionThreads sessionThreads;
    //是否正在运行
    bool isRunning = false;
};

NETAPI_NAMESPACE_END
#endif // CLIENTMANAGER_H
