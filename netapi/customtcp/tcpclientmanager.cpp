#include "tcpclientmanager.h"

#include "tcpsession.h"
#include "tcpsessionthreads.h"
#include "tcpthread.h"

#include <thread>

NETAPI_NAMESPACE_BEGIN

TcpClientManager::~TcpClientManager()
{

}

bool TcpClientManager::start(uint32_t threadNum)
{
    if (isRunning) {
        return false;
    }
    //验证
    if (threadNum == 0) {
        threadNum = std::thread::hardware_concurrency();
    }
    //启动
    this->sessionThreads.start(threadNum);
    isRunning = true;
    return true;
}

void TcpClientManager::stop()
{
    if (!isRunning) {
        return;
    }
    //停止会话线程池
    this->sessionThreads.stop();
    isRunning = false;
}

std::vector<uint32_t> TcpClientManager::getSessionSize() const
{
    return this->sessionThreads.getSessionSize();
}

std::shared_ptr<TcpSession> TcpClientManager::createClient()
{
    TcpThread *thread = this->sessionThreads.pickMinThread();
    std::shared_ptr<TcpSession> session = std::make_shared<TcpSession>(thread);
    this->sessionThreads.addSession(session);
    session->moveToThread(thread);
    if (thread) {
        thread->addOne();
    }
    return session;
}

NETAPI_NAMESPACE_END
