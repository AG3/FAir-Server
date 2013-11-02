#ifndef FTCPCONNECTION_H
#define FTCPCONNECTION_H

#include <QObject>
#include"QTcpSocket"
#include"QFile"
#include"QByteArray"
#include"QDataStream"
#include"QListWidgetItem"
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

signals:
    void ConnectionReady();

public slots:
    void StartRecive();
    void updateServerProgress();  //更新进度条，接收数据
    void displayError(QAbstractSocket::SocketError socketError);
    void connectionselected(QListWidgetItem*);
};

#endif // FTCPCONNECTION_H
