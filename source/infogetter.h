#ifndef INFOGETTER_H
#define INFOGETTER_H

#include <QtGlobal>
#include <QVector>
#include <QStringList>
#include <QTextStream>

class TreeItem;
class StateMachine;

class InfoGetter
{
public:
    explicit InfoGetter(TreeItem *rootItem = 0);
    ~InfoGetter();

    void setupModelData();

private:
    StateMachine *stateMachine_;
};


class StateMachine
{
public:
    enum StateEnum{Init, Prefecture, District, Ids};

    explicit StateMachine(TreeItem *rootItem = 0);
    ~StateMachine();

    bool run(const QString &line, QTextStream &in);

private:
    bool doRun(QStringList &header, QTextStream &in);

private:
    TreeItem *rootItem_;
    QList<TreeItem*> itemStack_;
};

#endif // INFOGETTER_H
