#ifndef INFOGETTER_H
#define INFOGETTER_H

#include <QtGlobal>
#include <QVector>
#include <QStringList>
#include <QTextStream>
#include <QFileInfoList>

class TreeItem;

class InfoGetter
{
public:
    explicit InfoGetter(TreeItem *rootItem = 0);
    ~InfoGetter();

    void setupModelStruct();
    void setupModelData(QFileInfoList &filesInfoList);


private:
    QStringList getHeader(const QString &line);

    bool setUpDataStruct(const QString &line, QTextStream &in);
    bool doSetUpDataStruct(const QStringList &header, QTextStream &in);

    bool setUpDataContent(QTextStream &in);
    bool doSetUpDataContent(QString &id, QString &gemstoneCredits,
                            QString &strengthenstoneCredits, QString &equipNumber);
    TreeItem* findItem(const QString &id);
    void doSetUpGemstoneContent(TreeItem *dataItem, QString &gemstoneCredits);
    void doSetUpStrengthenstoneConent(TreeItem *dataItem, QString &strengthenstoneCredits);
    void doSetUpEquipNumberContent(TreeItem *dataItem, QString &equipNumber);
    bool checkSum();

private:
    TreeItem *rootItem_;
    QList<TreeItem*> itemStack_;
};

#endif // INFOGETTER_H
