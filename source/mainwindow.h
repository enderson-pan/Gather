#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class InfoGetter;
class TreeModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createActions();

private slots:
    void setupTreeModelData();

private:
    QToolBar *tooBar_;
    QAction *setupTreeModelDataAction_;

private:
    Ui::MainWindow *ui;
    TreeModel *treeModel_;
};

#endif // MAINWINDOW_H
