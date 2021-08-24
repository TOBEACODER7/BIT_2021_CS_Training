#include "tcp_server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tcp_Server w;
    w.show();

    return a.exec();
}
