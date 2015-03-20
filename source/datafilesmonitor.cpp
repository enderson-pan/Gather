#include "datafilesmonitor.h"
#include "infogetter.h"

#include <QTimer>
#include <QFileInfoList>
#include <QDateTime>
#include <QtDebug>

#include <algorithm>


DataFilesMonitor::DataFilesMonitor(TreeItem *rootItem, QObject *parent)
    : QObject(parent),
      infoGetter_(new InfoGetter(rootItem))
{
    QString filesPath = QDir::homePath();
    filesPath += "/workspace/game-info/data";
    QDir::setCurrent(filesPath);
    dir_.setPath(filesPath);
}

DataFilesMonitor::~DataFilesMonitor()
{
    delete infoGetter_;
}

void DataFilesMonitor::requestDataFiles()
{
    // Codes need to be added.
}

void DataFilesMonitor::monitDataFiles()
{
    struct MyFileInfo
    {
        QString fileName;
        QDateTime lastModifiedTime;
    };

    QFileInfoList fileInfoList = dir_.entryInfoList(QDir::Files, QDir::Name);
    QVector<MyFileInfo> fileInfoVector;
    QFileInfo fileInfo;
    foreach (fileInfo, fileInfoList)
    {
        MyFileInfo e;
        e.fileName = fileInfo.fileName();
        e.lastModifiedTime = fileInfo.lastModified();
        fileInfoVector.push_back(e);
    }

    QVector<MyFileInfo> fileInfoVectorHistory;
    foreach (fileInfo, fileInfoHistoryList_)
    {
        MyFileInfo e;
        e.fileName = fileInfo.fileName();
        e.lastModifiedTime = fileInfo.lastModified();
        fileInfoVectorHistory.push_back(e);
    }

    for (int i = 0; i < fileInfoVector.size(); ++i)
    {
        int j;
        for (j = 0; j < fileInfoVectorHistory.size(); ++j)
        {
            if (fileInfoVector[i].fileName == fileInfoVectorHistory[j].fileName)
            {
                if (fileInfoVector[i].lastModifiedTime != fileInfoVectorHistory[j].lastModifiedTime)
                {
                    filesNeedParsedList_.push_back(fileInfoList[i]);
                }
                break;
            }
        }
        if (fileInfoVectorHistory.size() == j)
        {
            filesNeedParsedList_.push_back(fileInfoList[i]);
        }
    }

    if (!filesNeedParsedList_.isEmpty())
    {
        infoGetter_->setupModelData(filesNeedParsedList_);
        filesNeedParsedList_.clear();

        fileInfoHistoryList_.clear();
        std::copy(fileInfoList.begin(), fileInfoList.end(), std::back_inserter(fileInfoHistoryList_));

        emit updatModleDone();
    }
}



