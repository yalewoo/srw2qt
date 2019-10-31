#include "music.h"



Music::Music()
{

}

void Music::setMusicOnce(QString filename)
{
    setMedia(QUrl::fromLocalFile(filename));
    play();
}

void Music::setMusicLoop(QString filename)
{

    if (playlist)
        delete playlist;

    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl::fromLocalFile(filename));


    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    setPlaylist(playlist);
    play();

}

void Music::start()
{
    play();
}



void Music::state_change_slot(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState)
    {
        this->pause();
    }
    else if (state == QMediaPlayer::StoppedState)
    {
        this->start();
    }
}
