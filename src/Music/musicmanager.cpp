#include "musicmanager.h"

#include "datareader/config.h"
extern Config * config;

MusicManager::MusicManager()
{
    music_background = new Music();
    music_effect = new Music();
    music_battle = new Music();

    connect(music_battle, SIGNAL(stateChanged(QMediaPlayer::State)), music_background, SLOT(state_change_slot(QMediaPlayer::State)));

}

void MusicManager::PlayButtonClicked()
{
    music_effect->setMusicOnce(config->button_press_music);

}

void MusicManager::PlayStartMusic()
{
    music_background->setMusicLoop(config->background_music_start);
}

void MusicManager::PlayTitleMusic()
{
    music_background->setMusicLoop(config->background_music_title);
}

void MusicManager::PlayBackgroundMusic(int player)
{
    if (player == 0)
    {
        this->StopBattle();
        music_background->setMusicLoop(config->background_music_path);
    }
    else
    {
        music_background->setMusicLoop(config->background_music_path_enemy);
    }
}

void MusicManager::PlayBattleMusic(QString path)
{
    music_battle->setMusicLoop(config->background_music_path_root + path);
}

void MusicManager::PlayOnceMusic(QString path)
{
    music_battle->setMusicOnce(config->background_music_path_root + path);
}

void MusicManager::PlayOnceMusicNonStopBackground(QString path)
{
    music_effect->setMusicOnce(config->background_music_path_root + path);
}

void MusicManager::StopBattle()
{
    music_battle->stop();
}

void MusicManager::StopAll()
{
    music_background->stop();
    music_effect->stop();
    music_battle->stop();
}
