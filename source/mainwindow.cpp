#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTreeView>
#include <QTimer>
#include <QtDebug>


#include "treemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    treeModel_(new TreeModel)

{
    ui->setupUi(this);

    createActions();

    QTreeView *view = new QTreeView;
    view->setModel(treeModel_);
    view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setCentralWidget(view);

    QTimer::singleShot(500, this, SLOT(setupTreeModelData()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete treeModel_;
}

void MainWindow::createActions()
{

}

void MainWindow::setupTreeModelData()
{
    treeModel_->setupModelData();
    update();
}



























