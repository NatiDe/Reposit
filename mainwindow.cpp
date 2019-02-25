#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QtCore>
#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <stdlib.h>
#include <QStorageInfo>
#include <QDesktopServices>
#include <QRadioButton>
#include <QMessageBox>
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>
#include <QDialogButtonBox>
#include <QRegExp>
#include <QFileInfo>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->PhotosBackup->setChecked(true);
    ui->VideoBackup->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
    ui->PhotosRestore->setChecked(true);
    ui->VideoRestore->setChecked(true);
    ui->All->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_backupMain_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_RestoreMain_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}



void MainWindow::on_backTobackup_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_BackToRestore_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_ExitMain_clicked()
{
    QMessageBox::StandardButton reply;

    reply =  QMessageBox::question(this,"EXIT","Are you sure to exit?",QMessageBox::Yes |QMessageBox::No);
    if(reply == QMessageBox::Yes){
        this->close();
    }else {
        ui->stackedWidget->setCurrentIndex(0);
    }

}


//Library opening
void MainWindow::on_Library_clicked()
{
    ///creating backup library directory///
    QDir mdir;
    QString path = "C:/Users/PC/Rewind/Backuplibrary";
    qDebug() << "Scanning precreated backup library ...";
    if(!mdir.exists(path)){
        mdir.mkpath(path);

        qDebug() << "Library Creation in Progress ...";
        qDebug() << "Library Created!";
    }else{
        qDebug() << "Library Already Exists!";
        qDebug() << "Opening the library...";}
    //opening the library//
    QDesktopServices::openUrl(QUrl("file:///C:/Users/PC/Rewind/Backuplibrary", QUrl::TolerantMode));
}

/********AUTOMATIC********/
////////////Auto backup///////////////

void MainWindow::on_BackupButton_clicked()
{
    ///////////////////////////connected devices///////////////////////////////
    QProcess process;
    process.start("adb shell getprop ro.boot.serialno");
    process.waitForFinished();
    QString error = process.readAllStandardError();
    QString outPut( process.readAllStandardOutput());
    string Output = outPut.toUtf8().constData();

    const char * serialNo = Output.c_str();
    cout<<serialNo;
    const char * info1 = "adb ";
    const char * info2 = "shell getprop ro.product.";
    const char * manufacturer = "manufacturer";
    const char* model = "model";
    ///////////////////////////
    string info1string = string(info1) + string(info2) + string(manufacturer);
    string info2string = string(info1) + string(info2) + string(model);


    const char * info1char = info1string.c_str();
    const char * info3char = info2string.c_str();

    /////manuafcturer////
    QProcess infoDev;
    infoDev.start(info1char);
    infoDev.waitForFinished();
    QString errorMan = infoDev.readAllStandardError();
    QString info1outPut( infoDev.readAllStandardOutput());
    string info1string1 =  info1outPut.toUtf8().constData();
    /////model/////
    QProcess infoDev2;
    infoDev2.start(info3char);
    infoDev2.waitForFinished();
    QString errorMan3 = infoDev2.readAllStandardError();
    QString info3outPut ( infoDev2.readAllStandardOutput());
    string info3string3 =  info3outPut.toUtf8().constData();
    const char * name2 = info3string3.c_str();
    /////////////////////////////////////////////////////////////////
    QString Video = "/Video";
    QString Photos = "/Images";
    QString list = QString::fromUtf8(name2);
    list = list.simplified();
    QDir backup;
    if(!backup.exists("C:/Users/PC/Rewind/Backuplibrary/" + list)){
        backup.mkpath("C:/Users/PC/Rewind/Backuplibrary/" + list);

    }

////////////////////////////////Xender//////////////////////////////////
    if(ui->Xender->isChecked()&&!ui->Telegram->isChecked()&&!ui->CameraRoll->isChecked()){
        QString xenDir ="C:/Users/PC/Rewind/Backuplibrary/" + list + "/Xender";
        QDir xenFolder;

        if(!xenFolder.exists(xenDir)){
            xenFolder.mkpath(xenDir);}
        QString  locationDir = "/sdcard/Xender";

        if(ui->VideoBackup->isChecked() && ui->PhotosBackup->isChecked()){
            QString command = "adb pull " + locationDir + "/video " + xenDir;
            QString command23 = "adb pull " + locationDir + "/image " + xenDir;

            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping photos and videos form Xender";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                string command2 = command.toUtf8().constData();
                const char * comFinal = command2.c_str();
               system(comFinal);
                qDebug()<<"Videos Finished";


                string command3 = command23.toUtf8().constData();
               const char * comFinal2 = command3.c_str();

               system(comFinal2);
               system("cls");

                qDebug()<<"Photos Finished";
                QString last = xenDir + "/image";
                QString last2 = xenDir + "/video";
                QDir mDir2(last2);
                QDir mDir(last);
                if(!mDir.exists() && !mDir2.exists(last2) | mDir.isEmpty() && mDir2.isEmpty()){
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt","Photos and Videos not found!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }
                }
                if(!mDir.exists() && mDir2.exists(last2) | mDir.isEmpty() && !mDir2.isEmpty()){

                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt","Photos not found!   Videos Backuped",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }

                }
                if(mDir.exists() && !mDir2.exists(last2) | !mDir.isEmpty() && mDir2.isEmpty()){

                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt","Videos not found!   Photos Backuped",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }



                }
                if(mDir.exists() && mDir2.exists(last2) | !mDir.isEmpty() && !mDir2.isEmpty()){
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt","Photos and Videos Backuped!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }
                }

            }


        }



        if(ui->PhotosBackup->isChecked()&&!ui->VideoBackup->isChecked()){
            QString command = "adb pull " + locationDir + "/image " + xenDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping photos form Xender";
            const char * promptDev = prompt.c_str();
            qDebug()<<command;
            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                string command2 = command.toUtf8().constData();
                const char * comFinal = command2.c_str();
                system(comFinal);
                system("cls");

                qDebug()<<"Finished";
                QDir mDir;
                if(!mDir.exists(xenDir + "/image")){
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt","Photos not found!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }
                }else{

                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Backup Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }


        if(ui->VideoBackup->isChecked()&&!ui->PhotosBackup->isChecked()){
            QString command = "adb pull " + locationDir + "/video " + xenDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping videos form Xender";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                string command2 = command.toUtf8().constData();
                const char * comFinal = command2.c_str();

                system(comFinal);

                system("cls");
                qDebug()<<"Finished";
                QString last =xenDir + "/video";
                QDir mDir(last);
                if( !mDir.exists() | mDir.isEmpty()){
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt","Videos not found!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }
                }else{

                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Backup Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
            }

        }else{
            ui->stackedWidget->setCurrentIndex(1);
        }

    }
    ///////////////////////////////////////////////Telegram/////////////////////////////////////////////

    if(ui->Telegram->isChecked()&&!ui->Xender->isChecked()&&!ui->CameraRoll->isChecked()){

        QString telDir ="C:/Users/PC/Rewind/Backuplibrary/" + list + "/Telegram";
        QDir telFolder;
        if(!telFolder.exists(telDir)){
            telFolder.mkpath(telDir);}
        QString  locationDir = "/sdcard/Telegram/";

        if(ui->VideoBackup->isChecked() && ui->PhotosBackup->isChecked()){
            QString command = "adb pull " + locationDir + "\"Telegram Video\" " + telDir;
            QString command23 = "adb pull " + locationDir + "\"Telegram Images\" " + telDir;
            qDebug()<<command<<endl;
            qDebug()<<command23<<endl;

            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping photos and videos form Telegram";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                string command2 = command.toUtf8().constData();
                const char * comFinal = command2.c_str();
                system(comFinal);
                qDebug()<<"Videos Finished";

                string command3 = command23.toUtf8().constData();
                const char * comFinal2 = command3.c_str();

                system(comFinal2);

                qDebug()<<"Photos Finished";}
            QString last = telDir + "/Telegram Images";
            QString last2 = telDir + "/Telegram Video";
            QDir mDir(last);
            QDir mDir2(last2);
            if(!mDir.exists() && !mDir2.exists(last2) | mDir.isEmpty() && mDir2.isEmpty()){
                QMessageBox::StandardButton info;
                info = QMessageBox::information(this,"Prompt","Photos and Videos not found!",QMessageBox::Ok);
                if(info == QMessageBox::Ok){
                    ui->stackedWidget->setCurrentIndex(0);
                }
            }
            if(mDir.exists() && !mDir2.exists(last2) |!mDir.isEmpty() && mDir2.isEmpty()){

                QMessageBox::StandardButton info;
                info = QMessageBox::information(this,"Prompt","Videos not found!   Photos Backuped",QMessageBox::Ok);
                if(info == QMessageBox::Ok){
                    ui->stackedWidget->setCurrentIndex(0);
                }

            }
            if((!mDir.exists() && mDir2.exists(last2) |mDir.isEmpty() && !mDir2.isEmpty())){

                QMessageBox::StandardButton info;
                info = QMessageBox::information(this,"Prompt","Photos not found!   Videos Backuped",QMessageBox::Ok);
                if(info == QMessageBox::Ok){
                    ui->stackedWidget->setCurrentIndex(0);
                }

            }
            if(mDir.exists() && mDir2.exists(last2) | !mDir.isEmpty() && !mDir2.isEmpty()){

                QMessageBox::StandardButton info;
                info = QMessageBox::information(this,"Prompt","Backup successful ",QMessageBox::Ok);
                if(info == QMessageBox::Ok){
                    ui->stackedWidget->setCurrentIndex(0);
                }



            }
        }



        if(ui->PhotosBackup->isChecked()&&!ui->VideoBackup->isChecked()){
            QString command = "adb pull " + locationDir + "\"Telegram Images\" " + telDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping photos form Telegram";
            const char * promptDev = prompt.c_str();
            qDebug()<<command;
            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);

            if(connected == QMessageBox::Yes){

                string command2 = command.toUtf8().constData();
                const char * comFinal = command2.c_str();
               system(comFinal);

                qDebug()<<"Photos Finished";
                QString last  = telDir +  "/Telegram Images" ;
                QDir mDir(last);

                if(!mDir.exists() | mDir.isEmpty()){

                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt"," Photos not Found!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }

                }else{
                    QMessageBox::information(this,"Prompt","Backup Finished");
                }
            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }



        if(ui->VideoBackup->isChecked()&&!ui->PhotosBackup->isChecked()){
            QString command = "adb pull " + locationDir + "\"Telegram Video\" " + telDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping videos form Telegram";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                string command2 = command.toUtf8().constData();
                const char * comFinal = command2.c_str();
               system(comFinal);
                qDebug()<<"Videos Finished";
                QString last  = telDir +  "/Telegram Video" ;
                QDir mDir(last);

                if(!mDir.exists() | mDir.isEmpty()){

                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"Prompt"," Videos not Found!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }

                }else{
                    QMessageBox::information(this,"Prompt","Backup Finished");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }



    }

    //////////////////////////////////////////////cameraRoll//////////////////////////////////////////////////

    if(ui->CameraRoll->isChecked()&&!ui->Telegram->isChecked()&&!ui->Xender->isChecked()){
        QString camDir = "C:/Users/PC/Rewind/Backuplibrary/" + list +"/cameraRoll";
        QDir mDir;
        if(!mDir.exists(camDir)){
            mDir.mkpath(camDir);

        }

        if(!mDir.exists(camDir + "/Photos")){
            mDir.mkpath(camDir + "/Photos");

        }
        if(!mDir.exists(camDir + "/Videos")){
            mDir.mkpath(camDir + "/Videos");

        }


        if(ui->VideoBackup->isChecked() && ui->PhotosBackup->isChecked()){
            system("adb shell find sdcard/DCIM -iname *.jpg >> C:/Users/PC/Rewind/photos.txt");

            system("adb shell find sdcard/DCIM -iname *.mp4 >> C:/Users/PC/Rewind/videos.txt");

            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping photos and videos form camera roll";
            const char * promptDev = prompt.c_str();
      /////////////////////////photos//////////////////////////////
            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){

                QFile inputFile(QString("C:/Users/PC/Rewind/photos.txt"));
                inputFile.open(QIODevice::ReadOnly);
                QTextStream stream(&inputFile);
                QString line = stream.readLine();
                while(!line.isNull()&& !stream.atEnd()){
                    line = stream.readLine();
                    line = line.simplified();
                    QString command = "adb pull " + line +" "+ camDir + "/Photos";
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);
            }
                system("cls");
                qDebug()<<"Finished";
                inputFile.close();
                inputFile.remove();

                QDir mDir2(camDir + "/Photos");
                QString string0;
                if(mDir2.isEmpty()){
                    string0 = "No Photos Found! ";
                }else{
                    string0 = "Photos Backuped! ";
                }

                //////////////////////////video///////////////////////////////////

                QFile inputFile2(QString("C:/Users/PC/Rewind/videos.txt"));
                inputFile2.open(QIODevice::ReadOnly);
                QTextStream stream2(&inputFile2);
                QString line2 = stream2.readLine();
                while(!line2.isNull() && !stream2.atEnd()){
                    line2 = stream2.readLine();
                    line2 = line2.simplified();
                    qDebug()<<line2;

                    QString command2 = "adb pull " + line2 +" "+ camDir + "/Videos";
                    string command3 = command2.toUtf8().constData();
                    const char * comFinal = command3.c_str();
                    system(comFinal);


                }
                system("cls");
                qDebug()<<"Finished";
                inputFile2.close();
                inputFile2.remove();
                QDir mDir1(camDir + "/Videos");
                QString string1;
                if(mDir1.isEmpty()){
                    string1 = "No Videos Found!";
                }else{
                    string1 = "Videos Backuped! ";
                }
                QString last = string0 + string1;
                QMessageBox::information(this,"prompt",last);
            }else{
                ui->stackedWidget->setCurrentIndex(0);
            }


        }
        if(!ui->VideoBackup->isChecked() && ui->PhotosBackup->isChecked()){
            system("adb shell find sdcard/DCIM -iname *.jpg >> C:/Users/PC/Rewind/photos.txt");

            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping photos form camera roll";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                QFile inputFile(QString("C:/Users/PC/Rewind/photos.txt"));
                inputFile.open(QIODevice::ReadOnly);
                QTextStream stream(&inputFile);
                QString line = stream.readLine();

                while(!line.isNull() && !stream.atEnd()){
                    line = stream.readLine();
                    line = line.simplified();
                    QString command = "adb pull " + line +" "+ camDir + "/Photos";
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);

                }
                qDebug()<<"Finished";

                inputFile.close();
                inputFile.remove();
                QDir mDir2(camDir + "/Photos");
                QString string;
                if(mDir2.isEmpty()){
                    string = "No Photos Found! ";
                }else{
                    string = "Photos Backuped! ";
                }
                QMessageBox::information(this,"prompt",string);
            }

        }
        if(ui->VideoBackup->isChecked() && !ui->PhotosBackup->isChecked()){
            system("adb shell find sdcard/DCIM -iname *.mp4 >> C:/Users/PC/Rewind/videos.txt");

            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"backuping videos form camera roll";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){

                QFile inputFile2(QString("C:/Users/PC/Rewind/videos.txt"));
                inputFile2.open(QIODevice::ReadOnly);
                QTextStream stream2(&inputFile2);
                QString line2 = stream2.readLine();
                while(!line2.isNull() && !stream2.atEnd()){
                    line2 = stream2.readLine();
                    line2 = line2.simplified();
                    qDebug()<<line2;

                    QString command2 = "adb pull " + line2 +" "+ camDir + "/Videos";
                    string command23 = command2.toUtf8().constData();
                    const char * finalCom = command23.c_str();
                   system(finalCom);


                }
                qDebug()<<"Finished";
                inputFile2.close();
                inputFile2.remove();

                QDir mDir1(camDir + "/Videos");
                QString string1;
                if(mDir1.isEmpty()){
                    string1 = "No Videos Found!";
                }else{
                    string1 = "Videos Backuped! ";
                }
                QMessageBox::information(this,"prompt",string1);
            }else{
                ui->stackedWidget->setCurrentIndex(0);
            }
        }
    }


}
//////////////Auto Restore/////////////

void MainWindow::on_RestoreButton_clicked()
{  ////////////////connected devices////////////////////


    QProcess process;
    process.start("adb shell getprop ro.boot.serialno");
    process.waitForFinished();
    QString error = process.readAllStandardError();
    QString outPut( process.readAllStandardOutput());
    string Output = outPut.toUtf8().constData();


    cout<<Output;
    const char * info1 = "adb ";
    const char * info2 = "shell getprop ro.product.";
    const char * manufacturer = "manufacturer";
    const char* model = "model";
    ///////////////////////////
    string info1string = string(info1) + string(info2) + string(manufacturer);
    string info2string = string(info1) + string(info2) + string(model);


    const char * info1char = info1string.c_str();
    const char * info3char = info2string.c_str();

    /////manuafcturer////
    QProcess infoDev;
    infoDev.start(info1char);
    infoDev.waitForFinished();
    QString errorMan = infoDev.readAllStandardError();
    QString info1outPut( infoDev.readAllStandardOutput());
    string info1string1 =  info1outPut.toUtf8().constData();



    /////model/////
    QProcess infoDev2;
    infoDev2.start(info3char);
    infoDev2.waitForFinished();
    QString errorMan3 = infoDev2.readAllStandardError();
    QString info3outPut ( infoDev2.readAllStandardOutput());
    string info3string3 =  info3outPut.toUtf8().constData();
    const char * name2 = info3string3.c_str();


    /////xender//////
    QString list = QString::fromUtf8(name2);
    list = list.simplified();
    if(ui->Xender_2->isChecked()&&!ui->Telegram_2->isChecked()&&!ui->CameraRoll2->isChecked()){
        QString xenDir ="C:/Users/PC/Rewind/Backuplibrary/" + list + "/Xender";
        QDir xenFolder;

        QString  locationDir = "/sdcard/Xender";

        if(ui->VideoRestore->isChecked() && ui->PhotosRestore->isChecked()){
            QString command = "adb push " + xenDir + "/video " + locationDir;
            QString command23 = "adb push " +xenDir + "/image " + locationDir ;
            QString mString = xenDir + "/video";
            QString mString2 = xenDir + "/image";
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring photos and videos to Xender";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                int x = 2 ,y = 2 ;
                QDir mDir;
                if(mDir.exists(mString)){
                    x=1;
                }else{
                    x=0;
                }
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                   system(comFinal);

                    qDebug()<<"Videos Finished";

                }

                if(mDir.exists(mString2)){
                    y = 1;
                }else{
                    y = 0;
                }

                if(y == 1){
                    string command3 = command23.toUtf8().constData();
                    const char * comFinal = command3.c_str();
                    system(comFinal);

                    qDebug()<<"Photos Finished";


                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(2);
                    } }
                if(x == 0 && y == 1){
                    QMessageBox::information(this,"Fail","Video Backup Not found ");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 1 && y == 0){
                    QMessageBox::information(this,"Fail","Photo Backup Not found");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 0 && y == 0){
                    QMessageBox::information(this,"Fail","Backup Not found");
                    ui->stackedWidget->setCurrentIndex(2);
                }

            }else{
                ui->stackedWidget->setCurrentIndex(2);
            }


        }



        if(ui->VideoRestore->isChecked()&&!ui->PhotosRestore->isChecked()){


            int x = 2;
            QDir mDir;
            QString mString = xenDir + "/video";
            if(mDir.exists(mString)){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + xenDir + "/video " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring videos to Xender";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                   system(comFinal);

                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Video Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Video Backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }


        }

        if(ui->PhotosRestore->isChecked()&&!ui->VideoRestore->isChecked()){
            int x = 2;
            QDir mDir;
            QString mString = xenDir + "/image";
            if(mDir.exists(mString)){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + xenDir + "/image " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring photos to Xender";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);

                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Photo Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Photo backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }
    }
    //////Telegram///////
    if(ui->Telegram_2->isChecked() && !ui->Xender_2->isChecked() && !ui->CameraRoll2->isChecked()){
        QString  locationDir = "/sdcard/Telegram";
        QString telDir ="C:/Users/PC/Rewind/Backuplibrary/" + list + "/Telegram";
        QDir telFolder;
        QString mString = telDir + "/\"Telegram Video\" ";
        QString mString2 = telDir + "/\"Telegram Images\" ";
        QString command = "adb push " + mString + locationDir  ;
        QString command23 = "adb push " + mString2 + locationDir ;

        if(ui->VideoRestore->isChecked() && ui->PhotosRestore->isChecked()){

            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring photos and videos to Telegram";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                int x = 2,y = 2;
                QString list2 = telDir + "/Telegram Video";
                QString list = telDir + "/Telegram Images";
                QDir mDir(list2);
                QDir mDir1(list);
                if(mDir1.exists() && !mDir1.isEmpty()){
                    /*video*/ x = 1;
                }else{
                    x = 0;
                }
                if(mDir.exists() && !mDir.isEmpty()){
                    /*Photo*/y = 1;
                }else{
                    y = 0;
                }
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);

                }
                if(y == 1){
                    string command3 = command23.toUtf8().constData();
                    const char * comFinal = command3.c_str();
                    system(comFinal);

                    qDebug()<<"Photos Finished";


                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(2);
                    } }
                if(x == 0 && y == 1){
                    QMessageBox::information(this,"Prompt","Video Backup Not found...Photo Restored!");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 1 && y == 0){
                    QMessageBox::information(this,"Prompt","Photo Backup Not found...Video Restored!");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 0 && y == 0){
                    QMessageBox::information(this,"Prompt","Backup Not found!");
                    ui->stackedWidget->setCurrentIndex(2);
                }



            }else{
                ui->stackedWidget->setCurrentIndex(0);
            }



        }else{
            ui->stackedWidget->setCurrentIndex(2);
        }
        if(ui->VideoRestore->isChecked() && !ui->PhotosRestore->isChecked()){


            int x = 2;
            QString mString = telDir + "/\"Telegram Video\"";

            QString mString2 = telDir + "/Telegram Video";
            QDir mDir(mString2);

            if(mDir.exists()  && !mDir.isEmpty()){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + telDir + "/\"Telegram Video\" " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring videos to Telegram";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                   system(comFinal);
                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Video Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Video backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }
        if(!ui->VideoRestore->isChecked() && ui->PhotosRestore->isChecked()){


            int x = 2;
            QString mString = telDir + "/\"Telegram Images\"";
            QString mString2 = telDir + "/Telegram Images";
            QDir mDir(mString2);

            if(mDir.exists() && !mDir.isEmpty()){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + telDir + "/\"Telegram Images\" " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"Restoring Images to Telegram";
            const char * promptDev = prompt.c_str();
            qDebug()<<command;
            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);

                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Photo Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Photo backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }


    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    if(!ui->Telegram_2->isChecked() && !ui->Xender_2->isChecked() && ui->CameraRoll2->isChecked()){

        QString camDir ="C:/Users/PC/Rewind/Backuplibrary/" + list + "/cameraRoll";
        QDir camFolder;

        QString  locationDir = "/sdcard/Rewind";
        system("adb shell mkdir sdcard/Rewind");

        if(ui->VideoRestore->isChecked() && ui->PhotosRestore->isChecked()){
            QString command = "adb push " + camDir + "/video " + locationDir;
            QString command23 = "adb push " +camDir + "/image " + locationDir ;
            QString mString = camDir + "/Videos";
            QString mString2 = camDir + "/Photos";
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring photos and videos to Device";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                int x = 2 ,y = 2 ;
                QDir mDir(mString);
                if(mDir.exists() && !mDir.isEmpty()){
                    x=1;
                }else{
                    x=0;
                }
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);
                    qDebug()<<"Videos Finished";

                }
                QDir mDir2(mString2);
                if(mDir2.exists() && !mDir2.isEmpty() ){
                    y = 1;
                }else{
                    y = 0;
                }

                if(y == 1){
                    string command3 = command23.toUtf8().constData();
                    const char * comFinal = command3.c_str();
                    system(comFinal);

                    QProcess process2;
                    process2.start(command23);
                    process2.waitForFinished();
                    QString error2(process.readAllStandardError());
                    QString output2(process.readAllStandardOutput());
                    qDebug()<<"Photos Finished";


                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(2);
                    } }
                if(x == 0 && y == 1){
                    QMessageBox::information(this,"Fail","Video Backup Not found...Images Restored ");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 1 && y == 0){
                    QMessageBox::information(this,"Fail","Photo Backup Not found...Videos Restored");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 0 && y == 0){
                    QMessageBox::information(this,"Fail","Backup Not found");
                    ui->stackedWidget->setCurrentIndex(2);
                }

            }else{
                ui->stackedWidget->setCurrentIndex(2);
            }


        }



        if(ui->VideoRestore->isChecked()&&!ui->PhotosRestore->isChecked()){


            int x = 2;
            QString mString = camDir + "/Videos";
            QDir mDir(mString);

            if(mDir.exists() && !mDir.isEmpty()){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + camDir + "/Video " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring videos to Device";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);

                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Video Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Video Backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }


        }

        if(ui->PhotosRestore->isChecked()&&!ui->VideoRestore->isChecked()){
            int x = 2;
            QString mString = camDir + "/Photos";
            QDir mDir(mString);

            if(mDir.exists() && !mDir.isEmpty()){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + camDir + "/Photos " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring photos to Device";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                   system(comFinal);
                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Photo Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Photo backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }

    }

}

///////////System Backup and restore//////////
void MainWindow::on_SystemProcess_clicked()
{       //////Backup//////
    if(ui->RadioBackup->isChecked()){
        string all , apps , extSdcard , systemBac ;

        string command = "adb backup";
        string file = "C:/Users/PC/Rewind/Backup library/System Backups";
        QDir path;
        if(!path.exists("C:/Users/PC/Rewind/Backup library/System Backups")){
            path.mkdir("C:/Users/PC/Rewind/Backup library/System Backups");
        }
        if(ui->Apps->isChecked()){
            apps = " -apk";
        }else{
            apps = " -noapk";}
        if(ui->All->isChecked()){
            all = " -all";}
        if(ui->extSdcard->isChecked()){
            extSdcard = " -shared";
        }else{
            extSdcard = " -noshared"; }
        if(ui->System->isChecked()){
            systemBac = " -system";
        }else{
            systemBac = " -nosystem"; }

        //////////connected devices///////////

        QProcess process;
        process.start("adb shell getprop ro.boot.serialno");
        process.waitForFinished();
        QString error = process.readAllStandardError();
        QString outPut( process.readAllStandardOutput());
        string Output = outPut.toUtf8().constData();

        const char * serialNo = Output.c_str();
        cout<<serialNo;
        const char * info1 = "adb ";
        const char * info2 = "shell getprop ro.product.";
        const char * manufacturer = "manufacturer";
        const char* model = "model";
        ///////////////////////////
        string info1string = string(info1) + string(info2) + string(manufacturer);
        string info2string = string(info1) + string(info2) + string(model);


        const char * info1char = info1string.c_str();
        const char * info3char = info2string.c_str();

        /////manuafcturer////
        QProcess infoDev;
        infoDev.start(info1char);
        infoDev.waitForFinished();
        QString errorMan = infoDev.readAllStandardError();
        QString info1outPut( infoDev.readAllStandardOutput());
        string info1string1 =  info1outPut.toUtf8().constData();
        const char * name1 =info1string1.c_str();


        /////model/////
        QProcess infoDev2;
        infoDev2.start(info3char);
        infoDev2.waitForFinished();
        QString errorMan3 = infoDev2.readAllStandardError();
        QString info3outPut ( infoDev2.readAllStandardOutput());
        string info3string3 =  info3outPut.toUtf8().constData();
        const char * name2 = info3string3.c_str();



        string FinalInfo = string(name1) + " " + string(name2);
       /////////////////////////////////////////////

        QMessageBox::StandardButton connected;
        string prompt = "Continue with " + FinalInfo;
        const char * promptDev = prompt.c_str();


        connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
        if(connected == QMessageBox::Yes){

            QString list  = QString::fromUtf8(name2);
            list = list.simplified();
            string list2 = list.toUtf8().constData();
            string directory ="C:/Users/PC/Rewind/Backuplibrary/SystemBackups/" + list2;
            const char * dir = directory.c_str();
            QString FinalDir = QString::fromUtf8(dir);
            QDir mDir;
            if(!mDir.exists(FinalDir)){
                mDir.mkpath(FinalDir);
            }
            QString file = QString::fromUtf8(dir) +"/"+ list + ".ab" ;
            qDebug()<<file;
            if(QFileInfo::exists(file)&&QFileInfo(file).isFile()){
                QMessageBox::StandardButton override;
                override = QMessageBox::question(this,"Continue"," Backup already exists Override and continue?",QMessageBox::Yes | QMessageBox::No);
                if(override == QMessageBox::Yes){
                    QFile::remove(file);

                    string command_final = command + systemBac + apps + extSdcard +  all + " -f " + directory +"/"+list2 +".ab";


                    const char * command1 = command_final.c_str();
                    QProcess Backup;
                    Backup.start(command1);
                    Backup.waitForFinished();
                    QString error(Backup.readAllStandardError());
                    QString Out(Backup.readAllStandardOutput());
                    QMessageBox::information(this,"Prompt","Override completed");

                }else{
                    ui->stackedWidget->setCurrentIndex(0);
                }
            }else{

                string command_final = command + systemBac + apps + extSdcard +  all + " -f " + directory +"/"+list2 +".ab";


                const char * command1 = command_final.c_str();
                QProcess Backup;
                Backup.start(command1);
                Backup.waitForFinished();
                QString error(Backup.readAllStandardError());
                QString Out(Backup.readAllStandardOutput());
                QMessageBox::information(this,"Prompt","Backup completed");
            }

        }
    }
    //////Restore/////
    if(ui->RadioRestore->isChecked()){

        //////////connected devices///////////

        QProcess process;
        process.start("adb shell getprop ro.boot.serialno");
        process.waitForFinished();
        QString error = process.readAllStandardError();
        QString outPut( process.readAllStandardOutput());
        string Output = outPut.toUtf8().constData();


        cout<<Output;
        const char * info1 = "adb ";
        const char * info2 = "shell getprop ro.product.";
        const char * manufacturer = "manufacturer";
        const char* model = "model";
        ///////////////////////////
        string info1string = string(info1) + string(info2) + string(manufacturer);
        string info2string = string(info1) + string(info2) + string(model);

        const char * info1char = info1string.c_str();
        const char * info3char = info2string.c_str();

        /////manuafcturer////
        QProcess infoDev;
        infoDev.start(info1char);
        infoDev.waitForFinished();
        QString errorMan = infoDev.readAllStandardError();
        QString info1outPut( infoDev.readAllStandardOutput());
        string info1string1 =  info1outPut.toUtf8().constData();


        /////model/////
        QProcess infoDev2;
        infoDev2.start(info3char);
        infoDev2.waitForFinished();
        QString errorMan3 = infoDev2.readAllStandardError();
        QString info3outPut ( infoDev2.readAllStandardOutput());
        string info3string3 =  info3outPut.toUtf8().constData();
        const char * name2 = info3string3.c_str();

        ////////////////////////////////////////////////

        QString list  = QString::fromUtf8(name2);
        list = list.simplified();
        string list2 = list.toUtf8().constData();
        string locating =  "/" + list2 + ".ab";
        string file = "C:/Users/PC/Rewind/Backuplibrary/SystemBackups/" + list2;
        string temp = file + locating;
        QString file2 = "C:/Users/PC/Rewind/Backuplibrary/SystemBackups/" + list +"/" + list + ".ab";
        string pro2 = "Restore to " + list2;
        const char * FinalPrompt = pro2.c_str();

        QMessageBox::StandardButton connected;
        connected = QMessageBox::question(this,"process",FinalPrompt,QMessageBox::Yes |QMessageBox::No);
        if(connected == QMessageBox::Yes){

            if(QFileInfo::exists(file2)&&QFileInfo(file2).isFile()){

                QMessageBox::StandardButton Restore;
                Restore = QMessageBox::question(this,"Continue"," Backup Exists, Continue?",QMessageBox::Yes | QMessageBox::No);

                if(Restore == QMessageBox::Yes){

                    string command_final ="adb restore " + temp;
                    const char * command1 = command_final.c_str();
                    QMessageBox::information(this,"Processing","Restore is in progress...may take a while!");
                    QProcess Backup;
                    Backup.start(command1);
                    Backup.waitForFinished();
                    QString error(Backup.readAllStandardOutput());
                    QMessageBox::information(this,"Prompt","Restore completed!");

                }else{ui->stackedWidget->setCurrentIndex(0); }

            }else{
                QMessageBox::information(this,"Prompt","Backup Not found! ");
                ui->stackedWidget->setCurrentIndex(0);


            }

        }

    }}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_backTobackup_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_AllBackup_clicked()
{   ///////////////////////////connected devices///////////////////////////////
    QProcess process;
    process.start("adb shell getprop ro.boot.serialno");
    process.waitForFinished();
    QString error = process.readAllStandardError();
    QString outPut( process.readAllStandardOutput());
    string Output = outPut.toUtf8().constData();

    const char * serialNo = Output.c_str();
    cout<<serialNo;
    const char * info1 = "adb ";
    const char * info2 = "shell getprop ro.product.";
    const char * manufacturer = "manufacturer";
    const char* model = "model";
    ///////////////////////////
    string info1string = string(info1) + string(info2) + string(manufacturer);
    string info2string = string(info1) + string(info2) + string(model);


    const char * info1char = info1string.c_str();
    const char * info3char = info2string.c_str();

    /////manuafcturer////
    QProcess infoDev;
    infoDev.start(info1char);
    infoDev.waitForFinished();
    QString errorMan = infoDev.readAllStandardError();
    QString info1outPut( infoDev.readAllStandardOutput());
    string info1string1 =  info1outPut.toUtf8().constData();
   /////model/////
    QProcess infoDev2;
    infoDev2.start(info3char);
    infoDev2.waitForFinished();
    QString errorMan3 = infoDev2.readAllStandardError();
    QString info3outPut ( infoDev2.readAllStandardOutput());
    string info3string3 =  info3outPut.toUtf8().constData();
    const char * name2 = info3string3.c_str();
  /////////////////////////////////////////////////////////////////
    QString Video = "/Video";
    QString Photos = "/Images";
    QString list = QString::fromUtf8(name2);
    list = list.simplified();
    QDir backup;
    if(!backup.exists("C:/Users/PC/Rewind/Backuplibrary/" + list)){
        backup.mkpath("C:/Users/PC/Rewind/Backuplibrary/" + list);

    }



    QString devDir = "C:/Users/PC/Rewind/Backuplibrary/" + list +"/Default";
    QDir mDir;
    if(!mDir.exists(devDir)){
        mDir.mkpath(devDir);

    }

    if(!mDir.exists(devDir + "/Photos")){
        mDir.mkpath(devDir + "/Photos");

    }
    if(!mDir.exists(devDir + "/Videos")){
        mDir.mkpath(devDir + "/Videos");
    }
    if(ui->BackupAllPhoto->isChecked() && ui->BackupAllVideos->isChecked()){
        system("adb shell find sdcard/ -iname *.jpg >> C:/Users/PC/Rewind/photos.txt");

        system("adb shell find sdcard/ -iname *.mkv >> C:/Users/PC/Rewind/videos.txt");
        system("adb shell find sdcard/ -iname *.flv >> C:/Users/PC/Rewind/videos.txt");
        system("adb shell find sdcard/ -iname *.mp4 >> C:/Users/PC/Rewind/videos.txt");

        QMessageBox::StandardButton connected;
        string prompt = "Continue with " + string(name2) +"backuping all photos and videos form Device";
        const char * promptDev = prompt.c_str();

        connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
        if(connected == QMessageBox::Yes){

            QFile inputFile(QString("C:/Users/PC/Rewind/photos.txt"));
            inputFile.open(QIODevice::ReadOnly);
            QTextStream stream(&inputFile);
            QString line = stream.readLine();
            while(!line.isNull() && !stream.atEnd()){
                line = stream.readLine();
                line = line.simplified();

                QString command = "adb pull " + line +" "+ devDir + "/Photos";
                string command2 = command.toUtf8().constData();
                const char * comFinal = command2.c_str();
                system(comFinal);

            }
            system("cls");
            qDebug()<<"Finished";
            inputFile.close();
            inputFile.remove();
            QDir mDir2(devDir + "/Photos");
            QString stringx;
            if(mDir2.isEmpty()){
                stringx = "No Photos Found! ";
            }else{
                stringx = "Photos Backuped! ";
            }

            /////////////////////////////////////////////////////////////

            QFile inputFile2(QString("C:/Users/PC/Rewind/videos.txt"));
            inputFile2.open(QIODevice::ReadOnly);
            QTextStream stream2(&inputFile2);
            QString line2 = stream2.readLine();
            while(!line2.isNull() && !stream.atEnd()){
                line2 = stream2.readLine();
                line2 = line2.simplified();
                qDebug()<<line2;

                QString command2 = "adb pull " + line2 +" "+ devDir + "/Videos";
                string command23 = command2.toUtf8().constData();
                const char * comFinal = command23.c_str();
                system(comFinal);


            }
            system("cls");
            qDebug()<<"Finished";
            inputFile2.close();
            inputFile2.remove();
            QDir mDir1(devDir + "/Videos");
            QString string1;
            if(mDir1.isEmpty()){
                string1 = "No Videos Found!";
            }else{
                string1 = "Videos Backuped! ";
            }
            QString last = stringx + string1;
            QMessageBox::information(this,"prompt",last);
        }else{
            ui->stackedWidget->setCurrentIndex(0);
        }

    }
    ////////////////////////////////
    if(ui->BackupAllPhoto->isChecked() && !ui->BackupAllVideos->isChecked()){
        system("adb shell find sdcard/ -iname *.jpg >> C:/Users/PC/Rewind/photos.txt");


        QMessageBox::StandardButton connected;
        string prompt = "Continue with " + string(name2) +"backuping photos form Device";
        const char * promptDev = prompt.c_str();

        connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
        if(connected == QMessageBox::Yes){
            QFile inputFile(QString("C:/Users/PC/Rewind/photos.txt"));
            inputFile.open(QIODevice::ReadOnly);
            QTextStream stream(&inputFile);
            QString line = stream.readLine();
            while(!line.isNull() && !stream.atEnd()){
                line = stream.readLine();
                line = line.simplified();
                QString command = "adb pull " + line +" "+ devDir + "/Photos";
               string command2 = command.toUtf8().constData();
               const char * comFinal = command2.c_str();
               system(comFinal);

            }
            system("cls");
            qDebug()<<"Finished";

            inputFile.close();
            inputFile.remove();
            QDir mDir2(devDir + "/Photos");
            QString string;
            if(mDir2.isEmpty()){
                string = "No Photos Found! ";
            }else{
                string = "Photos Backuped! ";
            }
            QMessageBox::information(this,"prompt",string);
        }


    }
    ////////////////////////////////
    if(!ui->BackupAllPhoto->isChecked() && ui->BackupAllVideos->isChecked()){
        system("adb shell find sdcard/ -iname *.mkv >> C:/Users/PC/Rewind/videos.txt");
        system("adb shell find sdcard/ -iname *.flv >> C:/Users/PC/Rewind/videos.txt");
        system("adb shell find sdcard/ -iname *.mp4 >> C:/Users/PC/Rewind/videos.txt");

        QMessageBox::StandardButton connected;
        string prompt = "Continue with " + string(name2) +"backuping photos form Device";
        const char * promptDev = prompt.c_str();

        connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
        if(connected == QMessageBox::Yes){
            QFile inputFile(QString("C:/Users/PC/Rewind/videos.txt"));
            inputFile.open(QIODevice::ReadOnly);
            QTextStream stream(&inputFile);
            QString line = stream.readLine();
            while(!line.isNull() && !stream.atEnd()){
                line = stream.readLine();
                line = line.simplified();
                QString command = "adb pull " + line +" "+ devDir + "/Videos";
               string command2 = command.toUtf8().constData();
               const char * comFinal = command2.c_str();
               system(comFinal);

            }
            system("cls");
            qDebug()<<"Finished";

            inputFile.close();
            inputFile.remove();
            QDir mDir2(devDir + "/Videos");
            QString string;
            if(mDir2.isEmpty()){
                string = "No Videos Found! ";
            }else{
                string = "Videos Backuped! ";
            }
            QMessageBox::information(this,"prompt",string);
        }


    }
}

void MainWindow::on_AllRestore_clicked()
{
    ///////////////////////////connected devices///////////////////////////////
        QProcess process;
        process.start("adb shell getprop ro.boot.serialno");
        process.waitForFinished();
        QString error = process.readAllStandardError();
        QString outPut( process.readAllStandardOutput());
        string Output = outPut.toUtf8().constData();

        const char * serialNo = Output.c_str();
        cout<<serialNo;
        const char * info1 = "adb ";
        const char * info2 = "shell getprop ro.product.";
        const char * manufacturer = "manufacturer";
        const char* model = "model";
        ///////////////////////////
        string info1string = string(info1) + string(info2) + string(manufacturer);
        string info2string = string(info1) + string(info2) + string(model);


        const char * info1char = info1string.c_str();
        const char * info3char = info2string.c_str();

        /////manuafcturer////
        QProcess infoDev;
        infoDev.start(info1char);
        infoDev.waitForFinished();
        QString errorMan = infoDev.readAllStandardError();
        QString info1outPut( infoDev.readAllStandardOutput());
        string info1string1 =  info1outPut.toUtf8().constData();
       /////model/////
        QProcess infoDev2;
        infoDev2.start(info3char);
        infoDev2.waitForFinished();
        QString errorMan3 = infoDev2.readAllStandardError();
        QString info3outPut ( infoDev2.readAllStandardOutput());
        string info3string3 =  info3outPut.toUtf8().constData();
        const char * name2 = info3string3.c_str();
      /////////////////////////////////////////////////////////////////
        QString list;
        list = info3outPut.simplified();
        QString camDir ="C:/Users/PC/Rewind/Backuplibrary/" + list + "/Default";
        QDir camFolder;

        QString  locationDir = "/sdcard/Rewind";
        system("adb shell mkdir sdcard/Rewind");
        system("cls");

        if(ui->RestoreAllPhoto->isChecked() && ui->RestoreAllVideos->isChecked()){
            QString command = "adb push " + camDir + "/Videos " + locationDir;
            QString command23 = "adb push " +camDir + "/Photos " + locationDir ;
            QString mString = camDir + "/Videos";
            QString mString2 = camDir + "/Photos";
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring photos and videos to Device";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                int x = 2 ,y = 2 ;
                QDir mDir(mString);
                if(mDir.exists() && !mDir.isEmpty()){
                    x=1;
                }else{
                    x=0;
                }
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);
                    qDebug()<<"Videos Finished";

                }
                QDir mDir2(mString2);
                if(mDir2.exists() && !mDir2.isEmpty() ){
                    y = 1;
                }else{
                    y = 0;
                }

                if(y == 1){
                    string command3 = command23.toUtf8().constData();
                    const char * comFinal = command3.c_str();
                    system(comFinal);

                    qDebug()<<"Photos Finished";


                    }
                if(x == 0 && y == 1){
                    QMessageBox::information(this,"Fail","Video Backup Not found...Images Restored ");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 1 && y == 0){
                    QMessageBox::information(this,"Fail","Photo Backup Not found...Videos Restored");
                    ui->stackedWidget->setCurrentIndex(2);
                }
                if(x == 0 && y == 0){
                    QMessageBox::information(this,"Fail","Backup Not found");
                    ui->stackedWidget->setCurrentIndex(2);
                }

            }else{
                ui->stackedWidget->setCurrentIndex(2);
            }


        }



        if(ui->RestoreAllVideos->isChecked()&&!ui->RestoreAllPhoto->isChecked()){


            int x = 2;
            QString mString = camDir + "/Videos";
            QDir mDir(mString);

            if(mDir.exists() && !mDir.isEmpty()){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + camDir + "/Video " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring videos to Device";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                    system(comFinal);

                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Video Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Video Backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }


        }

        if(ui->RestoreAllPhoto->isChecked()&&!ui->RestoreAllVideos->isChecked()){
            int x = 2;
            QString mString = camDir + "/Photos";
            QDir mDir(mString);

            if(mDir.exists() && !mDir.isEmpty()){
                x =1;
            }else{x = 0;}

            QString command = "adb push " + camDir + "/Photos " + locationDir;
            QMessageBox::StandardButton connected;
            string prompt = "Continue with " + string(name2) +"restoring photos to Device";
            const char * promptDev = prompt.c_str();

            connected =  QMessageBox::question(this,"Connected Devices",promptDev,QMessageBox::Yes |QMessageBox::No);
            if(connected == QMessageBox::Yes){
                if(x == 1){
                    string command2 = command.toUtf8().constData();
                    const char * comFinal = command2.c_str();
                   system(comFinal);
                    qDebug()<<"Finished";
                    QMessageBox::StandardButton info;
                    info = QMessageBox::information(this,"End","Photo Restore Finished!",QMessageBox::Ok);
                    if(info == QMessageBox::Ok){
                        ui->stackedWidget->setCurrentIndex(0);
                    }}
                if(x == 0){
                    QMessageBox::information(this,"Fail","Photo backups not found!");
                }

            }else{
                ui->stackedWidget->setCurrentIndex(1);
            }

        }
}
