#include "tcpsession.h"

#include "tcpthread.h"

NETAPI_NAMESPACE_BEGIN

TcpSession::TcpSession(TcpThread *thread, QObject *parent) :
    QTcpSocket(parent),
    thread(thread)
{
    /**
     *     将所有对 连接（TcpSocket） 的操作，全部放到槽中，保证全部在单独的线程中处理，
     * 这样才能保证数据读写能收到正常的通知
     */
    connect(this, &TcpSession::signalConnect, this, &TcpSession::slotConnect);
    connect(this, &TcpSession::readyRead, this, &TcpSession::slotStartRead);
    connect(this, &TcpSession::disconnected, this, &TcpSession::slotDisConnected);
    connect(this, &TcpSession::signalDoDisConnect, this, &TcpSession::slotDoDisconnect);
    connect(this, &TcpSession::signalDoWrite, this, &TcpSession::slotDoWrite);
}

TcpSession::~TcpSession()
{
    disconnect(this, &TcpSession::signalConnect, this, &TcpSession::slotConnect);
    disconnect(this, &TcpSession::readyRead, this, &TcpSession::slotStartRead);
    disconnect(this, &TcpSession::disconnected, this, &TcpSession::slotDisConnected);
    disconnect(this, &TcpSession::signalDoDisConnect, this, &TcpSession::slotDoDisconnect);
    disconnect(this, &TcpSession::signalDoWrite, this, &TcpSession::slotDoWrite);
}

void TcpSession::disConnect()
{
    qDebug() << "TcpSession::Disconnect threadID:" << QThread::currentThreadId();
    emit this->signalDoDisConnect();
}

void TcpSession::doWrite(const QByteArray &data)
{
    emit this->signalDoWrite(data);
}

void TcpSession::connectToServer(const QString &host, quint16 port)
{
    emit this->signalConnect(host, port);
}

void TcpSession::slotConnect(const QString &host, quint16 port)
{
    this->connectToHost(host, port);
}

void TcpSession::slotStartRead()
{
    qDebug() << "TcpSession::SlotStartRead threadID:"<< QThread::currentThreadId();
    qint64 size = this->bytesAvailable();
    buffer = this->readAll();
    emit this->signalRead(buffer, size);
}

void TcpSession::slotDisConnected()
{
    if (thread) {
        thread->sessionCount--;
    }
    //通知会话断开连接
    if(onDisconnected) {
        onDisconnected(this);
    }
    emit this->signalDisConnected(this);
}

void TcpSession::slotDoWrite(const QByteArray &data)
{
    qDebug() << "TcpSession::SlotDoWrite threadID:"<< QThread::currentThreadId();
    this->write(data);
}

void TcpSession::slotDoDisconnect()
{
    qDebug() << "TcpSession::SlotDoDisconnect threadID:"<< QThread::currentThreadId();
    this->disconnectFromHost();
}

NETAPI_NAMESPACE_END
