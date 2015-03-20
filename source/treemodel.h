#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QThread>

class TreeItem;
class DataFilesMonitor;
class QTimer;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    void sort(int column, Qt::SortOrder order) Q_DECL_OVERRIDE;

public slots:
    void sortByColumn(int column, Qt::SortOrder order);

signals:
    void requestDataFiles();
    void updateTreeView();

private:
    void setUpModelStruct();

private slots:
    void setUpModelData();

private:
    TreeItem *rootItem_;

    DataFilesMonitor *dataFilesMonitor_;
    QTimer *timer_;
    QThread workingThread_;

};

#endif // TREEMODEL_H
