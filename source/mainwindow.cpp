#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QHBoxLayout>

#include "infogetter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    infoGetter_(new InfoGetter)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete infoGetter_;
}

void MainWindow::on_refreshButton__clicked()
{
    QLabel* label = ui->textLable_;
    label->setText("Output info here!");
}
