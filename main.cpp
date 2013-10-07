#include "serverbase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerBase w;
    w.show();
    
    return a.exec();
}
