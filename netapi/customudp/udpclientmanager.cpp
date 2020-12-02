#include "udpclientmanager.h"

#include "udpthread.h"

NETAPI_NAMESPACE_BEGIN

UdpClientManager::UdpClientManager()
{

}

UdpClientManager::~UdpClientManager()
{
    this->stop();
}

bool UdpClientManager::start(UdpServerData &conf)
{
    if (isRunning) {
        return true;
    }
    //验证数据
    conf.verify();
    //启动线程池
    sessionThreads.start(conf.threadNum);
    isRunning = true;
    qDebug() << "DdpServer::Start threadID:"<< QThread::currentThreadId();
    return true;
}

void UdpClientManager::stop()
{
    if (!this->isRunning) {
        return;
    }
    //关闭线程池
    sessionThreads.stop();
    isRunning = false;
}

std::vector<uint32_t> UdpClientManager::getSessionSize() const
{
    return sessionThreads.getSessionSize();
}

std::shared_ptr<UdpSession> UdpClientManager::createClient()
{
    UdpThread *thread = this->sessionThreads.pickMinThread();
    std::shared_ptr<UdpSession> session = std::make_shared<UdpSession>(thread);
    this->sessionThreads.addSession(session);
    session->moveToThread(thread);
    if (thread) {
        thread->addOne();
    }
    return session;
}

bool UdpClientManager::getIsRunning() const
{
    return isRunning;
}

NETAPI_NAMESPACE_END
