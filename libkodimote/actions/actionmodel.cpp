#include "actionmodel.h"

ActionModel::ActionModel(QObject *parent) :
    QAbstractItemModel(parent)
{
#ifndef QT5_BUILD
    setRoleNames(roleNames());
#endif
}

void ActionModel::exit()
{
    deleteLater();
}

QVariant ActionModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_actions.count()) {
        return QVariant();
    }

    const Action *action = m_actions[index.row()];
    switch (role) {
    case RoleTitle:
        return action->title();
    case RoleIcon:
        return action->icon();
    default:
        return QVariant();
    }
}

int ActionModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex ActionModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ActionModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ActionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_actions.count();
}

void ActionModel::insertAction(Action *action)
{
    action->setParent(this);
    m_actions.append(action);
}

QHash<int, QByteArray> ActionModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(RoleTitle, "title");
    roleNames.insert(RoleIcon, "icon");

    return roleNames;
}
