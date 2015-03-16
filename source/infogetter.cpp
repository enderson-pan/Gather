#include "infogetter.h"
#include "treeitem.h"

#include <QList>
#include <QVariant>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>

//
// InfoGetter
//
InfoGetter::InfoGetter(TreeItem *rootItem)
    : stateMachine_(new StateMachine(rootItem))
{

}

InfoGetter::~InfoGetter()
{
    if (stateMachine_)
    {
        delete stateMachine_;
        stateMachine_ = 0;
    }
}

void InfoGetter::setupModelData()
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
            if (!stateMachine_->run(line, in))
                break;
        }
    }

    infoListFile.close();
}


//
// StateMachine
//
StateMachine::StateMachine(TreeItem *rootItem)
    : rootItem_(rootItem)
{

}

StateMachine::~StateMachine()
{


}


bool StateMachine::run(const QString &line, QTextStream &in)
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

    return doRun(header, in);
}

bool StateMachine::doRun(QStringList &header, QTextStream &in)
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
        itemData << header[1] << 0 << 0 << 0 << 0 << 0 << 0;
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
                    itemData << line << 0 << 0 << 0 << 0 << 0 << 0;
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
































