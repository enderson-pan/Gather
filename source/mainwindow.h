#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class InfoGetter;
class TreeModel;
class DataFilesMonitor;
class QTreeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateTreeView();

private:
    void createActions();

    void requestData();

private:
    Ui::MainWindow *ui;
    QTreeView *treeView_;
    QToolBar *tooBar_;
    QAction *setupTreeModelDataAction_;

    TreeModel *treeModel_;
};

#endif // MAINWINDOW_H
