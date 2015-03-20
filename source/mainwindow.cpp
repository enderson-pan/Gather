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

    treeView_ = new QTreeView;
    treeView_->setModel(treeModel_);
    treeView_->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView_->setSortingEnabled(true);
    setCentralWidget(treeView_);

    QHeaderView *headerView = treeView_->header();
    connect(headerView, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            treeModel_, SLOT(sortByColumn(int, Qt::SortOrder)));
    connect(treeModel_, SIGNAL(updateTreeView()), this, SLOT(updateTreeView()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete treeModel_;
}

void MainWindow::createActions()
{

}

void MainWindow::updateTreeView()
{
    treeView_->update();
}

















