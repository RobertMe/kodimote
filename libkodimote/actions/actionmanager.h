#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include <QMap>

#include "action.h"

class ActionModel;

class ActionManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(Action::UseCase)
public:
    explicit ActionManager(QObject *parent = 0);

    void registerAction(const QMetaObject *actionType, Action::UseCases useCases);

    Q_INVOKABLE Action *get(const QString &actionDescription);

    Q_INVOKABLE ActionModel *list(Action::UseCase useCase);

signals:
    void openPage(QString pageName);

private:
    Action *createAction(const QMetaObject *actionMeta);

    QMap<QString, QPair<const QMetaObject*, Action::UseCases> > m_actions;
};

#endif // ACTIONMANAGER_H
