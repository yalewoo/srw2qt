#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "music.h"



class MusicManager
{
    //music
    Music * music_background = nullptr;
    Music * music_effect = nullptr;
    Music * music_battle = nullptr;

public:
    MusicManager();

    void PlayButtonClicked();

    void PlayStartMusic();
};

#endif // MUSICMANAGER_H
