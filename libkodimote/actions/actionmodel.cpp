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
