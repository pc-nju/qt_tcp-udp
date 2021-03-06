﻿#include "dialogtcpserver.h"
#include "ui_dialogtcpserver.h"

#include <functional>
#include <QDebug>

DialogTcpServer::DialogTcpServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTcpServer)
{
    ui->setupUi(this);
    initStyle();
}

DialogTcpServer::~DialogTcpServer()
{
    if (server) {
        server->stop();
        delete server;
    }
    server = nullptr;
    if (this->localFile) {
        delete this->localFile;
        this->localFile = nullptr;
    }
    delete ui;
}

void DialogTcpServer::initStyle()
{
    server = NetAPI::NetApiFactory::createServer();
    sessionList = new NetAPI::TcpSessionInfoList();
    server->onAccepted = [this](std::shared_ptr<NetAPI::TcpSession> &tcpSession){
        NetAPI::TcpSessionInfo *sessionInfo = this->sessionList->newSessionInfo(tcpSession);
        connect(sessionInfo, &NetAPI::TcpSessionInfo::signalConnect, this, &DialogTcpServer::slotConnect);
        connect(sessionInfo, &NetAPI::TcpSessionInfo::signalRead, this, &DialogTcpServer::slotRead);
        connect(sessionInfo, &NetAPI::TcpSessionInfo::signalBytesWritten, this, &DialogTcpServer::slotBytesWritten);
        connect(sessionInfo, &NetAPI::TcpSessionInfo::signalDisconnect, this, &DialogTcpServer::slotDisconnect);
        ui->plainTextEdit->appendPlainText("Accept One!");
    };
}

void DialogTcpServer::slotRead(NetAPI::TcpSessionInfo *sessionInfo, const QByteArray &data, qint64 size)
{
    Q_UNUSED(sessionInfo);
//    //----------------- 读数据 start ---------------//
//    QDataStream in(data);
//    //设置数据流版本
//    in.setVersion(QDataStream::Qt_5_9);
//    //首先判断数据大小是否为0，若0，则需要先读文件大小
//    if (blockSize == 0) {
//        //判断接收的数据是否有两字节，也就是文件的大小信息，若没有，则返回，继续接收数据
//        if (size < (int)sizeof(qint16)) {
//            return;
//        }
//        //读取文件的大小信息
//        in >> blockSize;
//    }
//    //判断接收的数据大小是否达到文件的大小，若无，则返回
//    if (size < blockSize) {
//        return;
//    }
//    //读取信息
//    in >> message;
//    blockSize = 0; //在读取完毕后，必须将数据大小重置为0，否则下次将无法接收数据
//    ui->plainTextEdit->appendPlainText(message);

//    //----------------- 读数据 end ---------------//

//    //----------------- 写数据 start ---------------//
//    //再将内容发回去
//    QDataStream out(&outBlock, QIODevice::WriteOnly);
//    //设置版本（客户端和服务端必须保持一致）
//    out.setVersion(QDataStream::Qt_5_9);
//    //添加一个 qint16 大小的空间，用于后面放置文件的大小信息
//    out << (qint16)0;
//    //写入正文
//    out << message;
//    //返回到 block 的开始，加入实际的文件大小信息
//    out.device()->seek(0);
//    //实际的文件大小
//    qint16 len = (qint16) (outBlock.size() - sizeof(qint16));
//    //写入文件大小
//    out << len;
//    //发送
//    sessionInfo->doWrite(outBlock);
//    //重置缓冲区
//    outBlock.resize(0);
//    //----------------- 写数据 end ---------------//

    //接收文件
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_5_9);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构
    if (bytesReceived <= sizeof(qint64) * 2) {
        if ((size >= sizeof(qint64) * 2) && fileNameSize == 0) {
            // 接收数据总大小信息和文件名大小信息
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if (size >= fileNameSize && fileNameSize != 0) {
            //接收文件名，并建立文件
            in >> fileName;
            qDebug() << QString("接收文件 %1 …").arg(fileName);
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly)) {
                qDebug() << "server: open file error!";
                return;
            }
        } else {
            return;
        }
    } else {
        //如果接收的数据小于总数据，那么写入文件
        if (bytesReceived < totalBytes) {
            bytesReceived += size;
            localFile->write(data);
        }
        //设置进度条

        if (bytesReceived == totalBytes) {
            localFile->close();
            bytesReceived = 0;
            fileNameSize = 0;
            qDebug() << QString("接收文件 %1 成功").arg(fileName);
        }
    }
}

void DialogTcpServer::slotBytesWritten(qint64 numBytes)
{
    qDebug() << "tcp serer";
    qDebug() << numBytes;
}

void DialogTcpServer::slotConnect()
{
    ui->plainTextEdit->appendPlainText("Connected!");
}

void DialogTcpServer::slotDisconnect()
{
    ui->plainTextEdit->appendPlainText("Disconnected!");
}

void DialogTcpServer::on_btStart_clicked()
{
    int port = ui->sbPort->value();
    NetAPI::TcpServerData serverData;
    serverData.port = port;
    if (server->start(serverData)) {
        ui->plainTextEdit->appendPlainText("Start Success!");
    } else {
        ui->plainTextEdit->appendPlainText("Could not start server!");
    }
}

void DialogTcpServer::on_btStop_clicked()
{
    if (!this->server->getIsRunning()) {
        return;
    }
    this->server->stop();
    ui->plainTextEdit->appendPlainText("Server Stop!");
}

void DialogTcpServer::on_btClear_clicked()
{
    ui->plainTextEdit->clear();
}
