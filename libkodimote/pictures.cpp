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

#include "pictures.h"
#include "kodi.h"
#include "kodiconnection.h"

//Pictures::Pictures():
//    KodiModel(parent)
//{
//    QVariantMap params;
//    if(artistid != -1) {
//      params.insert("artistid", artistid);
//    }
//    if(albumid != -1) {
//      params.insert("albumid", albumid);
//    }
//    QVariantList fields;
//    fields.append("artist");
//    fields.append("album");
//    params.insert("fields", fields);

//    QVariantMap sort;
//    if(m_albumId == -1) {
//        sort.insert("method", "label");
//    } else {
//        sort.insert("method", "track");
//    }
//    sort.insert("order", "ascending");
//    params.insert("sort", sort);
//    m_requestId = KodiConnection::sendCommand("AudioLibrary.GetSongs", params);
//    connect(KodiConnection::notifier(), SIGNAL(responseReceived(int,QVariantMap)), SLOT(responseReceived(int,QVariantMap)));
//}
