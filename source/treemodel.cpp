#include "treemodel.h"
#include "treeitem.h"
#include "infogetter.h"
#include "datafilesmonitor.h"

#include <QTimer>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << tr("服务器\\用户") \
            << tr("可能的五级宝石数") << tr("可能的四级宝石数") << tr("可能的三级宝石数") \
             << tr("可能的金色强化石数") << tr("可能的橙色强化石数") << tr("设备号");
    rootItem_ = new TreeItem(rootData);

    dataFilesMonitor_ = new DataFilesMonitor(rootItem_);
    dataFilesMonitor_->moveToThread(&workingThread_);
    connect(this, SIGNAL(requestDataFiles()), dataFilesMonitor_, SLOT(requestDataFiles()));
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), dataFilesMonitor_, SLOT(monitDataFiles()));
    connect(dataFilesMonitor_, SIGNAL(updatModleDone()), this, SLOT(setUpModelData()));

    emit requestDataFiles();

    setUpModelStruct();

    workingThread_.start();
    timer_->start(1000);
}

TreeModel::~TreeModel()
{
    delete rootItem_;
    workingThread_.quit();
    workingThread_.wait();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem_;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem_)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem_;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem_->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem_->data(section);

    return QVariant();
}

void TreeModel::setUpModelStruct()
{
    dataFilesMonitor_->infoGetter()->setupModelStruct();
}

void TreeModel::setUpModelData()
{
    // update the whole tree views.
    emit dataChanged(QModelIndex(), QModelIndex());
}

void TreeModel::sort(int column, Qt::SortOrder order)
{
    if (0 == column || 6 == column)
    {
        return;
    }

    struct lessThan
    {
        explicit lessThan(int column) { column_ = column;}
        bool operator()(const TreeItem *rhs, const TreeItem *lhs)
        {
            return rhs->data(column_).toInt() < lhs->data(column_).toInt();
        }

        int column_;
    };

    int numPrefecture = rootItem_->childCount();
    for (int prefectureCount = 0; prefectureCount < numPrefecture; ++prefectureCount)
    {
        TreeItem *prefectureItem = rootItem_->child(prefectureCount);
        int numDistrict = prefectureItem->childCount();
        for (int districtCount = 0; districtCount < numDistrict; ++districtCount)
        {
            TreeItem *districtItem = prefectureItem->child(districtCount);
            std::sort(districtItem->children().begin(), districtItem->children().end(),
                      lessThan(column));
            if (Qt::DescendingOrder == order)
            {
                QList<TreeItem*> reversedChildren;
                std::reverse_copy(districtItem->children().begin(), districtItem->children().end(),
                                  std::back_inserter(reversedChildren));
                std::copy(reversedChildren.begin(), reversedChildren.end(),
                          districtItem->children().begin());
            }

            // update the whole tree views.
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }
}

void TreeModel::sortByColumn(int column, Qt::SortOrder order)
{
    sort(column, order);
}














