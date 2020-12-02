#ifndef NETAPIFACTORY_H
#define NETAPIFACTORY_H

#include "netapinamespace.h"
#include "customtcp/tcpclientmanager.h"
#include "customtcp/tcpserver.h"
#include "customtcp/tcpsessioninfolist.h"
#include "customudp/udpclientmanager.h"
#include "customudp/udpsessioninfo.h"
#include "utils/jsonutils.h"

NETAPI_NAMESPACE_BEGIN
class NetApiFactory
{
public:
    static TcpClientManager *createClientManager();
    static TcpServer *createServer();
    static UdpClientManager *createUdpClientManager();
};
NETAPI_NAMESPACE_END
#endif // NETAPIFACTORY_H
