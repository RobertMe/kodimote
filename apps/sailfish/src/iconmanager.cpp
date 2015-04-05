#include "iconmanager.h"

IconManager::IconManager(QObject *parent) :
    QObject(parent)
{
    m_icons.insert("addHost", "image://theme/icon-cover-new");
    m_icons.insert("changeHost", "image://theme/icon-cover-search");
    m_icons.insert("musicLibrary", "image://theme/icon-l-music");
    m_icons.insert("videoLibrary", "image://theme/icon-l-video");
    m_icons.insert("play", "image://theme/icon-cover-play");
    m_icons.insert("pause", "image://theme/icon-cover-pause");
    m_icons.insert("stop", "../icons/icon-cover-stop.png");
}

QString IconManager::get(const QString &iconName) const
{
    return m_icons[iconName];
}
