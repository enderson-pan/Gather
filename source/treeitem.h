#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column, const QVariant &data);
    int row() const;
    TreeItem *parentItem();
    QList<TreeItem*>& children();

private:
    QList<QVariant> itemData_;
    QList<TreeItem*> childItems_;
    TreeItem *parentItem_;
};

#endif // TREEITEM_H
