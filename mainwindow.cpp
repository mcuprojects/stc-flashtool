#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&fd, SIGNAL(fileSelected(QString)), ui->lineEdit_filename, SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    ui->textBrowser->clear();
    indicate("");

    QString port = ui->lineEdit_port->text();
    QString mcu = ui->lineEdit_mcu->text();
    QString baud = ui->lineEdit_baud->text();
    QString filename = ui->lineEdit_filename->text();

    if (filename.isEmpty()) {
        ui->textBrowser->setTextColor(QColor(Qt::red));
        ui->textBrowser->append("file not selected");
        indicate("fail");
        return;
    }

    QString program = "stcgal";
    QStringList arguments;
    arguments << "-p" << port;
    arguments << "-P" << mcu;
    arguments << "-b" << baud;
    arguments << filename;

    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->append(program + " " + arguments.join(" "));

    flasher.start(program, arguments);
    connect(&flasher, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(&flasher, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    connect(&flasher, SIGNAL(finished(int)), this, SLOT(flasherFinished(int)));
}

void MainWindow::readyReadStandardOutput()
{
    QString text = flasher.readAllStandardOutput();
    const QString waiting = "Waiting for MCU, please cycle power:";
    if (text.contains(waiting)) {
        ui->textBrowser->setTextColor(Qt::black);
        ui->textBrowser->append(text.left(text.indexOf(waiting)));
        ui->textBrowser->setTextColor(Qt::green);
        ui->textBrowser->append(waiting);
        ui->textBrowser->setTextColor(Qt::black);
        ui->textBrowser->append(text.right(text.length() - text.indexOf(waiting) - waiting.length()));
    } else {
        ui->textBrowser->setTextColor(Qt::black);
        ui->textBrowser->append(text);
    }
}

void MainWindow::readyReadStandardError()
{
    ui->textBrowser->setTextColor(Qt::red);
    ui->textBrowser->append(flasher.readAllStandardError());
}

void MainWindow::on_pushButton_browse_clicked()
{
    fd.setNameFilter("*.ihx");
    fd.open();
}

void MainWindow::flasherFinished(int code)
{
    if (!code)
        indicate("ok");
    else
        indicate("fail");
}

void MainWindow::indicate(QString state)
{
    if (state == "ok") {
        ui->label_fail->setPixmap(QPixmap(":/img/img/circle_fail_inactive_40px.png"));
        ui->label_ok->setPixmap(QPixmap(":/img/img/circle_ok_active_40px.png"));
    } else if (state == "fail") {
        ui->label_fail->setPixmap(QPixmap(":/img/img/circle_fail_active_40px.png"));
        ui->label_ok->setPixmap(QPixmap(":/img/img/circle_ok_inactive_40px.png"));
    } else {
        ui->label_fail->setPixmap(QPixmap(":/img/img/circle_fail_inactive_40px.png"));
        ui->label_ok->setPixmap(QPixmap(":/img/img/circle_ok_inactive_40px.png"));
    }
}
