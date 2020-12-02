#include "tcpthread.h"

NETAPI_NAMESPACE_BEGIN

TcpThread::TcpThread(QObject *parent) : QThread(parent)
{

}

TcpThread::~TcpThread()
{

}

void TcpThread::run()
{
    //添加进事件循环，这样就能确保线程不执行完，且加进来的Session能够顺利接收自己的事件
    this->exec();
}

void TcpThread::addOne()
{
    sessionCount++;
}

NETAPI_NAMESPACE_END
