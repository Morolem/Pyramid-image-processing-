#include "mainwindow.h"
#include <QtWidgets>
#include <vector>
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);   
    MainWindow w;
    w.show();
    return a.exec();
}
