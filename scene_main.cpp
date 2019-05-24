#include "battle.h"
#include "scene_main.h"

SceneMain::SceneMain()
{
    //setSceneRect(0,0,1024,768);
}

void SceneMain::init()
{
    next_turn_button = new Button(QString("结束第1回合"));
    add(next_turn_button);
    connect(next_turn_button, SIGNAL(leftButtonClicked()), this, SLOT(next_turn()));

    debug_button = new Button(QString("进入Debug模式"));
    add(debug_button);
    connect(debug_button, SIGNAL(leftButtonClicked()), this, SLOT(enable_debug_mode()));

    changeStageButton = new Button(QString("切换关卡"));
    add(changeStageButton);
    connect(changeStageButton, SIGNAL(leftButtonClicked()), this, SLOT(change_stage()));

    saveButton = new Button(QString("Save"));
    add(saveButton);
    connect(saveButton, SIGNAL(leftButtonClicked()), this, SLOT(saveToFile()));

    loadButton = new Button(QString("Load"));
    add(loadButton);
    connect(loadButton, SIGNAL(leftButtonClicked()), this, SLOT(loadFromFile()));


    robotStatus = new RobotStatusView(200, 800);

    loadStage(1);


    music_background = new Music();
    music_background->setMusicLoop(config->background_music_path);
    music_battle = new Music();
    music_effect = new Music();

    connect(music_battle, SIGNAL(stateChanged(QMediaPlayer::State)), music_background, SLOT(state_change_slot(QMediaPlayer::State)));

    story = new Story();


}

void SceneMain::loadStage(int stage, bool useDefaultRobot)
{
    if (map)
    {
        delete map;
    }

    map = new Map();

    this->stage = stage;
    map->loadStage(stage);

    if (useDefaultRobot)
    {
        map->placeEnemy(stage,1);

        map->placeRobot(stage,1);
    }


    next_turn_button->setPos(map->width * 32 + 10, 650);
    debug_button->setPos(map->width * 32 + 10 + 200, 650);
    changeStageButton->setPos(map->width * 32 + 10, 690);
    saveButton->setPos(map->width * 32 + 10 + 100, 690);
    loadButton->setPos(map->width * 32 + 10 + 200, 690);

    robotStatus->updateXY(map->width * 32 + 10, 0);

    Menu_y = map->height * 32 + 10;
}

Menu * SceneMain::drawMenu(Menu::Layout layout, int x, int y)
{
    //draws a panel at x,y
    Menu * panel = new Menu(layout, x, y);


    this->add(panel);

    connect(panel, SIGNAL(rightButtonClicked()), this, SLOT(cancel()));

    return panel;
}

void SceneMain::displayMenu(Robot *robot)
{
    int x = robot->pos().x();
    int y = robot->pos().y();
    menu = drawMenu(Menu::Layout::horizontal, Menu_x, Menu_y);

    Button * button_ai = menu->addButton(QString("AI行动"));
    connect(button_ai, SIGNAL(leftButtonClicked()), this, SLOT(AI()));

    if (robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(attack1()));
    }
    if (robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(attack2()));
    }


    if (robot->pilot)
    {
        for (int i = 0; i < 19; ++i)
        {
            if (robot->pilot->property.spirit_table[i] && robot->pilot->spirit >= PeopleProperty::spirit_consume_table[i] && canUseSpirit(i))
            {
                Button * button = menu->addButton(robot->pilot->property.spirit_name[i] + " (" + QString::number(PeopleProperty::spirit_consume_table[i]) +")" );
                switch(i)
                {
                case 0: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_0())); break;
                case 1: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_1())); break;
                case 2: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_2())); break;
                case 3: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_3())); break;
                case 4: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_4())); break;
                case 5: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_5())); break;
                case 6: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_6())); break;
                case 7: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_7())); break;
                case 8: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_8())); break;
                case 9: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_9())); break;
                case 10: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_10())); break;
                case 11: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_11())); break;
                case 12: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_12())); break;
                case 13: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_13())); break;
                case 14: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_14())); break;
                case 15: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_15())); break;
                case 16: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_16())); break;
                case 17: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_17())); break;
                case 18: connect(button, SIGNAL(leftButtonClicked()), this, SLOT(use_sprit_18())); break;
                default:break;
                }

            }
        }
    }

    if (inDebugMode)
    {
        if (selectedRobot && selectedRobot->active == false)
        {
            Button * button_set_active = menu->addButton(QString("设置激活状态"));
            connect(button_set_active, SIGNAL(leftButtonClicked()), this, SLOT(setActive()));

        }
    }

    Button * button_ok = menu->addButton(QString("待命"));
    connect(button_ok, SIGNAL(leftButtonClicked()), this, SLOT(robotActionFinished()));
}

void SceneMain::displayMenu2(Robot *robot)
{
    int x = robot->pos().x();
    int y = robot->pos().y();
    menu = drawMenu(Menu::Layout::horizontal, Menu_x, Menu_y);

    if (robot->weapon1->range == 1 && robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(attack1()));
    }
    if (robot->weapon2->range == 1 && robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(attack2()));
    }


    Button * Button_move = menu->addButton(QString("待命"));
    connect(Button_move, SIGNAL(leftButtonClicked()), this, SLOT(robotActionFinished()));
}

void SceneMain::displayMenu3(Robot *robot, Robot *enemy)
{
    menu = drawMenu(Menu::Layout::vertical, enemy->pos().x() + 10, enemy->pos().y());

    if (robot->weapon1->range == 1 && robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(do_attack1()));
    }
    if (robot->weapon2->range == 1 && robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(do_attack2()));
    }
}

void SceneMain::deleteMenu()
{
    if (menu)
    {
        this->remove(menu);
        delete menu;
        menu = 0;
    }
}

void SceneMain::robotActionFinished()
{
    deleteMenu();

    map->UnshowMoveRange();
    inMoveStatus  = false;


    if (selectedRobot)
    {
        selectedRobot->setNotActive();

        selectedRobot = nullptr;
    }

    selectedWeapon = nullptr;

}

void SceneMain::cancel()
{
    deleteMenu();

    if (selectedRobot)
        map->move(selectedRobot, originalPosition.x, originalPosition.y);

    map->UnshowMoveRange();
    inMoveStatus  = false;
    selectedRobot = 0;
    selectedWeapon = 0;

}


void SceneMain::next_turn()
{
    ++round;

    map->placeEnemy(stage, round);

    music_background->setMusicLoop(config->background_music_path_enemy);

    for (int i = 1; i < map->width - 1; ++i)
    {
        for (int j = 1; j < map->height - 1; ++j)
        {
            if (map->robots[i][j])
            {
                map->robots[i][j]->setActive();

            }
        }
    }
    for (int i = 1; i < map->width - 1; ++i)
    {
        for (int j = 1; j < map->height - 1; ++j)
        {
            //qDebug() << i << j;
            if (map->robots[i][j] && map->robots[i][j]->player == 1)
            {
                Robot * robot = map->robots[i][j];
                if (robot->active)
                {
                    selectedRobot = map->robots[i][j];
                    AI();
                    //qDebug() << "AI()";
                }
            }
        }
    }

    for (int i = 1; i < map->width - 1; ++i)
    {
        for (int j = 1; j < map->height - 1; ++j)
        {
            if (map->robots[i][j])
            {
                map->robots[i][j]->setActive();
                map->robots[i][j]->clearSpirit();
            }
        }
    }


    QString s;
    s.sprintf("结束第%d回合", round);
    next_turn_button->setText(s);



    music_background->setMusicLoop(config->background_music_path);

    story->showConversition(round);
}

void SceneMain::enable_debug_mode()
{
    inDebugMode = !inDebugMode;

    QString s;
    if (inDebugMode)
    {
        s.sprintf("退出Debug模式", round);

    }
    else {
        s.sprintf("进入Debug模式", round);

    }
    debug_button->setText(s);
}

void SceneMain::change_stage()
{
    this->stage++;
    if (this->stage == 3)
        this->stage = 1;

    this->loadStage(this->stage);
}


void SceneMain::AI()
{
    deleteMenu();
    map->UnshowMoveRange();
    inMoveStatus  = false;

    Robot * enemy;
    if (enemy = selectedRobot->canAttack1())
    {
        selectedWeapon = selectedRobot->weapon1;
        attack(enemy);
        return;
    }
    if (enemy = selectedRobot->canAttack2())
    {
        selectedWeapon = selectedRobot->weapon2;
        attack(enemy);
        return;
    }

    if (round >= selectedRobot->robotBehavior)
    {
        map->AI_move(selectedRobot);
    }


    if (selectedRobot->weapon1 && selectedRobot->weapon1->range == 1 && (enemy = selectedRobot->canAttack1()))
    {
        selectedWeapon = selectedRobot->weapon1;
        attack(enemy);
        return;
    }
    else if (selectedRobot->weapon2 && selectedRobot->weapon2->range == 1 && (enemy = selectedRobot->canAttack2()))
    {
        selectedWeapon = selectedRobot->weapon2;
        attack(enemy);
        return;
    }

    selectedRobot->setNotActive();
    selectedRobot = 0;
}
void SceneMain::attack1()
{
    selectedWeapon = selectedRobot->weapon1;
    showAttackRange();
}

void SceneMain::attack2()
{
    selectedWeapon = selectedRobot->weapon2;
    showAttackRange();
}

void SceneMain::do_attack1()
{
    selectedWeapon = selectedRobot->weapon1;
    music_effect->setMusicOnce(config->button_press_music);
    attack(enemy);
}

void SceneMain::do_attack2()
{
    selectedWeapon = selectedRobot->weapon2;
    music_effect->setMusicOnce(config->button_press_music);
    attack(enemy);
}

void SceneMain::setActive()
{
    deleteMenu();

    map->UnshowMoveRange();

    selectedRobot->setActive();

    selectedRobot = 0;
}

void SceneMain::saveToFile()
{
    QFile data(config->saveFilePath);
    if (data.open(QFile::WriteOnly)) {
        QTextStream out(&data);
        out << stage << "\n";
        out << round << "\n";

        for (int i = 0; i < map->width; ++i)
        {
            for (int j = 0; j < map->height; ++j)
            {
                if (map->robots[i][j])
                {
                    Robot * robot = map->robots[i][j];
                    out << robot->x << ",";
                    out << robot->y << ",";
                    out << robot->id << ",";
                    out << robot->robotBehavior << ",";
                    out << robot->pilot->id << ",";
                    out << robot->player << ",";
                    out << robot->active << ",";
                    out << robot->level << ",";
                    out << robot->pilot->exp << ",";
                    out << robot->hp << ",";
                    out << robot->pilot->spirit << "\n";
                }
            }
        }

        data.close();
    }


    qDebug() << config->saveFilePath;
}

void SceneMain::loadFromFile()
{
    robotActionFinished();

    QString filename = config->saveFilePath;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << filename << "open failed";
        exit(-1);

    }

    QVector<RobotData> robots;

    QTextStream in(&file);

    QString line = in.readLine();
    stage = line.toInt();
    line = in.readLine();
    round = line.toInt();
    QString s;
    s.sprintf("结束第%d回合", round);
    next_turn_button->setText(s);

    loadStage(stage, false);

    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList t = line.split(",");

        if (t.size() == 0)
            continue;


        RunTimeRobotData robotData;
        int i = 0;

        robotData.x = QString(t[i++]).toInt();
        robotData.y = QString(t[i++]).toInt();
        robotData.robotId = QString(t[i++]).toInt();
        robotData.robotBehavior = QString(t[i++]).toInt();
        robotData.peopleId = QString(t[i++]).toInt();

        robotData.player = QString(t[i++]).toInt();
        robotData.active = QString(t[i++]).toInt();
        robotData.level = QString(t[i++]).toInt();
        robotData.exp = QString(t[i++]).toInt();
        robotData.hp = QString(t[i++]).toInt();

        robotData.sprit = QString(t[i++]).toInt();

        map->placeRobotRunTime(robotData);
    }

    file.close();
}

bool SceneMain::canUseSpirit(int id)
{
    switch(id)
    {
    case 0: return selectedRobot->hp < selectedRobot->hp_total;
    case 1: return true;
    case 2: return true;
    case 3: return true;
    case 4: return true;
    case 5: return true;
    case 6: return true;
    case 7: return true;
    case 8: return true;
    case 9: return true;
    case 10: return true;
    case 11: return true;
    case 12: return true;
    case 13: return true;
    case 14: return true;
    case 15: return true;
    case 16: return true;
    case 17: return true;
    case 18: return true;
    default:break;
    }
}




void SceneMain::attack(Robot *enemy2)
{
    enemy = enemy2;

    deleteMenu();

    map->UnshowMoveRange();

    map->showAttackGif(selectedRobot->x, selectedRobot->y, enemy->x, enemy->y);



    battle = new Battle(selectedRobot, selectedWeapon, enemy);

}
void SceneMain::showAttackRange()
{
    deleteMenu();

    map->UnshowMoveRange();

    map->showAttackRange(selectedRobot, selectedWeapon);
}
void SceneMain::showAttackRangeAfterMove()
{
    map->UnshowMoveRange();

    if (selectedRobot->weapon1->range == 1)
    {
        map->showAttackRange(selectedRobot, selectedRobot->weapon1);
    }
    else if (selectedRobot->weapon2->range == 1)
    {
        map->showAttackRange(selectedRobot, selectedRobot->weapon2);
    }
    else {
        map->showCannotAttackRange(selectedRobot);
    }
}
void SceneMain::attackDone()
{
    deleteMenu();

    if (enemy->hp <= 0)
    {
        map->removeRobot(enemy);
    }
    else if (selectedRobot->hp <= 0)
    {
        map->removeRobot(selectedRobot);
    }


    if (selectedRobot)
    {
        selectedRobot->setNotActive();
    }



    map->UnshowMoveRange();
    this->inMoveStatus = false;
    selectedRobot = 0;
    selectedWeapon = 0;

    if (battle)
    {
        delete battle;
        battle = 0;
    }

}

void SceneMain::showDiagDone()
{

}

void SceneMain::showConversition()
{


    story->showConversition(1);
}


void SceneMain::use_sprit_begin(int id)
{
    selectedRobot->pilot->spirit -= PeopleProperty::spirit_consume_table[id];

    selectedRobot->spirit[id] = true;

    deleteMenu();
}
void SceneMain::use_sprit_end(int id)
{
    robotStatus->showRobot(selectedRobot);
    displayMenu(selectedRobot);
}

void SceneMain::use_sprit_0()	//毅力
{
    use_sprit_begin(0);
    
    qDebug() << "use_sprit_0";
    selectedRobot->hp += 50;

    if (selectedRobot->hp > selectedRobot->hp_total)
    {
        selectedRobot->hp = selectedRobot->hp_total;
    }
    map->showText(selectedRobot->x, selectedRobot->y, QString("+50"));
    robotStatus->showRobot(selectedRobot);


    use_sprit_end(0);
}
void SceneMain::use_sprit_1()	//加速
{
    use_sprit_begin(1);
    
    qDebug() << "use_sprit_1";
    selectedRobot->spirit[1] = true;

    map->UnshowMoveRange();
    map->showMoveRange(selectedRobot);

    use_sprit_end(1);
}
void SceneMain::use_sprit_2()	//瞄准
{
    use_sprit_begin(2);
    
    qDebug() << "use_sprit_2";
    selectedRobot->spirit[2] = true;


    use_sprit_end(2);
}
void SceneMain::use_sprit_3()	//防守
{
    use_sprit_begin(3);
    
    qDebug() << "use_sprit_3";
    selectedRobot->spirit[3] = true;


    use_sprit_end(3);
}
void SceneMain::use_sprit_4()	//强攻
{
    use_sprit_begin(4);
    
    qDebug() << "use_sprit_4";
    selectedRobot->spirit[4] = true;


    use_sprit_end(4);
}
void SceneMain::use_sprit_5()	//友情
{
    use_sprit_begin(5);
    
    qDebug() << "use_sprit_5";


    use_sprit_end(5);
}
void SceneMain::use_sprit_6()	//必杀
{
    use_sprit_begin(6);
    
    qDebug() << "use_sprit_6";
    selectedRobot->spirit[6] = true;


    use_sprit_end(6);
}
void SceneMain::use_sprit_7()	//疾风
{
    use_sprit_begin(7);
    
    qDebug() << "use_sprit_7";
    selectedRobot->spirit[7] = true;

    map->UnshowMoveRange();
    map->showMoveRange(selectedRobot);

    use_sprit_end(7);
}
void SceneMain::use_sprit_8()	//回避
{
    use_sprit_begin(8);
    
    qDebug() << "use_sprit_8";
    selectedRobot->spirit[8] = true;


    use_sprit_end(8);
}
void SceneMain::use_sprit_9()	//潜力
{
    use_sprit_begin(9);
    
    qDebug() << "use_sprit_9";

    int hp_plus = selectedRobot->hp_total - selectedRobot->hp;
    selectedRobot->hp += hp_plus;

    if (selectedRobot->hp > selectedRobot->hp_total)
    {
        selectedRobot->hp = selectedRobot->hp_total;
    }
    map->showText(selectedRobot->x, selectedRobot->y, QString("+" + QString::number(hp_plus)));
    robotStatus->showRobot(selectedRobot);

    use_sprit_end(9);
}
void SceneMain::use_sprit_10()	//热血
{
    use_sprit_begin(10);
    
    qDebug() << "use_sprit_10";
    selectedRobot->spirit[10] = true;


    use_sprit_end(10);
}
void SceneMain::use_sprit_11()	//情义
{
    use_sprit_begin(11);
    
    qDebug() << "use_sprit_11";


    use_sprit_end(11);
}
void SceneMain::use_sprit_12()	//传真
{
    use_sprit_begin(12);
    
    qDebug() << "use_sprit_12";


    use_sprit_end(12);
}
void SceneMain::use_sprit_13()	//援助
{
    use_sprit_begin(13);
    
    qDebug() << "use_sprit_13";


    use_sprit_end(13);
}
void SceneMain::use_sprit_14()	//怒
{
    use_sprit_begin(14);
    
    qDebug() << "use_sprit_14";


    use_sprit_end(14);
}
void SceneMain::use_sprit_15()	//祈祷
{
    use_sprit_begin(15);
    
    qDebug() << "use_sprit_15";


    use_sprit_end(15);
}
void SceneMain::use_sprit_16()	//干扰
{
    use_sprit_begin(16);
    
    qDebug() << "use_sprit_16";


    use_sprit_end(16);
}
void SceneMain::use_sprit_17()	//狂怒
{
    use_sprit_begin(17);
    
    qDebug() << "use_sprit_17";


    use_sprit_end(17);
}
void SceneMain::use_sprit_18()	//爱心
{
    use_sprit_begin(18);
    
    qDebug() << "use_sprit_18";
}
