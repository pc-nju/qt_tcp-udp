#ifndef SESSIONINFOLIST_H
#define SESSIONINFOLIST_H

#include "../netapinamespace.h"

#include <mutex>
#include "tcpsessioninfo.h"

NETAPI_NAMESPACE_BEGIN

class TcpSessionInfoList
{
public:
    TcpSessionInfoList();
    ~TcpSessionInfoList();
    
    TcpSessionInfo *newSessionInfo(std::shared_ptr<TcpSession> &session);
    void clear();
    TcpSessionInfo* findById(const void *id);
    
private:
    void sessionDisconnected(void *id);
    
private:
    std::mutex lock;
    std::vector<TcpSessionInfo*> sessionInfos;
};
NETAPI_NAMESPACE_END
#endif // SESSIONINFOLIST_H
