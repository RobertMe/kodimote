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

#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include "kodimodel.h"

#include <QTime>
#include <QObject>
#include <QStandardItem>

class LibraryItem : public KodiModelItem
{
    Q_OBJECT

    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString filename READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
    Q_PROPERTY(QString filetype READ fileType WRITE setFileType NOTIFY fileTypeChanged)
    Q_PROPERTY(bool playable READ playable WRITE setPlayable NOTIFY playableChanged)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(int songId READ songId WRITE setSongId NOTIFY songIdChanged)
    Q_PROPERTY(int genreId READ genreId WRITE setGenreId NOTIFY genreIdChanged)
    Q_PROPERTY(int musicvideoId READ musicvideoId WRITE setMusicvideoId NOTIFY musicvideoIdChanged)
    Q_PROPERTY(int tvshowId READ tvshowId WRITE setTvshowId NOTIFY tvshowIdChanged)
    Q_PROPERTY(QString tvShow READ tvShow WRITE setTvShow NOTIFY tvShowChanged)
    Q_PROPERTY(int seasonId READ seasonId WRITE setSeasonId NOTIFY seasonIdChanged)
    Q_PROPERTY(int episodeId READ episodeId WRITE setEpisodeId NOTIFY episodeIdChanged)
    Q_PROPERTY(int movieId READ movieId WRITE setMovieId NOTIFY movieIdChanged)
    Q_PROPERTY(int channelgroupId READ channelgroupId WRITE setChannelgroupId NOTIFY channelgroupIdChanged)
    Q_PROPERTY(int channelId READ channelId WRITE setChannelId NOTIFY channelIdChanged)
    Q_PROPERTY(int recordingId READ recordingId WRITE setRecordingId NOTIFY recordingIdChanged)
    Q_PROPERTY(QString plot READ plot WRITE setPlot NOTIFY plotChanged)
    Q_PROPERTY(int rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int season READ season WRITE setSeason NOTIFY seasonChanged)
    Q_PROPERTY(int episode READ episode WRITE setEpisode NOTIFY episodeChanged)
    Q_PROPERTY(QString firstAired READ firstAired WRITE setFirstAired NOTIFY firstAiredChanged)
    Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged)
    Q_PROPERTY(QString year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(QString director READ director WRITE setDirector NOTIFY directorChanged)
    Q_PROPERTY(QString tagline READ tagline WRITE setTagline NOTIFY taglineChanged)
    Q_PROPERTY(QString mpaa READ mpaa WRITE setMpaa NOTIFY mpaaChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString instrument READ instrument WRITE setInstrument NOTIFY instrumentChanged)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(QString mood READ mood WRITE setMood NOTIFY moodChanged)
    Q_PROPERTY(QString born READ born WRITE setBorn NOTIFY bornChanged)
    Q_PROPERTY(QString formed READ formed WRITE setFormed NOTIFY formedChanged)
    Q_PROPERTY(QString died READ died WRITE setDied NOTIFY diedChanged)
    Q_PROPERTY(QString disbanded READ disbanded WRITE setDisbanded NOTIFY disbandedChanged)
    Q_PROPERTY(QTime duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString durationString READ durationString NOTIFY durationChanged)
    Q_PROPERTY(int durationInSecs READ durationInSecs NOTIFY durationChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)
    Q_PROPERTY(int playcount READ playcount WRITE setPlaycount NOTIFY playcountChanged)
    Q_PROPERTY(QString cast READ cast WRITE setCast NOTIFY castChanged)
    Q_PROPERTY(int resume READ resume WRITE setResume NOTIFY resumeChanged)
    Q_PROPERTY(QString resumeString READ resumeString NOTIFY resumeChanged)

public:
    explicit LibraryItem(const QString &title, const QString &subTitle = QString(), QObject *parent = 0);
    explicit LibraryItem(QObject *parent = 0);

    QString type() const;
    void setType(const QString &type);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString thumbnail() const;
    void setThumbnail(const QString &thumbnail);

    QString fanart() const;
    void setFanart(const QString &fanart);

    QString fileType() const;
    void setFileType(const QString &fileType);

    bool playable() const;
    void setPlayable(bool playable);

    int artistId() const;
    void setArtistId(int artistId);

    QString artist() const;
    void setArtist(const QString &artist);

    int albumId() const;
    void setAlbumId(int albumId);

    QString album() const;
    void setAlbum(const QString &album);

    int songId() const;
    void setSongId(int songId);

    int genreId() const;
    void setGenreId(int genreId);

    int musicvideoId() const;
    void setMusicvideoId(int musicvideoId);

    int tvshowId() const;
    void setTvshowId(int tvshowId);

    QString tvShow() const;
    void setTvShow(QString tvShow);

    int seasonId() const;
    void setSeasonId(int seasonId);

    int episodeId() const;
    void setEpisodeId(int episodeId);

    int movieId() const;
    void setMovieId(int movieId);

    int channelgroupId() const;
    void setChannelgroupId(int channelgroupId);

    int channelId() const;
    void setChannelId(int channelId);

    int recordingId() const;
    void setRecordingId(int recordingId);

    QString plot() const;
    void setPlot(const QString &plot);

    int rating() const;
    void setRating(int rating);

    int season() const;
    void setSeason(int season);

    int episode() const;
    void setEpisode(int episode);

    QString firstAired() const;
    void setFirstAired(const QString &firstAired);

    QString genre() const;
    void setGenre(const QString &genre);

    QString year() const;
    void setYear(const QString &year);

    QString director() const;
    void setDirector(const QString &director);

    QString tagline() const;
    void setTagline(const QString &tagline);

    QString mpaa() const;
    void setMpaa(const QString &mpaa);

    QString description() const;
    void setDescription(const QString &description);

    QString instrument() const;
    void setInstrument(const QString &instrument);

    QString style() const;
    void setStyle(const QString &style);

    QString mood() const;
    void setMood(const QString &mood);

    QString born() const;
    void setBorn(const QString &born);

    QString formed() const;
    void setFormed(const QString &formed);

    QString died() const;
    void setDied(const QString &died);

    QString disbanded() const;
    void setDisbanded(const QString &disbanded);

    QTime duration() const;
    QString durationString() const;
    int durationInSecs() const;
    void setDuration(const QTime &duration);

    QString comment() const;
    void setComment(const QString &comment);

    int playcount() const;
    void setPlaycount(int playcount);

    QString cast() const;
    void setCast(const QString &cast);

    int resume() const;
    QString resumeString() const;
    void setResume(int resume);

    virtual QVariant data(int role) const;

signals:
    void typeChanged();
    void fileNameChanged();
    void thumbnailChanged();
    void fanartChanged();
    void fileTypeChanged();
    void playableChanged();
    void artistIdChanged();
    void artistChanged();
    void albumIdChanged();
    void albumChanged();
    void songIdChanged();
    void genreIdChanged();
    void musicvideoIdChanged();
    void tvshowIdChanged();
    void tvShowChanged();
    void seasonIdChanged();
    void episodeIdChanged();
    void movieIdChanged();
    void channelgroupIdChanged();
    void channelIdChanged();
    void recordingIdChanged();
    void plotChanged();
    void ratingChanged();
    void seasonChanged();
    void episodeChanged();
    void firstAiredChanged();
    void genreChanged();
    void yearChanged();
    void directorChanged();
    void taglineChanged();
    void mpaaChanged();
    void descriptionChanged();
    void instrumentChanged();
    void styleChanged();
    void moodChanged();
    void bornChanged();
    void formedChanged();
    void diedChanged();
    void disbandedChanged();
    void durationChanged();
    void commentChanged();
    void playcountChanged();
    void castChanged();
    void resumeChanged();

private slots:
    Q_INVOKABLE void imageFetched(int id);

private:
    void init();

    QString m_type;
    QString m_fileName;
    QString m_thumbnail;
    QString m_cachedThumbnail;
    QString m_fanart;
    QString m_fileType;
    bool m_playable;
    int m_artistId;
    QString m_artist;
    int m_albumId;
    QString m_album;
    int m_songId;
    int m_genreId;
    int m_musicvideoId;
    int m_tvshowId;
    QString m_tvShow;
    int m_seasonId;
    int m_episodeId;
    int m_movieId;
    int m_channelgroupId;
    int m_channelId;
    int m_recordingId;
    QString m_plot;
    int m_rating;
    int m_season;
    int m_episode;
    QString m_firstAired;
    QString m_genre;
    QString m_year;
    QString m_director;
    QString m_tagline;
    QString m_mpaa;
    QString m_description;
    QString m_instrument;
    QString m_style;
    QString m_mood;
    QString m_born;
    QString m_formed;
    QString m_died;
    QString m_disbanded;
    QTime m_duration;
    QString m_comment;
    int m_playcount;
    QString m_cast;
    int m_resume;

    enum ImageType {
        ImageTypeThumbnail,
        ImageTypeFanart
    };

    // needed for caching in const getters
    mutable QMap<int, ImageType> m_imageFetchJobs;
};

#endif // LIBRARYITEM_H
