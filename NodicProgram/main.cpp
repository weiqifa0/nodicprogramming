#include "mainwindow.h"
#include <QApplication>
#include "mythread.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString runPath = QCoreApplication::applicationDirPath();
    w.setWindowTitle("HEX烧录软件["+runPath+"]");
    w.show();

    return a.exec();
}
