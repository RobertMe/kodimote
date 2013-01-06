/*****************************************************************************
 * Copyright: 2011 Michael Zanetti <mzanetti@kde.org>                        *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include "seasons.h"
#include "episodes.h"
#include "xbmcconnection.h"
#include "xbmc.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"

Seasons::Seasons(int tvshowid, XbmcModel *parent):
    XbmcLibrary(parent),
    m_tvshowid(tvshowid),
    m_refreshing(false)
{
    connect(XbmcConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
}

void Seasons::receivedAnnouncement(const QVariantMap &map)
{
    QString method = map.value("method").toString();

    if(method != "VideoLibrary.OnUpdate")
        return;

    QVariantMap data = map.value("params").toMap().value("data").toMap();
    QString type = data.value("item").toMap().value("type").toString();
    if(type != "episode") {
        return;
    }

    QVariant playcount = map.value("params").toMap().value("data").toMap().value("playcount");
    if(!playcount.isValid() || playcount.toInt() < 0) {
        return;
    }

    if(m_refreshing) {
        return;
    }

    QVariantMap params;
    if(m_tvshowid != -1) {
      params.insert("tvshowid", m_tvshowid);
    }
    QVariantList properties;
    properties.append("season");
    properties.append("playcount");
    params.insert("properties", properties);

    XbmcConnection::sendCommand("VideoLibrary.GetSeasons", params, this, "playcountReceived");
    m_refreshing = true;
}

void Seasons::playcountReceived(const QVariantMap &rsp)
{
    m_refreshing = false;
    QVariantList seasons = rsp.value("result").toMap().value("seasons").toList();
    foreach(const QVariant &season, seasons) {
        QVariantMap seasonMap = season.toMap();

        int i = m_seasonIndexMapping.value(seasonMap.value("season").toInt());
        LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(i));
        item->setPlaycount(seasonMap.value("playcount").toInt());
    }
    dataChanged(index(0, 0, QModelIndex()), index(m_list.count() - 1, 0, QModelIndex()));
}

void Seasons::refresh()
{
    QVariantMap params;
    if(m_tvshowid != -1) {
      params.insert("tvshowid", m_tvshowid);
    }
    QVariantList properties;
    properties.append("showtitle");
    properties.append("season");
    properties.append("thumbnail");
    properties.append("playcount");
    params.insert("properties", properties);

    QVariantMap sort;
    sort.insert("method", "label");
    sort.insert("order", "ascending");
    params.insert("sort", sort);

    XbmcConnection::sendCommand("VideoLibrary.GetSeasons", params, this, "listReceived");
    m_refreshing = true;
}

void Seasons::listReceived(const QVariantMap &rsp)
{
    m_refreshing = false;
    setBusy(false);
    QList<XbmcModelItem*> list;
    qDebug() << "got Seasons:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("seasons").toList();
    int i = 0;
    m_seasonIndexMapping.clear();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem();
        item->setTitle(itemMap.value("label").toString());
        item->setSubtitle(itemMap.value("showtitle").toString());
        item->setSeasonId(itemMap.value("season").toInt());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        item->setPlaycount(itemMap.value("playcount").toInt());
        item->setIgnoreArticle(ignoreArticle());
        item->setFileType("directory");
        item->setPlayable(false);
        list.append(item);
        m_seasonIndexMapping.insert(item->seasonId(), i++);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}

XbmcModel *Seasons::enterItem(int index)
{
    return new Episodes(m_tvshowid, m_list.at(index)->data(RoleSeasonId).toInt(), m_list.at(index)->title(), this);
}

void Seasons::playItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Seasons: playing whole season not supported by xbmc";
}

void Seasons::addToPlaylist(int row)
{
    Q_UNUSED(row)
    qDebug() << "Seasons: playing whole season not supported by xbmc";
}

QString Seasons::title() const
{
    return tr("Seasons");
}