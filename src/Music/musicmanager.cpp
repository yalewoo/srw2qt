#include "musicmanager.h"

#include "datareader/config.h"
extern Config * config;

MusicManager::MusicManager()
{
    music_background = new Music();
    music_effect = new Music();
    music_battle = new Music();
}

void MusicManager::PlayButtonClicked()
{
    music_effect->setMusicOnce(config->button_press_music);

}

void MusicManager::PlayStartMusic()
{
    music_background->setMusicLoop(config->background_music_start);
}
