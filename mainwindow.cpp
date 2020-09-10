#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    if (ui->widget->match(text))
        ui->pushButton->setText("验证通过");
    else
        ui->pushButton->setText("验证失败");
}
