#include "FTcpConnection.h"

FTcpConnection::FTcpConnection(QObject *parent) :
    QObject(parent)
{
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
}

void FTcpConnection::updateServerProgress()  //更新进度条，接收数据
{

    QDataStream in(connection);
    in.setVersion(QDataStream::Qt_4_6);

    if(bytesReceived <= sizeof(qint64)*2)//Header
    {
        if((connection->bytesAvailable() >= sizeof(qint64)*2)&& (fileNameSize == 0))
        {
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }

        if((connection->bytesAvailable() >= fileNameSize)
                && (fileNameSize != 0))
        {
            in >> fileName;
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "open file error!";
                return;
            }
        }
        else return;
    }//Header End


    if(bytesReceived < totalBytes)
    {
        bytesReceived += connection->bytesAvailable();
        inBlock =  connection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    if(bytesReceived == totalBytes)
    {
        localFile->close();
        delete localFile;
        totalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;
    }
}

void FTcpConnection::displayError(QAbstractSocket::SocketError) //错误处理
{

}

/*void FTcpConnection::StartUpload(int num)  //实现文件大小等信息的发送
{
    QString UfileName=QFileDialog::getOpenFileName(this);
    SendlocalFile = new QFile(UfileName);
    if(!SendlocalFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }

    FTcpConnection *tmpu;
    QTcpSocket *uploadtcp=new QTcpSocket(this);

    tmpu=qobject_cast<FTcpConnection*>(sender());
    uploadtcp=

    tmpu->SendtotalBytes = tmpu->SendlocalFile->size();

    QDataStream sendOut(&tmpu->outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFileName = UfileName.right(UfileName.size() - UfileName.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0) << currentFileName;

    tmpu->SendtotalBytes += tmpu->outBlock.size();

    sendOut.device()->seek(0);
    sendOut<<tmpu->SendtotalBytes<<qint64((tmpu->outBlock.size() - sizeof(qint64)*2));

    tmpu->bytesToWrite = tmpu->SendtotalBytes - tcpClient->write(tmpu->outBlock);

    tmpu->outBlock.resize(0);
}

void FTcpConnection::updateClientProgress(qint64 numBytes) //更新进度条，实现文件的传送
{
    bytesWritten += (int)numBytes;

    if(bytesToWrite > 0)
    {
        outBlock = SendlocalFile->read(qMin(bytesToWrite,loadSize));
        bytesToWrite -= (int)tcpClient->write(outBlock);
        outBlock.resize(0);
    }
    else
    {
        SendlocalFile->close();
    }

    if(bytesWritten == totalBytes) //发送完毕
    {
        qDebug()<<"传送文件成功";
        SendlocalFile->close();
        delete SendlocalFile;
    }
}

void FTcpConnection::on_sendfile_clicked()
{
    emit startTransfer();
}
*/
