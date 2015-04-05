#ifndef PLAYPAUSEACTION_H
#define PLAYPAUSEACTION_H

#include "action.h"

class PlayPauseAction : public Action
{
    Q_OBJECT
public:
    explicit PlayPauseAction(ActionManager *manager);

    QString identifier() const { return "playPause"; }
    QString icon() const;
    QString title() const;
    UseCases useCases() const;

    Q_INVOKABLE void execute();
};

#endif // PLAYPAUSEACTION_H
