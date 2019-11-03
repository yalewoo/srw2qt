#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QString>



struct Config
{
    QString workDir;
    QString robot_value_path;
    QString weapon_value_path;
    QString people_value_path;
    QString map_path;
    QString maprect_path;
    QString robot_icon_path;
    QString robot_icon_path_player;
    QString robot_icon_path_enemy;
    QString robot_image_path;
    QString people_image_path;

    QString start_map_image_path;

    QString exp_table_path;

    QString robot_init_path;
    QString enemy_init_path;

    QString background_music_path_root;
    QString background_music_path;
    QString background_music_path_enemy;
    QString background_music_start;
    QString background_music_title;

    QString button_press_music;

    QString attack_arrow;
    QString selected_arrow;
    QString attack_fire_music;
    QString attack_fire_img;

    QString conversion_path;

    QString saveFilePath;
    QString saveFilePath_exp;

    QString html_path;

    int rectWidth = 32;

    Config() {
        workDir  = QString("D:/code/Github/srw2qt/");
        QDir dir(workDir);
        if(!dir.exists())
        {
          workDir = "./";
        }

        saveFilePath = workDir + "tmpfiles/save.txt";
        saveFilePath_exp = workDir + "tmpfiles/save_exp.txt";

        robot_value_path = workDir + "input/value/robot.csv";
        weapon_value_path = workDir + "input/value/weapon.csv";
        people_value_path = workDir + "input/value/people.csv";
        exp_table_path = workDir + "input/value/exp_table.txt";

        maprect_path = workDir + "res/images/map32/";
        robot_icon_path = workDir + "res/images/robot32/";
        robot_icon_path_player = workDir + "resource/images/robot_icon/player/";
        robot_icon_path_enemy = workDir + "resource/images/robot_icon/enemy/";
        people_image_path = workDir + "res/images/people/";
        robot_image_path = workDir + "res/images/robotImg/";

        start_map_image_path = workDir + "res/images/background/start_map.png";

        map_path = workDir + "input/map/map1.csv";

        robot_init_path = workDir + "input/stage/robot_init.csv";
        enemy_init_path = workDir + "input/stage/enemy_init.csv";

        background_music_path_root = workDir + "/res/music/";
        background_music_path = workDir + "/res/music/87.wav";
        background_music_path_enemy = workDir + "/res/music/88.wav";
        background_music_start = workDir + "/res/music/85.wav";
        background_music_title = workDir + "/res/music/92.wav";


        button_press_music = workDir + "res/wav/pushbutton.mp3";


        attack_arrow = workDir + "res/images/arrow/arrow.png";
        selected_arrow = workDir + "res/images/arrow/select.png";

        attack_fire_music = workDir + "res/wav/huoqiu.mp3";
        attack_fire_img = workDir + "/res/images/weapon/fire.png";

        conversion_path = workDir + "input/stage/conversitions.csv";

        html_path = workDir + "html/robotStatus.html";
    }

    QString getPlaybookPath(int stage, int turn)
    {
        if (turn == -1)
        {
            return workDir + "input/stage/" + QString::number(stage) + "/end.csv";
        }
        return workDir + "input/stage/" + QString::number(stage) + "/" + QString::number(turn) + ".csv";
    }

    QString getMapPathOfLevel(int level)
    {
        return workDir + "input/map/map" + QString::number(level) + ".csv";
    }

};

#endif // CONFIG_H
