#include "action.h"

#include "actionmanager.h"

Action::Action(ActionManager *manager) :
    QObject(manager)
{
    m_manager = manager;
}
