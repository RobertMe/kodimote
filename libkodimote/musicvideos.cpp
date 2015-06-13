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

#include "musicvideos.h"
#include "kodiconnection.h"
#include "kodi.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"

MusicVideos::MusicVideos(bool recentlyAdded, KodiModel *parent) :
    KodiLibrary(parent),
    m_recentlyAdded(recentlyAdded)
{
    connect(KodiConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
}

void MusicVideos::receivedAnnouncement(const QVariantMap &map)
{
    QString method = map.value("method").toString();

    if(method != "VideoLibrary.OnUpdate") {
        return;
    }

    QVariantMap data = map.value("params").toMap().value("data").toMap();

    QString type = data.value("item").toMap().value("type").toString();
    int id = data.value("item").toMap().value("id").toInt();
    if(type != "musicvideo" || !m_idIndexMapping.contains(id)) {
        return;
    }

    refresh();
}

void MusicVideos::refresh()
{
    QVariantMap params;
    QVariantList properties;
    properties.append("fanart");
    properties.append("playcount");
    properties.append("year");
    properties.append("resume");
    params.insert("properties", properties);


    if (m_recentlyAdded) {
        KodiConnection::sendCommand("VideoLibrary.GetMusicVideos", params, this, "listReceived");
    } else {
        QVariantMap sort;
        sort.insert("method", "label");
        sort.insert("order", "ascending");
        sort.insert("ignorearticle", ignoreArticle());
        params.insert("sort", sort);

        KodiConnection::sendCommand("VideoLibrary.GetRecentlyAddedMusicVideos", params, this, "listReceived");
    }
}

void MusicVideos::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("musicvideoid", m_list.at(index)->data(RoleMusicVideoId).toInt());

    QVariantList properties;
//    properties.append("title");
//    properties.append("playcount");
    properties.append("runtime");
//    properties.append("director");
//    properties.append("studio");
    properties.append("year");
    properties.append("plot");
//    properties.append("album");
//    properties.append("artist");
    properties.append("genre");
//    properties.append("track");
//    properties.append("streamdetails");
//    properties.append("lastplayed");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("file");
//    properties.append("resume");

    params.insert("properties", properties);

    int id = KodiConnection::sendCommand("VideoLibrary.GetMusicVideoDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void MusicVideos::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<KodiModelItem*> list;
    qDebug() << "got MusicCideos:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("musicvideos").toList();
    int index = 0;
    m_idIndexMapping.clear();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem(this);
        item->setTitle(itemMap.value("label").toString());
        item->setMusicvideoId(itemMap.value("musicvideoid").toInt());
        item->setThumbnail(itemMap.value("fanart").toString());
        item->setPlaycount(itemMap.value("playcount").toInt());
        item->setIgnoreArticle(ignoreArticle());
        item->setFileType("file");
        item->setPlayable(true);
        item->setYear(itemMap.value("year").toString());
        item->setResume(itemMap.value("resume").toMap().value("position").toInt());
        list.append(item);
        m_idIndexMapping.insert(item->musicvideoId(), index++);
    }
    beginInsertRows(QModelIndex(), 0, list.count() - 1);
    m_list = list;
    endInsertRows();
}


void MusicVideos::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
    QVariantMap details = rsp.value("result").toMap().value("musicvideodetails").toMap();
    //item->setRuntime(details.value("runtime").toInt());
    item->setYear(details.value("year").toString());
    item->setPlot(details.value("plot").toString());
    item->setGenre(details.value("genre").toString());
    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

KodiModel *MusicVideos::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter MusicVideo. Use playItem() to play it";
    return 0;
}

void MusicVideos::playItem(int index, bool resume)
{
    VideoPlaylistItem item;
    item.setMusicVideoId(m_list.at(index)->data(RoleMusicVideoId).toInt());
    Kodi::instance()->videoPlayer()->open(item, resume);
}

void MusicVideos::addToPlaylist(int row)
{
    VideoPlaylistItem pItem;
    pItem.setMusicVideoId(m_list.at(row)->data(RoleMusicVideoId).toInt());
    Kodi::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString MusicVideos::title() const
{
    return tr("Music Videos");
}
