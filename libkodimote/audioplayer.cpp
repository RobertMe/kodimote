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

#include "audioplayer.h"

#include "audioplaylist.h"

AudioPlayer::AudioPlayer(QObject *parent):
    Player(PlayerTypeAudio, parent)
{
    m_playList = new AudioPlaylist(this);
}

int AudioPlayer::playerId() const
{
    return 0;
}

Playlist * AudioPlayer::playlist() const
{
    return m_playList;
}

void AudioPlayer::open(const PlaylistItem &item, bool resume)
{
    open(item, 0, resume);
}

void AudioPlayer::open(const PlaylistItem &item, int position, bool resume)
{
    if (resume) {
        qDebug() << "resume is not supported for AudioPlayer";
    }

    m_playList->clear();
    m_playList->addItems(item);
    playItem(position);
}
