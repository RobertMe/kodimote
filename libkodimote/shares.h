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

#ifndef SHARES_H
#define SHARES_H

#include "kodilibrary.h"

class Shares : public KodiLibrary
{
    Q_OBJECT
public:
    explicit Shares(const QString &mediatype, KodiLibrary *parent = 0);

    KodiModel *enterItem(int index);
    void playItem(int index, bool resume = false);
    void addToPlaylist(int index);

    QString title() const;

    bool allowSearch();

    MediaFormat mediaFormat() const {
        if (m_mediaType == "music") {
            return MediaFormatAudio;
        } else if (m_mediaType == "video") {
            return MediaFormatVideo;
        } else if (m_mediaType == "pictures") {
            return MediaFormatPictures;
        }
        return MediaFormatUnknown;
    }

public slots:
    void refresh();

private slots:
    void sourcesReceived(const QVariantMap &rsp);

private:
    QString m_mediaType;
};

#endif // SHARES_H
