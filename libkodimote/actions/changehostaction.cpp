#include "changehostaction.h"

#include "actionmanager.h"

ChangeHostAction::ChangeHostAction(ActionManager *manager) :
    Action(manager)
{
}

ChangeHostAction::~ChangeHostAction()
{
}

QString ChangeHostAction::icon() const
{
    return "changeHost";
}

QString ChangeHostAction::title() const
{
    return tr("Change host");
}

Action::UseCases ChangeHostAction::useCases() const
{
    return UseCaseDisconnected | UseCaseConnected | UseCasePlaying;
}

void ChangeHostAction::execute()
{
    emit m_manager->openPage("changeHost");
}
