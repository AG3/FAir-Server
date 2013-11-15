#ifndef FTCPCONNECTION_H
#define FTCPCONNECTION_H

#include <QObject>
#include"QTcpSocket"
#include"QFile"
#include"QByteArray"
#include"QDataStream"
#include"QFileDialog"

class FTcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit FTcpConnection(QObject *parent = 0);
    QTcpSocket *connection;
    qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    QString fileName;   //存放文件名
    QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区

    QFile *SendlocalFile;  //要发送的文件
    qint64 SendtotalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据

    //void updateClientProgress(qint64 numBytes);


public slots:
    void updateServerProgress();  //更新进度条，接收数据
    void displayError(QAbstractSocket::SocketError socketError);
    //void on_sendfile_clicked();
    //void StartUpload(int num);
};

#endif // FTCPCONNECTION_H
