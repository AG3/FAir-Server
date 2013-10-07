#ifndef SERVERBASE_H
#define SERVERBASE_H

#include <QMainWindow>
#include "QTcpServer"
#include "QTcpSocket"
#include "QFile"
#include "QByteArray"
#include <QMutex>
namespace Ui {
class ServerBase;
}

class ServerBase : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ServerBase(QWidget *parent = 0);
    //void Finit();//Start Network connection and readyRead

    ~ServerBase();
    
private:
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    QString fileName;   //存放文件名
    QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区
    QTcpServer tcpServer;
    bool InitializeServers();
    Ui::ServerBase *ui;

private slots:
    void start();   //开始监听
    void acceptConnection();  //建立连接
    void updateServerProgress();  //更新进度条，接收数据
    void displayError(QAbstractSocket::SocketError socketError);
    void on_startButton_clicked();

};

#endif // SERVERBASE_H
