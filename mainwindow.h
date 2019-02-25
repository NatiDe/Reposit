#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    void on_BackupButton_clicked();

    void on_Library_clicked();

    void on_RestoreMain_clicked();

    void on_backupMain_clicked();

    void on_backTobackup_clicked();

    void on_BackToRestore_clicked();

    void on_RestoreButton_clicked();

    void on_ExitMain_clicked();

    void on_SystemProcess_clicked();


    void on_pushButton_clicked();

    void on_backTobackup_2_clicked();

    void on_AllBackup_clicked();

    void on_AllRestore_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
