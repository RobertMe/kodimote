#ifndef ACTION_H
#define ACTION_H

#include <QObject>

class ActionManager;

class Action : public QObject
{
    Q_OBJECT
    Q_ENUMS(UseCase)
    Q_FLAGS(UseCases)
    Q_PROPERTY(QString identifier READ identifier)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(UseCases useCases READ useCases NOTIFY useCasesChanged)
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

    virtual QString identifier() const = 0;
    virtual QString icon() const = 0;
    virtual QString title() const = 0;
    virtual UseCases useCases() const = 0;

    Q_INVOKABLE virtual void execute() = 0;

signals:
    void iconChanged();
    void titleChanged();
    void useCasesChanged();

protected:
    ActionManager *m_manager;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Action::UseCases)
Q_DECLARE_METATYPE(Action::UseCase)
Q_DECLARE_METATYPE(Action::UseCases)

#endif // ACTION_H
