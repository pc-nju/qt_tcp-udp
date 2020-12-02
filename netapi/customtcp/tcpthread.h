#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include "../netapinamespace.h"

#include <QThread>
#include <atomic>

NETAPI_NAMESPACE_BEGIN

class TcpThread : public QThread
{
public:
    explicit TcpThread(QObject *parent = Q_NULLPTR);
    ~TcpThread();
    virtual void run() override;

    void addOne();

    //会话数
    std::atomic_uint_fast32_t sessionCount{0};
};

NETAPI_NAMESPACE_END

#endif // TCPTHREAD_H
