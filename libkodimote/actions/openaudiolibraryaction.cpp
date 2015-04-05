#include "openaudiolibraryaction.h"

#include "actionmanager.h"
#include "audiolibrary.h"

OpenAudioLibraryAction::OpenAudioLibraryAction(ActionManager *manager) :
    Action(manager)
{
}

QString OpenAudioLibraryAction::icon() const
{
    return "musicLibrary";
}

QString OpenAudioLibraryAction::title() const
{
    return AudioLibrary::tr("Music");
}

Action::UseCases OpenAudioLibraryAction::useCases() const
{
    return UseCaseConnected | UseCasePlaying;
}

void OpenAudioLibraryAction::execute()
{
    emit m_manager->tryOpenBrowser(new AudioLibrary());
}
