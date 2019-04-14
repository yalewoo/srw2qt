#ifndef MUSIC_H
#define MUSIC_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

class Music : public QMediaPlayer
{
    Q_OBJECT
public:
    Music();

    QMediaPlaylist *playlist = 0;

    void setMusicOnce(QString filename);

    void setMusicLoop(QString filename);

    void start();



public slots:
    void state_change_slot(QMediaPlayer::State state);
};

#endif // MUSIC_H
