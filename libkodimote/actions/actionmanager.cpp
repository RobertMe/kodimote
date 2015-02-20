#include "actionmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#ifdef QT5_BUILD
#include <QtQuick>
#else
#include <QtDeclarative>
#endif

#include "actionmodel.h"
#include "changehostaction.h"
#include "playpauseaction.h"

ActionManager::ActionManager(QObject *parent) :
    QAbstractItemModel(parent)
{
#ifndef QT5_BUILD
    setRoleNames(roleNames());
#endif

#ifdef SAILFISH
    // @uri harbour.kodimote
    const char *qmlUri = "harbour.kodimote";
#else
    // @uri Kodi
    const char *qmlUri = "Kodi";
#endif

    qmlRegisterUncreatableType<Action>(qmlUri, 1, 0, "Action", "Cannot create action. Use the 'actions' ActionManager instead");
    qmlRegisterUncreatableType<ActionModel>(qmlUri, 1, 0, "ActionModel", "Cannot create action model. Use the 'actions.list' instead");
    qRegisterMetaType<Action::UseCase>();

    m_actions
            << new ChangeHostAction(this)
            << new PlayPauseAction(this);
}

QVariant ActionManager::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_actions.count()) {
        return QVariant();
    }

    const Action *action = m_actions.at(index.row());
    switch (role) {
    case RoleIdentifier:
        return action->identifier();
    case RoleTitle:
        return action->title();
    case RoleIcon:
        return action->icon();
    case RoleUseCases:
        return QVariant::fromValue(action->useCases());
    default:
        return QVariant();
    }
}

int ActionManager::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex ActionManager::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ActionManager::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int ActionManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_actions.count();
}

QHash<int, QByteArray> ActionManager::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(RoleIdentifier, "identifier");
    roleNames.insert(RoleTitle, "title");
    roleNames.insert(RoleIcon, "icon");
    roleNames.insert(RoleUseCases, "useCases");

    return roleNames;
}

void ActionManager::registerAction(Action *action)
{
    m_actions.append(action);
}

Action *ActionManager::get(const QString &identifier) const
{
    foreach (Action *action, m_actions) {
        if (action->identifier() == identifier) {
            return action;
        }
    }

    return NULL;
}

Action *ActionManager::get(int row) const
{
    if (row < 0 || row >= m_actions.count()) {
        return NULL;
    }

    return m_actions[row];
}

ActionModel *ActionManager::list(Action::UseCase useCases)
{
    ActionModel *model = new ActionModel(useCases);
    model->setSourceModel(this);
    return model;
}
