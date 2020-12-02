#include "tcpserver.h"

NETAPI_NAMESPACE_BEGIN

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}

TcpServer::~TcpServer()
{
    this->stop();
}

bool TcpServer::start(TcpServerData &conf)
{
    if (isRunning) {
        return true;
    }
    //验证数据
    conf.verify();
    //启动线程池
    sessionThreads.start(conf.threadNum);
    //监听端口
    if (!this->listen(QHostAddress::Any, (quint16)conf.port)) {
        return false;
    }
    isRunning = true;
    qDebug() << "TcpServer::Start threadID:"<< QThread::currentThreadId();
    return true;
}

void TcpServer::stop()
{
    if (!isRunning) {
        return;
    }
    //关闭监听
    this->close();
    //关闭线程池
    sessionThreads.stop();
    isRunning = false;
}

std::vector<uint32_t> TcpServer::getSessionSize() const
{
    return sessionThreads.getSessionSize();
}

void TcpServer::incomingConnection(qintptr handle)
{
    qDebug() << "TcpServer::incomingConnection threadID:"<< QThread::currentThreadId();
    std::shared_ptr<TcpSession> session = sessionThreads.createSession(handle);
    if (this->onAccepted) {
        this->onAccepted(session);
    }
}

bool TcpServer::getIsRunning() const
{
    return isRunning;
}

NETAPI_NAMESPACE_END
