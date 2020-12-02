#include "udpsession.h"

#include "udpthread.h"

#include <QDataStream>

NETAPI_NAMESPACE_BEGIN

UdpSession::UdpSession(UdpThread *thread, QObject *parent) :
    QUdpSocket(parent),
    thread(thread)
{
    /**
     *     将所有对 连接（TcpSocket） 的操作，全部放到槽中，保证全部在单独的线程中处理，
     * 这样才能保证数据读写能收到正常的通知
     */
    connect(this, &UdpSession::signalConnect, this, &UdpSession::slotConnect);
    connect(this, &UdpSession::readyRead, this, &UdpSession::slotStartRead);
    connect(this, &UdpSession::signalDoWrite, this, &UdpSession::slotDoWrite);
    connect(this, &UdpSession::disconnected, this, &UdpSession::slotDisConnected);
    connect(this, &UdpSession::signalDoDisConnect, this, &UdpSession::slotDoDisconnect);

}

UdpSession::~UdpSession()
{
    disconnect(this, &UdpSession::signalConnect, this, &UdpSession::slotConnect);
    disconnect(this, &UdpSession::readyRead, this, &UdpSession::slotStartRead);
    disconnect(this, &UdpSession::signalDoWrite, this, &UdpSession::slotDoWrite);
    disconnect(this, &UdpSession::disconnected, this, &UdpSession::slotDisConnected);
    disconnect(this, &UdpSession::signalDoDisConnect, this, &UdpSession::slotDoDisconnect);

}

void UdpSession::disConnect()
{
    qDebug() << "UdpSession::Disconnect threadID:" << QThread::currentThreadId();
    emit this->signalDoDisConnect();
}

void UdpSession::doWrite(const QByteArray &data, const QString &targetHost, int targetPort)
{
    emit this->signalDoWrite(data, targetHost, targetPort);
}

void UdpSession::connectToServer(quint16 port)
{
    emit this->signalConnect(port);
}

void UdpSession::slotConnect(quint16 port)
{
    //监听端口
    this->bind(QHostAddress::Any, port);
}

void UdpSession::slotStartRead()
{
    while (this->hasPendingDatagrams()) {
        qDebug() << "UdpSession::SlotStartRead threadID:"<< QThread::currentThreadId();
        buffer.resize(this->pendingDatagramSize());
        this->readDatagram(buffer.data(), buffer.size());
        emit this->signalRead(buffer, buffer.size());
    }
}

void UdpSession::slotDisConnected()
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

void UdpSession::slotDoWrite(const QByteArray &data, const QString &targetHost, int targetPort)
{
    qDebug() << "UdpSession::SlotDoWrite threadID:"<< QThread::currentThreadId();
    QHostAddress targetAddr(targetHost);
    this->writeDatagram(data, targetAddr, targetPort);
}

void UdpSession::slotDoDisconnect()
{
    qDebug() << "UdpSession::SlotDoDisconnect threadID:"<< QThread::currentThreadId();
    this->abort();
}

NETAPI_NAMESPACE_END
