#ifndef DATAFILESMONITOR_H
#define DATAFILESMONITOR_H

#include <QObject>
#include <QDir>
#include <QFileInfoList>

class InfoGetter;

class TreeItem;

class DataFilesMonitor : public QObject
{
    Q_OBJECT

public:
    explicit DataFilesMonitor(TreeItem *rootItem, QObject *parent = 0);
    ~DataFilesMonitor();

    InfoGetter* infoGetter() {return infoGetter_;}

    bool isModified() {return filesNeedParsedList_.isEmpty();}

    QFileInfoList dataFiles() const {return filesNeedParsedList_;}

public slots:
    void requestDataFiles();
    void monitDataFiles();

signals:
    void updatModleDone();

private:
    QDir dir_;
    QFileInfoList fileInfoHistoryList_;
    QFileInfoList filesNeedParsedList_;

    InfoGetter* infoGetter_;
};

#endif // DATAFILESMONITOR_H
