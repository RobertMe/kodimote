#ifndef OPENVIDEOLIBRARYACTION_H
#define OPENVIDEOLIBRARYACTION_H

#include "action.h"

class OpenVideoLibraryAction : public Action
{
    Q_OBJECT
public:
    explicit OpenVideoLibraryAction(ActionManager *manager);

    QString identifier() const { return "videoLibrary"; }
    QString icon() const;
    QString title() const;
    UseCases useCases() const;

    Q_INVOKABLE void execute();
};

#endif // OPENVIDEOLIBRARYACTION_H
