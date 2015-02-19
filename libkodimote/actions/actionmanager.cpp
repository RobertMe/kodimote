#include "actionmanager.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "actionmodel.h"
#include "changehostaction.h"
#include "playpauseaction.h"

ActionManager::ActionManager(QObject *parent) :
    QObject(parent)
{
    registerAction(&ChangeHostAction::staticMetaObject, Action::UseCaseDisconnected | Action::UseCaseConnected | Action::UseCasePlaying);
    registerAction(&PlayPauseAction::staticMetaObject, Action::UseCasePlaying);
}

void ActionManager::registerAction(const QMetaObject* actionType, Action::UseCases useCases)
{
    m_actions.insert(actionType->className(), qMakePair(actionType, useCases));
}

Action *ActionManager::get(const QString &actionDescription)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(actionDescription.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        return NULL;
    }

    QJsonObject description = document.object();

    QString className = description.value("className").toString();

    if (className.isEmpty() || !m_actions.contains(className)) {
        return NULL;
    }

    return createAction(m_actions[className].first);
}

ActionModel *ActionManager::list(Action::UseCase useCase)
{
    ActionModel *model = new ActionModel(this);
    QPair<const QMetaObject*, Action::UseCases> actionPair;
    foreach (actionPair, m_actions) {
        if (actionPair.second.testFlag(useCase)) {
            Action *action = createAction(actionPair.first);
            model->insertAction(action);
        }
    }

    return model;
}

Action *ActionManager::createAction(const QMetaObject *actionMeta)
{
    return static_cast<Action*>(actionMeta->newInstance(Q_ARG(ActionManager*, this)));
}
