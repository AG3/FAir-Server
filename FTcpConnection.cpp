#include "FTcpConnection.h"

FTcpConnection::FTcpConnection(QObject *parent) :
    QObject(parent)
{
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
}

void FTcpConnection::StartRecive()
{
    emit ConnectionReady();
}

void FTcpConnection::updateServerProgress()  //更新进度条，接收数据
{
    FTcpConnection *tmpc;
    QTcpSocket *NowTcp=new QTcpSocket(this);
    tmpc=qobject_cast<FTcpConnection*>(sender());
    NowTcp=tmpc->connection;

    QDataStream in(NowTcp);
    in.setVersion(QDataStream::Qt_4_6);

    if(tmpc->bytesReceived <= sizeof(qint64)*2)
    { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息

        if((NowTcp->bytesAvailable() >= sizeof(qint64)*2)&& (tmpc->fileNameSize == 0))
        { //接收数据总大小信息和文件名大小信息
            in >> tmpc->totalBytes >> tmpc->fileNameSize;
            tmpc->bytesReceived += sizeof(qint64) * 2;
        }

        if((NowTcp->bytesAvailable() >= tmpc->fileNameSize)
                && (tmpc->fileNameSize != 0))
        {  //接收文件名，并建立文件
            in >> fileName;
            //ui->serverStatusLabel->setText(tr("接收文件 %1 …").arg(fileName));
            tmpc->bytesReceived += tmpc->fileNameSize;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "open file error!";
                return;
            }
        }

        else return;
    }


    if(tmpc->bytesReceived < tmpc->totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件
        tmpc->bytesReceived += NowTcp->bytesAvailable();
        inBlock = NowTcp->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    //ui->serverProgressBar->setMaximum(tmpc->totalBytes);
    //ui->serverProgressBar->setValue(tmpc->bytesReceived);
    //更新进度条
    if(tmpc->bytesReceived == tmpc->totalBytes)
    { //接收数据完成时
        //NowTcp->reset();
        localFile->close();
        //ui->startButton->setEnabled(true);
        delete localFile;
        tmpc->totalBytes = 0;
        tmpc->bytesReceived = 0;
        tmpc->fileNameSize = 0;
        //ui->serverStatusLabel->setText(tr("接收文件 %1 成功！").arg(fileName));
    }
}

void FTcpConnection::displayError(QAbstractSocket::SocketError) //错误处理
{
    //qDebug() << NowTcp->errorString();
    //NowTcp->close();
    //ui->serverProgressBar->reset();
    //ui->serverStatusLabel->setText(tr("服务端就绪"));
    //ui->startButton->setEnabled(true);
}
