#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "music.h"



class MusicManager : public QObject
{
    Q_OBJECT

    //music
    Music * music_background = nullptr;
    Music * music_effect = nullptr;
    Music * music_battle = nullptr;

public:
    MusicManager();

    void PlayButtonClicked();

    void PlayStartMusic();

    void PlayBackgroundMusic(int player);

    void PlayBattleMusic(QString path);
    void PlayOnceMusic(QString path);
    void StopBattle();
};

#endif // MUSICMANAGER_H
