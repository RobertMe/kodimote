#include "playpauseaction.h"

#include "actionmanager.h"

#include "../kodi.h"
#include "../player.h"

PlayPauseAction::PlayPauseAction(ActionManager *manager) :
    Action(manager)
{
}

PlayPauseAction::~PlayPauseAction()
{
}

QString PlayPauseAction::icon() const
{
    return "play";
}

QString PlayPauseAction::title() const
{
    return tr("Play/Pause");
}

void PlayPauseAction::execute()
{
    emit Kodi::instance()->activePlayer()->playPause();
}
