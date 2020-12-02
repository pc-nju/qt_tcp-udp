#include "tcpsessioninfo.h"

NETAPI_NAMESPACE_BEGIN

TcpSessionInfo::TcpSessionInfo(std::shared_ptr<TcpSession> session)
{
    this->session = session;
    connect(this->session.get(), &TcpSession::connected, this, &TcpSessionInfo::signalConnect);
    connect(this->session.get(), &TcpSession::signalRead, this, &TcpSessionInfo::slotRead);
    connect(this->session.get(), &TcpSession::disconnected, this, &TcpSessionInfo::slotDisconnected);
}

TcpSessionInfo::~TcpSessionInfo()
{
    if (!this->session) {
        return;
    }
    disconnect(this->session.get(), &TcpSession::connected, this, &TcpSessionInfo::signalConnect);
    disconnect(this->session.get(), &TcpSession::signalRead, this, &TcpSessionInfo::slotRead);
    disconnect(this->session.get(), &TcpSession::disconnected, this, &TcpSessionInfo::slotDisconnected);
    this->session = nullptr;
}

void TcpSessionInfo::connectServer(const QString &host, quint16 port)
{
    if (this->session) {
        this->session->connectToServer(host, port);
    }
}

void TcpSessionInfo::disconnectServer()
{
    if (this->session) {
        this->session->disConnect();
    }
}

void TcpSessionInfo::doWrite(const QByteArray &data)
{
    if (this->session) {
        this->session->doWrite(data);
    }
}

void TcpSessionInfo::slotRead(const QByteArray &data, int size)
{
    emit this->signalRead(this, data, size);
}

void TcpSessionInfo::slotDisconnected()
{
    emit this->signalDisconnect();
    if (this->onDisConnected) {
        this->onDisConnected(this);
    }
}

NETAPI_NAMESPACE_END
