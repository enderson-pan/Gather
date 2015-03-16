#ifndef INFOGETTER_H
#define INFOGETTER_H

class TreeItem;

class InfoGetter
{
public:
    explicit InfoGetter();
    ~InfoGetter();

    void setupModelData(TreeItem *treeItem);
};

#endif // INFOGETTER_H
