#include "playpauseaction.h"

#include "../kodi.h"
#include "../player.h"
#include "../audioplayer.h"
#include "../videoplayer.h"

PlayPauseAction::PlayPauseAction(ActionManager *manager) :
    Action(manager)
{
    connect(Kodi::instance()->audioPlayer(), SIGNAL(stateChanged()), this, SIGNAL(iconChanged()));
    connect(Kodi::instance()->videoPlayer(), SIGNAL(stateChanged()), this, SIGNAL(iconChanged()));
    connect(Kodi::instance(), SIGNAL(activePlayerChanged()), this, SIGNAL(iconChanged()));
}

QString PlayPauseAction::icon() const
{
    return Kodi::instance()->activePlayer()->state() == "playing" ? "pause" : "play";
}

QString PlayPauseAction::title() const
{
    return tr("Play/Pause");
}

Action::UseCases PlayPauseAction::useCases() const
{
    return UseCasePlaying;
}

void PlayPauseAction::execute()
{
    emit Kodi::instance()->activePlayer()->playPause();
}
