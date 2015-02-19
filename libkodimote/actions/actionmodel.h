#ifndef ACTIONMODEL_H
#define ACTIONMODEL_H

#include <QAbstractItemModel>

#include "action.h"

class ActionModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleTitle = Qt::DisplayRole,
        RoleIcon = Qt::UserRole + 1
    };

    explicit ActionModel(QObject *parent = 0);

    Q_INVOKABLE void exit();

    QVariant data(const QModelIndex &index, int role) const;

    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QHash<int, QByteArray> roleNames() const;

    void insertAction(Action *action);

private:
    QList<const Action*> m_actions;

};

#endif // ACTIONMODEL_H
