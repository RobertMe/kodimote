#include "openvideolibraryaction.h"

#include "actionmanager.h"
#include "videolibrary.h"

OpenVideoLibraryAction::OpenVideoLibraryAction(ActionManager *manager) :
    Action(manager)
{
}

QString OpenVideoLibraryAction::icon() const
{
    return "videoLibrary";
}

QString OpenVideoLibraryAction::title() const
{
    return VideoLibrary::tr("Videos");
}

Action::UseCases OpenVideoLibraryAction::useCases() const
{
    return UseCaseConnected | UseCasePlaying;
}

void OpenVideoLibraryAction::execute()
{
    emit m_manager->tryOpenBrowser(new VideoLibrary());
}
