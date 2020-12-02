#include "udpsessioninfo.h"

NETAPI_NAMESPACE_BEGIN


UdpSessionInfo::UdpSessionInfo(std::shared_ptr<UdpSession> session)
{
    this->session = session;
    connect(this->session.get(), &UdpSession::connected, this, &UdpSessionInfo::signalConnect);
    connect(this->session.get(), &UdpSession::signalRead, this, &UdpSessionInfo::slotRead);
    connect(this->session.get(), &UdpSession::disconnected, this, &UdpSessionInfo::slotDisconnected);
}

UdpSessionInfo::~UdpSessionInfo()
{
    if (!this->session) {
        return;
    }
    disconnect(this->session.get(), &UdpSession::connected, this, &UdpSessionInfo::signalConnect);
    disconnect(this->session.get(), &UdpSession::signalRead, this, &UdpSessionInfo::slotRead);
    disconnect(this->session.get(), &UdpSession::disconnected, this, &UdpSessionInfo::slotDisconnected);
    this->session = nullptr;
}

void UdpSessionInfo::connectServer(quint16 port)
{
    if (this->session) {
        this->session->connectToServer(port);
    }
}

void UdpSessionInfo::disconnectServer()
{
    if (this->session) {
        this->session->disConnect();
    }
}

void UdpSessionInfo::doWrite(const QByteArray &buffer, const QString &targetIp, quint16 port)
{
    if (this->session) {
        this->session->doWrite(buffer, targetIp, port);
    }
}

void UdpSessionInfo::slotRead(const QByteArray &data, int size)
{
    emit this->signalRead(this, data, size);
}

void UdpSessionInfo::slotDisconnected()
{
    emit this->signalDisconnect();
    if (this->onDisConnected) {
        this->onDisConnected(this);
    }
}

NETAPI_NAMESPACE_END
