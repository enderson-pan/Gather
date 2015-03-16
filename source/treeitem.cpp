#include "treeitem.h"

#include <QList>
#include <QVariant>

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
    : itemData_(data),
      parentItem_(parent)
{
    if (parent)
    {
        parent->appendChild(this);
    }
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems_);
}

void TreeItem::appendChild(TreeItem *item)
{
    childItems_.append(item);
}


TreeItem *TreeItem::child(int row)
{
    return childItems_.value(row);
}

int TreeItem::childCount() const
{
    return childItems_.count();
}

int TreeItem::row() const
{
    if (parentItem_)
        return parentItem_->childItems_.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const
{
    return itemData_.count();
}

QVariant TreeItem::data(int column) const
{
    return itemData_.value(column);
}

TreeItem *TreeItem::parentItem()
{
    return parentItem_;
}

