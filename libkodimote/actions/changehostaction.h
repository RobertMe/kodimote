#ifndef CHANGEHOSTACTION_H
#define CHANGEHOSTACTION_H

#include "action.h"

class ChangeHostAction : public Action
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ChangeHostAction(ActionManager *manager);

    ~ChangeHostAction();

    QString icon() const;
    QString title() const;

    Q_INVOKABLE void execute();
};

#endif // CHANGEHOSTACTION_H
