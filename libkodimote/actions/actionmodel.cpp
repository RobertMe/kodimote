#include "actionmodel.h"

#include "actionmanager.h"

ActionModel::ActionModel(Action::UseCases useCases, QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_useCases = useCases;
}

bool ActionModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent)) {
        return false;
    }

    ActionManager *manager = qobject_cast<ActionManager*>(sourceModel());
    if (!manager) {
        return false;
    }

    return manager->get(source_row)->useCases() & m_useCases;
}

Action *ActionModel::get(int row) const
{
    ActionManager *manager = qobject_cast<ActionManager*>(sourceModel());
    if (!manager) {
        return 0;
    }

    return manager->get(mapToSource(index(row, 0, QModelIndex())).row());
}

int ActionModel::indexOf(const QString &identifier) const
{
    ActionManager *manager = qobject_cast<ActionManager*>(sourceModel());
    if (!manager) {
        return -1;
    }

    int index = manager->indexOf(identifier);
    return mapFromSource(manager->index(index, 0, QModelIndex())).row();
}
