#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_start_clicked();
    void on_pushButton_browse_clicked();

    void readyReadStandardOutput();
    void readyReadStandardError();
    void flasherFinished(int code);

private:
    Ui::MainWindow *ui;
    QFileDialog fd;
    QProcess flasher;

    void indicate(QString state);
};

#endif // MAINWINDOW_H
