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

#include "episodes.h"
#include "kodiconnection.h"
#include "kodi.h"
#include "videoplayer.h"
#include "videoplaylist.h"
#include "videoplaylistitem.h"
#include "libraryitem.h"
#include "kodidownload.h"

Episodes::Episodes(int tvshowid, int seasonid, const QString &seasonString, KodiModel *parent):
    KodiLibrary(parent),
    m_tvshowid(tvshowid),
    m_seasonid(seasonid),
    m_seasonString(seasonString)
{
    connect(KodiConnection::notifier(), SIGNAL(receivedAnnouncement(QVariantMap)), SLOT(receivedAnnouncement(QVariantMap)));
}

void Episodes::receivedAnnouncement(const QVariantMap &map)
{
    QString method = map.value("method").toString();

    if(method != "VideoLibrary.OnUpdate") {
        return;
    }

    QVariantMap data = map.value("params").toMap().value("data").toMap();

    QString type = data.value("item").toMap().value("type").toString();
    int id = data.value("item").toMap().value("id").toInt();
    if(type != "episode" || !m_idIndexMapping.contains(id)) {
        return;
    }

    refresh();
}

void Episodes::refresh()
{
    QVariantMap params;
    if(m_tvshowid >= 0) {
        params.insert("tvshowid", m_tvshowid);
    }
    if(m_seasonid >= 0) {
        params.insert("season", m_seasonid);
    }
    QVariantList properties;
    properties.append("showtitle");
    properties.append("episode");
    properties.append("thumbnail");
    properties.append("playcount");
    properties.append("file");
    properties.append("resume");
    params.insert("properties", properties);

    if (m_tvshowid == KodiModel::ItemIdRecentlyAdded && m_seasonid == KodiModel::ItemIdRecentlyAdded) {
        KodiConnection::sendCommand("VideoLibrary.GetRecentlyAddedEpisodes", params, this, "listReceived");
    } else {
        QVariantMap sort;
        sort.insert("method", "episode");
        sort.insert("order", "ascending");
        params.insert("sort", sort);

        KodiConnection::sendCommand("VideoLibrary.GetEpisodes", params, this, "listReceived");
    }
}

void Episodes::fetchItemDetails(int index)
{
    QVariantMap params;
    params.insert("episodeid", m_list.at(index)->data(RoleEpisodeId).toInt());

    QVariantList properties;
//    properties.append("resume");
//    properties.append("title");

    properties.append("plot");

//    properties.append("votes");
    properties.append("rating");
//    properties.append("writer");
    properties.append("firstaired");
    properties.append("playcount");
    properties.append("runtime");
//    properties.append("director");
//    properties.append("productioncode");
    properties.append("season");
    properties.append("episode");
//    properties.append("originaltitle");
//    properties.append("showtitle");
//    properties.append("cast");
//    properties.append("streamdetails");
//    properties.append("lastplayed");
//    properties.append("fanart");
//    properties.append("thumbnail");
//    properties.append("file");
//    properties.append("resume");
//    properties.append("tvshowid");

    params.insert("properties", properties);

    int id = KodiConnection::sendCommand("VideoLibrary.GetEpisodeDetails", params, this, "detailsReceived");
    m_detailsRequestMap.insert(id, index);
}

void Episodes::download(int index, const QString &path)
{
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(index));

    QString destination = path + "/Movies/" + item->tvShow() + "/Season " + QString::number(item->season()) + '/';
    qDebug() << "should download" << destination;

    KodiDownload *download = new KodiDownload();
    download->setDestination(destination);
    download->setIconId("icon-m-content-videos");
    download->setLabel(item->title());

    startDownload(index, download);
}

void Episodes::listReceived(const QVariantMap &rsp)
{
    setBusy(false);
    QList<KodiModelItem*> list;
    qDebug() << "got Episodes:" << rsp.value("result");
    QVariantList responseList = rsp.value("result").toMap().value("episodes").toList();
    int index = 0;
    m_idIndexMapping.clear();
    foreach(const QVariant &itemVariant, responseList) {
        QVariantMap itemMap = itemVariant.toMap();
        LibraryItem *item = new LibraryItem(this);
        item->setTitle(itemMap.value("episode").toString() + ". " + itemMap.value("label").toString());
        //        item->setData(itemMap.value("showtitle").toString() + " - " + itemMap.value("season").toString(), Qt::UserRole+2);
        item->setSubtitle(itemMap.value("showtitle").toString() + (m_seasonString.isEmpty() ? "" :  (" - " + m_seasonString)));
        item->setTvShow(itemMap.value("showtitle").toString());
        item->setSeason(m_seasonid);
        item->setEpisodeId(itemMap.value("episodeid").toInt());
        item->setThumbnail(itemMap.value("thumbnail").toString());
        item->setPlaycount(itemMap.value("playcount").toInt());
        item->setFileName(itemMap.value("file").toString());
        item->setIgnoreArticle(false); // We ignore the setting here...
        item->setFileType("file");
        item->setPlayable(true);
        item->setResume(itemMap.value("resume").toMap().value("position").toInt());
        list.append(item);
        m_idIndexMapping.insert(item->episodeId(), index++);
    }
    beginResetModel();
    m_list = list;
    endResetModel();
}

void Episodes::detailsReceived(const QVariantMap &rsp)
{
    qDebug() << "got item details:" << rsp;
    int id = rsp.value("id").toInt();
    int row = m_detailsRequestMap.take(id);
    LibraryItem *item = qobject_cast<LibraryItem*>(m_list.at(row));
    QVariantMap details = rsp.value("result").toMap().value("episodedetails").toMap();
    item->setPlot(details.value("plot").toString());
    item->setRating(details.value("rating").toInt());
    item->setSeason(details.value("season").toInt());
    item->setEpisode(details.value("episode").toInt());
    item->setFirstAired(details.value("firstaired").toString());
    emit dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

KodiModel *Episodes::enterItem(int index)
{
    Q_UNUSED(index)
    qDebug() << "Cannot enter Episode. Use playItem() to play it";
    return 0;
}

void Episodes::playItem(int index, bool resume)
{
    VideoPlaylistItem item;
    item.setEpisodeId(m_list.at(index)->data(RoleEpisodeId).toInt());
    Kodi::instance()->videoPlayer()->open(item, resume);
}

void Episodes::addToPlaylist(int row)
{
    VideoPlaylistItem pItem;
    pItem.setEpisodeId(index(row, 0, QModelIndex()).data(RoleEpisodeId).toInt());
    Kodi::instance()->videoPlayer()->playlist()->addItems(pItem);
}

QString Episodes::title() const
{
    return tr("Episodes");
}
