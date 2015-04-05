#ifndef STOPPLAYBACKACTION_H
#define STOPPLAYBACKACTION_H

#include "action.h"

class StopPlaybackAction : public Action
{
    Q_OBJECT
public:
    explicit StopPlaybackAction(ActionManager *manager);

    QString identifier() const { return "stopPlayback"; }
    QString icon() const;
    QString title() const;
    UseCases useCases() const;

    Q_INVOKABLE void execute();

};

#endif // STOPPLAYBACKACTION_H
