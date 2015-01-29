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

#ifndef RECORDINGS_H
#define RECORDINGS_H

#include "kodilibrary.h"

class Recordings: public KodiLibrary
{
    Q_OBJECT
public:
    explicit Recordings(const QString &path, bool allSubItems = false, KodiModel *parent = 0);
    
    QString title() const;
    void refresh();
    KodiModel* enterItem(int index);
    void playItem(int index);
    void addToPlaylist(int index);

    virtual QHash<int, QByteArray> roleNames() const;

    ThumbnailFormat thumbnailFormat() const { return ThumbnailFormatPortrait; }
    Q_INVOKABLE void fetchItemDetails(int index);
    Q_INVOKABLE bool hasDetails() { return true; }

signals:
    
private slots:
    void listReceived(const QVariantMap &rsp);

private:

    static bool recordingLessThan(KodiModelItem *item1, KodiModelItem *item2);

    QString m_path;
    bool m_allSubItems;
};

#endif // RECORDINGS_H
