#include "tcpsessionthreads.h"

#include "tcpthread.h"
#include "tcpsession.h"

NETAPI_NAMESPACE_BEGIN

TcpSessionThreads::TcpSessionThreads()
{

}

TcpSessionThreads::~TcpSessionThreads()
{
    this->stop();
}

bool TcpSessionThreads::start(uint32_t threadNum)
{
    if (isRunning) {
        return true;
    }
    for (uint32_t i=0; i<threadNum; i++) {
        TcpThread *tcpThread = new TcpThread();
        threadList.push_back(tcpThread);
        tcpThread->start();
    }
    isRunning = true;
    return true;
}

void TcpSessionThreads::stop()
{
    if (!isRunning) {
        return;
    }
    {
        std::lock_guard<std::mutex> locker(this->lock);
        auto iterator = sessionList.begin();
        //关闭连接
        for (iterator = sessionList.begin(); iterator != sessionList.end(); ++iterator) {
            std::shared_ptr<TcpSession> session = iterator->second;
            if (session.get()) {
                disconnect(session.get(), &TcpSession::signalDisConnected, this, &TcpSessionThreads::slotSessionDisConnected);
                session.get()->disConnect();
            }
        }
    }
    for (TcpThread *tcpThread : this->threadList) {
        tcpThread->exit();
        tcpThread->wait();
    }
    for (TcpThread *tcpThread : this->threadList) {
        delete tcpThread;
    }
    this->threadList.clear();
    this->sessionList.clear();
    isRunning = false;
}

TcpThread *TcpSessionThreads::pickMinThread()
{
    TcpThread *tcpThread = nullptr;
    uint32_t minCount = 0;
    bool first = true;
    for (TcpThread *tempThread : this->threadList) {
        uint32_t tempCount = tempThread->sessionCount;
        if (first || tempCount < minCount) {
            minCount = tempCount;
            tcpThread = tempThread;
        }
        first = false;
    }
    return tcpThread;
}

std::vector<uint32_t> TcpSessionThreads::getSessionSize() const
{
    std::vector<uint32_t> vec;
    for (TcpThread *tcpThread : this->threadList) {
        vec.push_back(tcpThread->sessionCount);
    }
    return vec;
}

std::shared_ptr<TcpSession> TcpSessionThreads::createSession(qintptr handle)
{
    TcpThread *thread = pickMinThread();
    //std::make_shared<TcpSession>(thread)：thread就是参数
    std::shared_ptr<TcpSession> session = std::make_shared<TcpSession>(thread);
    connect(session.get(), &TcpSession::signalDisConnected, this, &TcpSessionThreads::slotSessionDisConnected);
    //将新创建的通信套接字描述符指定为参数socketdescriptor
    session->setSocketDescriptor(handle);
    session->moveToThread(thread);
    ++thread->sessionCount;
    //加锁
    std::lock_guard<std::mutex> locker(this->lock);
    //TODO：这里的意思是（TcpSession*， std::shared_ptr<TcpSession>）是键值对
    this->sessionList[session.get()] = session;
    return session;
}

void TcpSessionThreads::addSession(std::shared_ptr<TcpSession> session)
{
    std::lock_guard<std::mutex> locker(this->lock);
    //TODO：这里的意思是（TcpSession*， std::shared_ptr<TcpSession>）是键值对
    this->sessionList[session.get()] = session;
}

void TcpSessionThreads::slotSessionDisConnected(void *id)
{
    //枷锁
    std::lock_guard<std::mutex> locker(this->lock);
    auto iterator = sessionList.begin();
    iterator = sessionList.find(id);
    if (iterator != sessionList.end()) {
        sessionList.erase(iterator);
        qDebug()<< "SessionThreads::SlotSessionDisConnected" << QThread::currentThread();
    }

}

NETAPI_NAMESPACE_END
