#include "stopplaybackaction.h"

#include "../kodi.h"
#include "../player.h"

StopPlaybackAction::StopPlaybackAction(ActionManager *manager) :
    Action(manager)
{
}

QString StopPlaybackAction::icon() const
{
    return "stop";
}

QString StopPlaybackAction::title() const
{
    return tr("Stop playback");
}

Action::UseCases StopPlaybackAction::useCases() const
{
    return UseCasePlaying;
}

void StopPlaybackAction::execute()
{
    emit Kodi::instance()->activePlayer()->stop();
}
