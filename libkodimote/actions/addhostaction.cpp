#include "addhostaction.h"

#include "actionmanager.h"

AddHostAction::AddHostAction(ActionManager *manager) :
    Action(manager)
{
}

QString AddHostAction::icon() const
{
    return "addHost";
}

QString AddHostAction::title() const
{
    return tr("Add host");
}

Action::UseCases AddHostAction::useCases() const
{
    return UseCaseDisconnected | UseCaseConnected | UseCasePlaying;
}

void AddHostAction::execute()
{
    emit m_manager->openPage("addHost");
}
