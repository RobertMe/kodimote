/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *            2014      Robert Meijers <robert.meijers@gmail.com>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef SAILFISHHELPER_H
#define SAILFISHHELPER_H

#include <QDBusArgument>
#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QObject>

class Settings;

class SailfishHelper : public QObject
{
    Q_OBJECT
public:
    explicit SailfishHelper(Settings *settings, QObject *parent = 0);
    
private slots:
    void callAdded(const QDBusMessage &msg);
    void callRemoved();

private:
    QString lookupContact(QString phoneNumber);
    QMap<QString, QString> unpackMessage(const QDBusArgument &args);

    Settings *m_settings;

    bool m_videoPaused;
    bool m_musicPaused;
};

#endif // SAILFISHHELPER_H
