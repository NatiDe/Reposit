#include "mainwindow.h"
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QDebug>
#include <stdlib.h>
#include <QString>
#include<string>
#include <iostream>
#include <QProcess>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    return a.exec();
}

