#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include "../netapinamespace.h"

#include <QThread>
#include <atomic>

NETAPI_NAMESPACE_BEGIN

class UdpThread : public QThread
{
public:
    explicit UdpThread(QObject *parent = Q_NULLPTR);
    ~UdpThread();
    virtual void run() override;

    void addOne();

    //会话数
    std::atomic_uint_fast32_t sessionCount{0};
};

NETAPI_NAMESPACE_END

#endif // UDPTHREAD_H
