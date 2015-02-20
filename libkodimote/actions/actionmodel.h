#ifndef ACTIONMODEL_H
#define ACTIONMODEL_H

#include <QSortFilterProxyModel>

#include "action.h"

class ActionModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ActionModel(Action::UseCases useCases, QObject *parent = 0);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    Action::UseCases m_useCases;
};

#endif // ACTIONMODEL_H
