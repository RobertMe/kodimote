#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <QHash>

class IconManager : public QObject
{
    Q_OBJECT
public:
    explicit IconManager(QObject *parent = 0);

    Q_INVOKABLE QString get(const QString &iconName) const;

private:
    QHash<QString, QString> m_icons;
};

#endif // ICONMANAGER_H
