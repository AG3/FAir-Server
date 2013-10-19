#ifndef SERVERBASE_H
#define SERVERBASE_H

#include <QMainWindow>
#include "QTcpServer"
#include"FTcpConnection.h"
#include"List"
#define N 100
namespace Ui {
class ServerBase;
}

class ServerBase : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ServerBase(QWidget *parent = 0);

    ~ServerBase();
    
private:
    QTcpServer tcpServer;
    int TCPnum;
    FTcpConnection* TcpBox[1000];
    Ui::ServerBase *ui;

private slots:
    void acceptConnection();  //建立连接
    void on_startButton_clicked();

};

#endif // SERVERBASE_H
