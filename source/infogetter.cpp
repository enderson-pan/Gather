#include "infogetter.h"
#include "treeitem.h"

#include <QList>
#include <QVariant>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QFileInfoList>

#include <cmath>

//
// InfoGetter
//
InfoGetter::InfoGetter(TreeItem *rootItem)
    : rootItem_(rootItem)
{

}

InfoGetter::~InfoGetter()
{

}

void InfoGetter::setupModelStruct()
{
    QString filePath = QDir::homePath();
    filePath += "/workspace/game-info";
    QDir::setCurrent(filePath);
    QFile infoListFile("Infolist.txt");
    if (!infoListFile.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("File does not exists!");
        msgBox.exec();
        return;
    }
    if (!infoListFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Open Infolist.txt failed!");
        msgBox.exec();
        return;
    }

    // Establish the frame.
    QTextStream in(&infoListFile);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (!line.isEmpty())
        {
            if (!setUpDataStruct(line, in))
                break;
        }
    }

    infoListFile.close();
}

bool InfoGetter::setUpDataStruct(const QString &line, QTextStream &in)
{
    QStringList header = getHeader(line);

    return doSetUpDataStruct(header, in);
}

QStringList InfoGetter::getHeader(const QString &line)
{
    int index = line.indexOf("[");

    QStringList header;
    if (-1 ==  index)   // An id.
    {
        header << line;
    }
    else                // A node or end of a node.
    {
        int endIndex = line.indexOf("]");
        QString token = line.mid(index + 1, endIndex - index - 1);
        header << token;

        int contentIndex = line.indexOf(":");
        if (-1 != contentIndex)
        {
            QString content = line.mid(contentIndex + 1);
            content = content.simplified();
            header << content;
        }
    }

    return header;
}

bool InfoGetter::doSetUpDataStruct(const QStringList &header, QTextStream &in)
{
    if (QString("prefecture") == header[0])
    {
        itemStack_.push_front(rootItem_);

        QList<QVariant> itemData;
        itemData << header[1] << " " << " " << " " << " " << " " << " ";
        TreeItem *item =  new TreeItem(itemData, itemStack_.front());
        itemStack_.push_front(item);

        return true;
    }
    else if (QString("district") == header[0])
    {
        QList<QVariant> itemData;
        itemData << header[1] << 0 << 0 << 0 << 0 << 0 << "";
        TreeItem *item =  new TreeItem(itemData, itemStack_.front());
        itemStack_.push_front(item);

        return true;
    }
    else if (QString("ids") == header[0])
    {
        QList<QVariant> itemData;
        itemData << header[1] << "" << "" << "" << "" << "" << "";
        new TreeItem(itemData, itemStack_.front());

        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (!line.isEmpty())
            {
                QString idsEndStr("/ids");
                if (!line.contains(idsEndStr))
                {
                    QList<QVariant> itemData;
                    itemData << line << "" << "" << "" << "" << "" << "";
                    new TreeItem(itemData, itemStack_.front());
                }
                else
                {
                    break;
                }

            }
        }
    }
    else if (QString("/district") == header[0])
    {
        itemStack_.pop_front();
    }
    else if (QString("/prefecture") == header[0])
    {
        itemStack_.pop_front();
    }

    return true;
}

void InfoGetter::setupModelData()
{
    QString filesPath = QDir::homePath();
    filesPath += "/workspace/game-info/data";
    QDir::setCurrent(filesPath);
    QDir dir(filesPath);

    // Get all files' info in this dir.

    QFileInfoList filesInfoList = dir.entryInfoList(QDir::Files);

    if (filesInfoList.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("No data files!");

        return;
    }

    for (int i = 0; i < filesInfoList.size(); ++i)
    {
        QFile dataFile(filesInfoList[i].fileName());
        if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("Open data file failed!");
            msgBox.exec();
            return;
        }

        QTextStream in(&dataFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList header = getHeader(line);
            if (QString("entry") == header[0])
            {
                if(!setUpDataContent(in))
                    break;
            }
        }

        dataFile.close();
    }

    if (!checkSum())
    {
        QMessageBox msgBox;
        msgBox.setText("宝石或者强化石总数与上报数之和不符，"
                       "请检查数据文件中是否有id重复,"
                       "或者是否所有id的数据都已经上传。");
        msgBox.exec();
    }
}

bool InfoGetter::setUpDataContent(QTextStream &in)
{
    QString id;
    QString equipNumber;
    QString gemstoneCredits;
    QString strengthenstoneCredits;

    QString line = in.readLine();
    QStringList header = getHeader(line);
    while (QString("/entry") != header[0])
    {
        if (QString("id") == header[0])
        {
            id = header[1];
        }
        else if (QString("equipment number") == header[0])
        {
            equipNumber = header[1];
        }
        else if (QString("gemstome credits") == header[0])
        {
            gemstoneCredits = header[1];
        }
        else if (QString("strengthenstone credits") == header[0])
        {
            strengthenstoneCredits = header[1];
        }
        else
        {
            return false;
        }

        line = in.readLine();
        header = getHeader(line);
    }

    return doSetUpDataContent(id, gemstoneCredits, strengthenstoneCredits, equipNumber);
}

bool InfoGetter::doSetUpDataContent(QString &id, QString &gemstoneCredits,
                                    QString &strengthenstoneCredits, QString &equipNumber)
{
    TreeItem *dataItem = findItem(id);

    if (0 == dataItem)
    {
        QMessageBox msgBox;
        msgBox.setText(QString("用户 ID:%1 不存在，请检查上传数据文件。").arg(id));
        msgBox.exec();
        return true;
    }

    doSetUpGemstoneContent(dataItem, gemstoneCredits);
    doSetUpStrengthenstoneConent(dataItem, strengthenstoneCredits);
    doSetUpEquipNumberContent(dataItem, equipNumber);

    return true;
}

TreeItem* InfoGetter::findItem(const QString &id)
{
    int numPrefecture = rootItem_->childCount();
    for (int prefectureCount = 0; prefectureCount < numPrefecture; ++prefectureCount)
    {
        TreeItem *prefecture = rootItem_->child(prefectureCount);
        int districtNum = prefecture->childCount();
        for (int districtCount = 0; districtCount < districtNum; ++districtCount)
        {
            TreeItem *district = prefecture->child(districtCount);
            int idNum = district->childCount();
            for (int idCount = 0; idCount < idNum; ++idCount)
            {
                TreeItem *idItem = district->child(idCount);
                if (id == idItem->data(0).toString())
                    return idItem;
            }
        }
    }
    return 0;
}

void InfoGetter::doSetUpGemstoneContent(TreeItem *dataItem, QString &gemstoneCredits)
{
    int numGemCredits = gemstoneCredits.toInt();
    for (int i = 0; i < 3; ++i)
    {
        int base = pow(5, 4 - i);
        int numGem = numGemCredits / base;
        dataItem->setData(1 + i, numGem);
        numGemCredits = numGemCredits - numGem * base;
    }

    TreeItem *distrctItem = dataItem->parentItem();
    for (int i = 0; i < 3; ++i)
    {
        int total = distrctItem->data(i + 1).toInt();
        total += dataItem->data(i + 1).toInt();
        distrctItem->setData(i + 1, total);
    }

}

void InfoGetter::doSetUpStrengthenstoneConent(TreeItem *dataItem, QString &strengthenstoneCredits)
{
    int numStrengthenCredits = strengthenstoneCredits.toInt();
    for (int i = 0; i < 3; ++i)
    {
        int base = pow(5, 4 - i);
        int numStrengthen = numStrengthenCredits / base;
        dataItem->setData(4 + i, numStrengthen);
        numStrengthenCredits = numStrengthenCredits - numStrengthen * base;
    }

    TreeItem *distrctItem = dataItem->parentItem();
    for (int i = 0; i < 2; ++i)
    {
        int total = distrctItem->data(i + 4).toInt();
        total += dataItem->data(i + 4).toInt();
        distrctItem->setData(i + 4, total);
    }
}

void InfoGetter::doSetUpEquipNumberContent(TreeItem *dataItem, QString &equipNumber)
{
    dataItem->setData(6, equipNumber);
}

bool InfoGetter::checkSum()
{
    int numPrefectur = rootItem_->childCount();
    for (int i = 0; i < numPrefectur; ++i)
    {
        TreeItem *prefectureItem = rootItem_->child(i);
        int numDistrict = prefectureItem->childCount();
        for (int j = 0; j < numDistrict; ++j)
        {
            TreeItem *districtItem = prefectureItem->child(j);
            int column = districtItem->columnCount();
            QList<int> totalRead;
            for (int k = 1; k < column - 1; ++k)
            {
                bool isOk = false;
                int total = districtItem->data(k).toInt(&isOk);
                totalRead.push_back(total);
            }

            int numId = districtItem->childCount();
            QList<int> totalCalc;
            for (int k = 1; k < column - 1; ++k)
            {
                int total = 0;
                for (int m = 0; m < numId; ++m)
                {
                    TreeItem *idItem = districtItem->child(m);
                    bool isOk = false;
                    int data = idItem->data(k).toInt(&isOk);
                    if (!isOk)
                    {
                        QMessageBox msgBox;
                        msgBox.setText(QString("id: %1 列: %2 无法转换为整型，"
                                               "请检查数据是否存在。").arg(idItem->data(0).toString()).arg(k));
                        msgBox.exec();
                        return false;
                    }
                    total += data;
                }
                totalCalc.push_back(total);
            }
            if (totalRead != totalCalc)
            {
                return false;
            }
        }
    }

    return true;
}






















