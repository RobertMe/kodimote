#ifndef CHANGEHOSTACTION_H
#define CHANGEHOSTACTION_H

#include "action.h"

class ChangeHostAction : public Action
{
    Q_OBJECT
public:
    explicit ChangeHostAction(ActionManager *manager);

    ~ChangeHostAction();

    QString identifier() const { return "changeHost"; }
    QString icon() const;
    QString title() const;
    UseCases useCases() const;

    Q_INVOKABLE void execute();
};

#endif // CHANGEHOSTACTION_H
