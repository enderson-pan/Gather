#include "infogetter.h"
#include "treeitem.h"

#include "QList"
#include "QVariant"

InfoGetter::InfoGetter()
{

}

InfoGetter::~InfoGetter()
{

}

void InfoGetter::setupModelData(TreeItem *treeItem)
{
    QStringList server;
    server << "台服" << "国服";

    for (int i = 0; i < server.size(); ++i)
    {
        QList<QVariant> nodeData;
        nodeData.push_front(server[i]);
        nodeData << 0 << 0 << 0 << 0 << 0;

        new TreeItem(nodeData, treeItem);
    }

    TreeItem *parent = treeItem->child(0);
    QList<QVariant> id;
    id << "g845673" << 0 << 0 << 0 << 0 << 0;
    new TreeItem(id, parent);
}








