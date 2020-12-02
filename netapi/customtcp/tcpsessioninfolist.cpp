#include "tcpsessioninfolist.h"

#include <functional>

NETAPI_NAMESPACE_BEGIN

TcpSessionInfoList::TcpSessionInfoList()
{
    
}

TcpSessionInfoList::~TcpSessionInfoList()
{
    this->clear();
}

TcpSessionInfo *TcpSessionInfoList::newSessionInfo(std::shared_ptr<TcpSession> &session)
{
    std::lock_guard<std::mutex> locker(this->lock);
    TcpSessionInfo *info = new TcpSessionInfo(session);
    this->sessionInfos.push_back(info);
    info->onDisConnected = std::bind(&TcpSessionInfoList::sessionDisconnected, this, std::placeholders::_1);
    return info;
}

void TcpSessionInfoList::clear()
{
    std::lock_guard<std::mutex> locker(this->lock);
    for (TcpSessionInfo *sessionInfo : sessionInfos) {
        if (sessionInfo) {
            delete sessionInfo;
        }
    }
    this->sessionInfos.clear();
}

TcpSessionInfo *TcpSessionInfoList::findById(const void *id)
{
    if (!id) {
        return nullptr;
    }
    std::lock_guard<std::mutex> locker(this->lock);
    for (size_t i=0; i<this->sessionInfos.size(); ++i) {
        TcpSessionInfo *sessionInfo = this->sessionInfos.at(i);
        if (sessionInfo == id) {
            return sessionInfo;
        }
    }
    return nullptr;
}

void TcpSessionInfoList::sessionDisconnected(void *id)
{
    TcpSessionInfo *sessionInfo = nullptr;
    std::lock_guard<std::mutex> locker(this->lock);
    std::vector<TcpSessionInfo*>::iterator it;
    for (it = this->sessionInfos.begin(); it != this->sessionInfos.end(); ++it) {
        sessionInfo = *it;
        if (sessionInfo == id) {
            it = this->sessionInfos.erase(it);
            sessionInfo->onDisConnected = nullptr;
            delete sessionInfo;
            return;
        }
    }
}

NETAPI_NAMESPACE_END
