#include "dialogtcpclient.h"
#include "ui_dialogtcpclient.h"

#include <QFileDialog>

DialogTcpClient::DialogTcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTcpClient)
{
    ui->setupUi(this);
    initStyle();
}

DialogTcpClient::~DialogTcpClient()
{
    if (this->sessionInfo) {
        disconnect(sessionInfo, &NetAPI::TcpSessionInfo::signalConnect, this, &DialogTcpClient::slotConnect);
        disconnect(sessionInfo, &NetAPI::TcpSessionInfo::signalRead, this, &DialogTcpClient::slotRead);
        disconnect(sessionInfo, &NetAPI::TcpSessionInfo::signalBytesWritten, this, &DialogTcpClient::slotBytesWritten);
        disconnect(sessionInfo, &NetAPI::TcpSessionInfo::signalDisconnect, this, &DialogTcpClient::slotDisconnect);
        delete this->sessionInfo;
        this->sessionInfo = nullptr;
    }
    if (this->clientManager) {
        this->clientManager->stop();
        delete this->clientManager;
        this->clientManager = nullptr;
    }
    if (this->localFile) {
        delete this->localFile;
        this->localFile = nullptr;
    }
    delete ui;
}

void DialogTcpClient::initStyle()
{
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    payloadSize = 64 * 1024;

    ui->btnSendFile->setEnabled(false);

    clientManager = NetAPI::NetApiFactory::createClientManager();
    clientManager->start(1);
    this->sessionInfo = new NetAPI::TcpSessionInfo(clientManager->createClient());
    connect(sessionInfo, &NetAPI::TcpSessionInfo::signalConnect, this, &DialogTcpClient::slotConnect);
    connect(sessionInfo, &NetAPI::TcpSessionInfo::signalRead, this, &DialogTcpClient::slotRead);
    connect(sessionInfo, &NetAPI::TcpSessionInfo::signalBytesWritten, this, &DialogTcpClient::slotBytesWritten);
    connect(sessionInfo, &NetAPI::TcpSessionInfo::signalDisconnect, this, &DialogTcpClient::slotDisconnect);
}

void DialogTcpClient::writeLog(const QString &msg)
{
    ui->plainTextEdit->appendPlainText(msg);
}

void DialogTcpClient::slotConnect()
{
    writeLog("Connected Success!");
}

void DialogTcpClient::slotRead(NetAPI::TcpSessionInfo *sessionInfo, const QByteArray &data, qint64 size)
{
    Q_UNUSED(sessionInfo);
    //----------------- 读数据 start ---------------//
    QDataStream in(data);
    //设置数据流版本
    in.setVersion(QDataStream::Qt_5_9);
    //首先判断数据大小是否为0，若0，则需要先读文件大小
    if (blockSize == 0) {
        //判断接收的数据是否有两字节，也就是文件的大小信息，若没有，则返回，继续接收数据
        if (size < (int)sizeof(qint16)) {
            return;
        }
        //读取文件的大小信息
        in >> blockSize;
    }
    //判断接收的数据大小是否达到文件的大小，若无，则返回
    if (size < blockSize) {
        return;
    }
    //读取信息
    in >> message;
    blockSize = 0; //在读取完毕后，必须将数据大小重置为0，否则下次将无法接收数据
    ui->plainTextEdit->appendPlainText(message);
    //----------------- 读数据 end ---------------//
}

void DialogTcpClient::slotBytesWritten(qint64 numBytes)
{
    bytesToWrite = totalBytes - numBytes;
    bytesWritten += numBytes;
    if (bytesToWrite > 0) {
        outBlock = localFile->read(qMin(bytesToWrite, payloadSize));
        this->sessionInfo->doWrite(outBlock);
        outBlock.resize(0);
    } else {
        localFile->close();
    }

    ui->progressBar->setValue(bytesWritten);
    //传输完成
    if (bytesWritten == totalBytes) {
        //关闭文件
        localFile->close();
        //这里可以设置传输完成的相关操作
        bytesWritten = 0;
    }
}

void DialogTcpClient::slotDisconnect()
{
    this->writeLog("Disconnected!");
}

void DialogTcpClient::on_btnConnect_clicked()
{
    int port = ui->sbPort->value();
    QString ip = ui->leHost->text().trimmed();
    if (this->sessionInfo) {
        this->sessionInfo->connectServer(ip, port);
    }
}

void DialogTcpClient::on_btnDisconnect_clicked()
{
    this->clientManager->stop();
}

void DialogTcpClient::on_btnSend_clicked()
{
    QString message = ui->leContent->toPlainText().trimmed();
    if (this->sessionInfo) {
        //----------------- 写数据 start ---------------//
        //再将内容发回去
        QDataStream out(&outBlock, QIODevice::WriteOnly);
        //设置版本（客户端和服务端必须保持一致）
        out.setVersion(QDataStream::Qt_5_9);
        //添加一个 qint16 大小的空间，用于后面放置文件的大小信息
        out << (qint16)0;
        //写入正文
        out << message;
        //返回到 block 的开始，加入实际的文件大小信息
        out.device()->seek(0);
        //实际的文件大小
        qint16 len = (qint16) (outBlock.size() - sizeof(qint16));
        //写入文件大小
        out << len;
        //发送
        this->sessionInfo->doWrite(outBlock);
        //读取完毕后，重置缓冲区
        outBlock.resize(0);
        //----------------- 写数据 end ---------------//
    }
}

void DialogTcpClient::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}

void DialogTcpClient::on_btnSelectFile_clicked()
{
    this->fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        ui->btnSendFile->setEnabled(true);
        ui->leFilePath->setText(fileName);
    }
}

void DialogTcpClient::on_btnSendFile_clicked()
{
    ui->btnSendFile->setEnabled(false);
    bytesWritten = 0;
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly)) {
        qDebug() << "client: open file error!";
        return;
    }
    totalBytes = localFile->size();
    QDataStream out(&outBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    //文件总大小、文件名大小、文件名
    out << qint64(0) << qint64(0) << currentFileName;
    totalBytes += outBlock.size();
    ui->progressBar->setMaximum(totalBytes);
    out.device()->seek(0);
    //打包数据
    out << totalBytes << qint64(outBlock.size() - sizeof(qint64)*2);
    this->sessionInfo->doWrite(outBlock);
    outBlock.resize(0);
}
