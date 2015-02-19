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

void ChangeHostAction::execute()
{
    emit m_manager->openPage("changeHost");
}
