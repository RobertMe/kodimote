#ifndef OPENAUDIOLIBRARYACTION_H
#define OPENAUDIOLIBRARYACTION_H

#include "action.h"

class OpenAudioLibraryAction : public Action
{
    Q_OBJECT
public:
    explicit OpenAudioLibraryAction(ActionManager *manager);

    QString identifier() const { return "musicLibrary"; }
    QString icon() const;
    QString title() const;
    UseCases useCases() const;

    Q_INVOKABLE void execute();
};

#endif // OPENAUDIOLIBRARYACTION_H
