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
    {

        if((NowTcp->bytesAvailable() >= sizeof(qint64)*2)&& (tmpc->fileNameSize == 0))
        {
            in >> tmpc->totalBytes >> tmpc->fileNameSize;
            tmpc->bytesReceived += sizeof(qint64) * 2;
        }

        if((NowTcp->bytesAvailable() >= tmpc->fileNameSize)
                && (tmpc->fileNameSize != 0))
        {
            in >> fileName;
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
    {
        tmpc->bytesReceived += NowTcp->bytesAvailable();
        inBlock = NowTcp->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    if(tmpc->bytesReceived == tmpc->totalBytes)
    {
        localFile->close();
        delete localFile;
        tmpc->totalBytes = 0;
        tmpc->bytesReceived = 0;
        tmpc->fileNameSize = 0;
    }
}

void FTcpConnection::displayError(QAbstractSocket::SocketError) //错误处理
{

}
