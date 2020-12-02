#include "dialogudpclient.h"
#include "ui_dialogudpclient.h"

DialogUdpClient::DialogUdpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUdpClient)
{
    ui->setupUi(this);
    initStyle();
}

DialogUdpClient::~DialogUdpClient()
{
    if (this->sessionInfo) {
        disconnect(sessionInfo, &NetAPI::UdpSessionInfo::signalConnect, this, &DialogUdpClient::slotConnect);
        disconnect(sessionInfo, &NetAPI::UdpSessionInfo::signalRead, this, &DialogUdpClient::slotRead);
        disconnect(sessionInfo, &NetAPI::UdpSessionInfo::signalDisconnect, this, &DialogUdpClient::slotDisconnect);
        delete this->sessionInfo;
        this->sessionInfo = nullptr;
    }
    if (this->clientManager) {
        this->clientManager->stop();
        delete this->clientManager;
        this->clientManager = nullptr;
    }
    delete ui;
}

void DialogUdpClient::initStyle()
{
    //网络传输部分
    clientManager = NetAPI::NetApiFactory::createUdpClientManager();
    NetAPI::UdpServerData serverData;
    serverData.threadNum = 1;
    //这里必须先启动，否则在初始化 SessionInfo 时，找不到线程
    clientManager->start(serverData);
    this->sessionInfo = new NetAPI::UdpSessionInfo(clientManager->createClient());
    connect(sessionInfo, &NetAPI::UdpSessionInfo::signalConnect, this, &DialogUdpClient::slotConnect);
    connect(sessionInfo, &NetAPI::UdpSessionInfo::signalRead, this, &DialogUdpClient::slotRead);
    connect(sessionInfo, &NetAPI::UdpSessionInfo::signalDisconnect, this, &DialogUdpClient::slotDisconnect);
}

void DialogUdpClient::writeLog(const QString &msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void DialogUdpClient::slotConnect()
{
    this->writeLog("Connected Success!");
}

void DialogUdpClient::slotRead(NetAPI::UdpSessionInfo *sessionInfo, const QByteArray &data, int size)
{
    Q_UNUSED(sessionInfo);
    Q_UNUSED(size);
    QString msg = data;
    ui->plainTextEdit->appendPlainText(msg);
}

void DialogUdpClient::slotDisconnect()
{
    this->writeLog("DisConnected!");
}

void DialogUdpClient::on_btnConnect_clicked()
{
    int port = ui->sbBindPort->value();
    if (this->sessionInfo) {
        this->sessionInfo->connectServer(port);
        ui->plainTextEdit->appendPlainText("Bind Success!");
    }
}

void DialogUdpClient::on_btnDisConnect_clicked()
{
    if (this->sessionInfo) {
        this->sessionInfo->disconnectServer();
        ui->plainTextEdit->appendPlainText("UnBind Success!");
    }
}

void DialogUdpClient::on_btnSend_clicked()
{
    QString msg = ui->leContent->toPlainText().trimmed();
    if (this->sessionInfo) {
        QByteArray buffer = msg.toUtf8();
        QString targetIp = ui->leHost->text().trimmed();
        int targetPort = ui->sbPort->value();
        //发送数据
        this->sessionInfo->doWrite(buffer, targetIp, targetPort);
    }
}

void DialogUdpClient::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}
