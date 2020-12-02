#include "udpsessionthreads.h"

#include "udpthread.h"
#include "udpsession.h"

NETAPI_NAMESPACE_BEGIN

UdpSessionThreads::UdpSessionThreads()
{

}

UdpSessionThreads::~UdpSessionThreads()
{
    this->stop();
}

bool UdpSessionThreads::start(uint32_t threadNum)
{
    if (isRunning) {
        return true;
    }
    for (uint32_t i=0; i<threadNum; i++) {
        UdpThread *udpThread = new UdpThread();
        threadList.push_back(udpThread);
        udpThread->start();
    }
    isRunning = true;
    return true;
}

void UdpSessionThreads::stop()
{
    if (!isRunning) {
        return;
    }
    {
        std::lock_guard<std::mutex> locker(this->lock);
        auto iterator = sessionList.begin();
        //关闭连接
        for (iterator = sessionList.begin(); iterator != sessionList.end(); ++iterator) {
            std::shared_ptr<UdpSession> session = iterator->second;
            if (session.get()) {
                disconnect(session.get(), &UdpSession::signalDisConnected, this, &UdpSessionThreads::slotSessionDisConnected);
                session.get()->disConnect();
            }
        }
    }
    for (UdpThread *udpThread : this->threadList) {
        udpThread->exit();
        udpThread->wait();
    }
    for (UdpThread *udpThread : this->threadList) {
        delete udpThread;
    }
    this->threadList.clear();
    this->sessionList.clear();
    isRunning = false;
}

UdpThread *UdpSessionThreads::pickMinThread()
{
    UdpThread *udpThread = nullptr;
    uint32_t minCount = 0;
    bool first = true;
    for (UdpThread *tempThread : this->threadList) {
        uint32_t tempCount = tempThread->sessionCount;
        if (first || tempCount < minCount) {
            minCount = tempCount;
            udpThread = tempThread;
        }
        first = false;
    }
    return udpThread;
}

std::vector<uint32_t> UdpSessionThreads::getSessionSize() const
{
    std::vector<uint32_t> vec;
    for (UdpThread *udpThread : this->threadList) {
        vec.push_back(udpThread->sessionCount);
    }
    return vec;
}

std::shared_ptr<UdpSession> UdpSessionThreads::createSession()
{
    UdpThread *thread = pickMinThread();
    //std::make_shared<UdpSession>(thread)：thread就是参数
    std::shared_ptr<UdpSession> session = std::make_shared<UdpSession>(thread);
    connect(session.get(), &UdpSession::signalDisConnected, this, &UdpSessionThreads::slotSessionDisConnected);
    session->moveToThread(thread);
    ++thread->sessionCount;
    //加锁
    std::lock_guard<std::mutex> locker(this->lock);
    //TODO：这里的意思是（TcpSession*， std::shared_ptr<TcpSession>）是键值对
    this->sessionList[session.get()] = session;
    return session;
}

void UdpSessionThreads::addSession(std::shared_ptr<UdpSession> session)
{
    std::lock_guard<std::mutex> locker(this->lock);
    //TODO：这里的意思是（TcpSession*， std::shared_ptr<UdpSession>）是键值对
    this->sessionList[session.get()] = session;
}

void UdpSessionThreads::slotSessionDisConnected(void *id)
{
    //枷锁
    std::lock_guard<std::mutex> locker(this->lock);
    auto iterator = sessionList.begin();
    iterator = sessionList.find(id);
    if (iterator != sessionList.end()) {
        sessionList.erase(iterator);
        qDebug()<< "UdpSessionThreads::SlotSessionDisConnected" << QThread::currentThread();
    }
}

NETAPI_NAMESPACE_END
