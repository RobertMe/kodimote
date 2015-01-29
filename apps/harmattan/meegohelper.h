/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Kodimote                                           *
 *                                                                           *
 * Kodimote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Kodimote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef MEEGOHELPER_H
#define MEEGOHELPER_H

#include <QObject>
#include <QTimer>

#include <qmsystem2/qmkeys.h>
#include <qmsystem2/qmdisplaystate.h>
#include <qmsystem2/qmbattery.h>
#include <resource/qt4/policy/resource-set.h>
#include <QtDBus/QDBusObjectPath>

#include <TransferUI/Client>

class Settings;
class KodiDownload;

class MeeGoHelper : public QObject
{
    Q_OBJECT
public:
    explicit MeeGoHelper(Settings *settings, QObject *parent = 0);

signals:

private slots:
    void keyEvent(MeeGo::QmKeys::Key,MeeGo::QmKeys::State);
    bool eventFilter(QObject *obj, QEvent *event);
    void callEvent(const QDBusObjectPath &, const QString &);
    void callTerminated();

    void connectionChanged(bool connected);
    void hostRemoved();
    void internalConnect();

    void displaySettingChanged();

    void setBlankingPause();

    void downloadAdded(KodiDownload *download);
    void downloadProgress();
    void downloadStarted();
    void downloadDone(bool success);
    void cancelTransfer();

private:
    Settings *m_settings;
    MeeGo::QmKeys m_keys;
    ResourcePolicy::ResourceSet *m_resouceSet;
    bool m_buttonsAcquired;
    QTimer m_displayBlankingTimer;
    MeeGo::QmDisplayState m_disaplyState;
    MeeGo::QmBattery *m_battery;
    int m_connectToIndex;
    bool m_mustConnect;

    bool m_videoPaused;
    bool m_musicPaused;

    TransferUI::Client *m_transferClient;
    QMap<KodiDownload*, TransferUI::Transfer*> m_transferMap;
    QDateTime m_lastTransferUpdate;
};

#endif // MEEGOHELPER_H
