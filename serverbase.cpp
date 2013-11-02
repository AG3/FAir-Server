#include "serverbase.h"
#include "ui_serverbase.h"

ServerBase::ServerBase(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerBase)
{
    ui->setupUi(this);
    TCPnum=0;
    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    connect(ui->connectionlist,SIGNAL(itemActivated(QListWidgetItem*)),this,SLOT());
}

ServerBase::~ServerBase()
{
    delete ui;
}

void ServerBase::acceptConnection()  //接受连接
{
    TcpBox[TCPnum]=new FTcpConnection(this);
    TcpBox[TCPnum]->connection = tcpServer.nextPendingConnection();

    connect(TcpBox[TCPnum]->connection,SIGNAL(readyRead()),TcpBox[TCPnum],SLOT(StartRecive()));
    connect(TcpBox[TCPnum]->connection,SIGNAL(error(QAbstractSocket::SocketError)),TcpBox[TCPnum],SLOT(displayError(QAbstractSocket::SocketError)));
    connect(TcpBox[TCPnum],SIGNAL(ConnectionReady()),TcpBox[TCPnum],SLOT(updateServerProgress()));
    //在这里添加服务器上传文件的信号槽 然后在FTcpConnection里实现
    ui->connectionlist->addItem(QString(TCPnum));
    TCPnum++;
    ui->serverStatusLabel->setText(tr("接受连接"));
}

void ServerBase::on_startButton_clicked()
{
    ui->startButton->setEnabled(false);
    if(!tcpServer.listen(QHostAddress::Any,6666))
    {
        qDebug() << tcpServer.errorString();
        close();
        return;
    }
    ui->serverStatusLabel->setText(tr("监听"));
}
