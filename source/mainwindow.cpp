#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTreeView>


#include "treemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    treeModel_(new TreeModel)

{
    ui->setupUi(this);

    QTreeView *view = new QTreeView;
    view->setModel(treeModel_);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete treeModel_;
}


