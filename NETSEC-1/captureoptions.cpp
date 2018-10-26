#include "captureoptions.h"

captureoptions::captureoptions(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant captureoptions::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex captureoptions::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex captureoptions::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int captureoptions::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int captureoptions::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant captureoptions::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
