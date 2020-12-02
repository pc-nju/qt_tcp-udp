#include "netapifactory.h"
NETAPI_NAMESPACE_BEGIN

TcpClientManager *NetApiFactory::createClientManager()
{
    return new TcpClientManager();
}

TcpServer *NetApiFactory::createServer()
{
    return new TcpServer();
}

UdpClientManager *NetApiFactory::createUdpClientManager()
{
    return new UdpClientManager();
}

NETAPI_NAMESPACE_END
