#ifndef ACTION_H
#define ACTION_H

#include <QObject>

class ActionManager;

class Action : public QObject
{
    Q_OBJECT
    Q_ENUMS(UseCase)
    Q_FLAGS(UseCases)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
public:
    enum UseCase {
        UseCaseDisconnected = 1,
        UseCaseConnected = 2,
        UseCasePlaying = 4
    };
    Q_DECLARE_FLAGS(UseCases, UseCase)

    explicit Action(ActionManager *manager);

    virtual void attach() { }
    virtual void detach() { }

    virtual QString icon() const = 0;
    virtual QString title() const = 0;

    Q_INVOKABLE virtual void execute() = 0;

signals:
    void iconChanged();
    void titleChanged();

protected:
    ActionManager *m_manager;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Action::UseCases)
Q_DECLARE_METATYPE(Action::UseCase)

#endif // ACTION_H
