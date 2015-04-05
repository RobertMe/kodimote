#ifndef ADDHOSTACTION_H
#define ADDHOSTACTION_H

#include "action.h"

class AddHostAction : public Action
{
    Q_OBJECT
public:
    explicit AddHostAction(ActionManager *manager);

    QString identifier() const { return "addHost"; }
    QString icon() const;
    QString title() const;
    UseCases useCases() const;

    Q_INVOKABLE void execute();
};

#endif // ADDHOSTACTION_H
