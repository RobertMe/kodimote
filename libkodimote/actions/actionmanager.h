#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QAbstractItemModel>
#include <QList>

#include "action.h"

class ActionModel;

class ActionManager : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleTitle = Qt::DisplayRole,
        RoleIdentifier = Qt::UserRole + 1,
        RoleIcon,
        RoleUseCases
    };
    explicit ActionManager(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;

    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QHash<int, QByteArray> roleNames() const;

    void registerAction(Action *action);

    Q_INVOKABLE Action *get(const QString &identifier) const;
    Action *get(int row) const;

    Q_INVOKABLE ActionModel *list(Action::UseCase useCases);

signals:
    void openPage(QString pageName);

private:
    QList<Action*> m_actions;
};

#endif // ACTIONMANAGER_H
