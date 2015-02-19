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
    QObject(parent)
{
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
