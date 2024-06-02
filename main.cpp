#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString appid = "niko-player-qt-edition";
    QSharedMemory sharedMemory(appid, &a);
    MainWindow w;
    if(!sharedMemory.attach()){
        if(sharedMemory.create(1)){
            w.show();
        }else {
            QMessageBox::critical(nullptr, "Error", "cann't create a single instance");
            return 1;
        }
    }else {
        QMessageBox::critical(nullptr, "warning", "ther is one niko player is running");
        return 0;
    }
    return a.exec();
}
