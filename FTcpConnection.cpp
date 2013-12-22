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

        if((connection->bytesAvailable() >= fileNameSize)&& (fileNameSize != 0))
        {
            in >> fileName;
            bytesReceived += fileNameSize;
            if(userID.isEmpty())
            {
                localFile = new QFile(fileName);
            }
            else
            {
                qDebug()<<"mkdir"<<userID;
                QDir folder;
                folder.mkdir(userID);
                localFile = new QFile(userID+"/"+fileName);
            }
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

        if(fileName=="userDiscern")
        {
            localFile = new QFile(fileName);
            localFile->open(QIODevice::ReadWrite);
            userID=localFile->readLine(64);
            userID=userID.simplified();
            localFile->remove();
            localFile->close();
            delete localFile;
            qDebug()<<userID;
        }
        else if(fileName=="sync")
        {
            localFile = new QFile(userID+"/"+"fileListInfo");
            if(!localFile->open(QIODevice::ReadOnly))
            {

            }
        }
    }
}

void FTcpConnection::displayError(QAbstractSocket::SocketError) //错误处理
{

}

void FTcpConnection::StartUpload(QString toSend)  //实现文件大小等信息的发送
{
    SendlocalFile = new QFile(toSend);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = SendlocalFile->size();

    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFileName = toSend.right(toSend.size() - toSend.lastIndexOf('/')-1);
    sendOut << qint64(0) << qint64(0) << currentFileName;

    totalBytes += outBlock.size();

    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));

    bytesWritten+=connection->write(outBlock);
    //qDebug()<<totalBytes;
    while(bytesWritten<totalBytes)
    {
        //qDebug()<<bytesWritten;
        outBlock.clear();
        outBlock = SendlocalFile->read(loadSize);
        connection->write(outBlock);
        bytesWritten += outBlock.size();

    }
    if(bytesWritten == totalBytes) //发送完毕
    {
        qDebug()<<"Send file success";
        SendlocalFile->close();
        delete SendlocalFile;
    }
    outBlock.resize(0);
}
