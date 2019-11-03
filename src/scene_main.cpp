#include "battle/battle.h"
#include "battle/battleground.h"
#include "scene_main.h"


#include "game.h"

extern Game * game;

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


    game->musicManager->PlayBackgroundMusic(0); // 我方音乐

    exp_table = QVector<int>(200, 0);
}

PositionAndSize SceneMain::getConversationPosition()
{
    return PositionAndSize(0,0,map->width*32, map->height*32);
}

void SceneMain::loadStage(int stage, bool useDefaultRobot)
{
    if (map)
    {
        delete map;
    }

    map = new Map();

    if (this->stage != stage)
    {
        delete story;
        story = new Story(stage, this);
    }

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
    connect(button_ai, SIGNAL(leftButtonClicked()), this, SLOT(AI_robot()));

    if (robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->m_weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(attack1()));
    }
    if (robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->m_weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(attack2()));
    }




    if (robot->m_pilot)
    {
        for (int i = 0; i < 19; ++i)
        {
            if (robot->m_pilot->property.spirit_table[i] && robot->m_pilot->spirit >= PeopleProperty::spirit_consume_table[i] && canUseSpirit(i))
            {
                Button * button = menu->addButton(robot->m_pilot->property.spirit_name[i] + " (" + QString::number(PeopleProperty::spirit_consume_table[i]) +")" );
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
    QVector<int> transforms = robot->canTransform();
    if (transforms.length() > 0)
    {
        for (int i = 0; i < transforms.length(); ++i)
        {
            int id = transforms[i];
            RobotProperty p = DataHelper::getRobotProperty(id);
            Button * button = menu->addButton(QString("变形为") + p.robotName);
            button->id = id;
            connect(button, SIGNAL(leftButtonClickedWithParameter(int)), this, SLOT(transform(int)));
        }
    }

    // 起飞
    if (selectedRobot->m_pilot->id == 54)
    {
        for (int i = 0; i < selectedRobot->m_passengers.length(); ++i)
        {
            Robot * robot = selectedRobot->m_passengers[i];
            if (robot->m_active || inDebugMode)
            {

                Button * button = menu->addButton(robot->m_property.robotName);
                button->id = i;
                connect(button, SIGNAL(leftButtonClickedWithParameter(int)), this, SLOT(launch(int)));
            }
        }
    }

    if (inDebugMode)
    {
        if (selectedRobot && selectedRobot->m_active == false)
        {
            Button * button_set_active = menu->addButton(QString("设置激活状态"));
            connect(button_set_active, SIGNAL(leftButtonClicked()), this, SLOT(setActive()));

        }
        if (selectedRobot)
        {
            Button * button = menu->addButton(QString("经验+50"));
            connect(button, SIGNAL(leftButtonClicked()), this, SLOT(addExp()));

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

    if (robot->m_weapon1->range == 1 && robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->m_weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(attack1()));
    }
    if (robot->m_weapon2->range == 1 && robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->m_weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(attack2()));
    }


    Button * Button_move = menu->addButton(QString("待命"));
    connect(Button_move, SIGNAL(leftButtonClicked()), this, SLOT(robotActionFinished()));
}

void SceneMain::displayMenu3(Robot *robot, Robot *enemy)
{
    menu = drawMenu(Menu::Layout::vertical, enemy->pos().x() + 10, enemy->pos().y());

    if (robot->m_weapon1->range == 1 && robot->canAttack1())
    {
        Button * button_attack1 = menu->addButton(robot->m_weapon1->name);
        connect(button_attack1, SIGNAL(leftButtonClicked()), this, SLOT(do_attack1()));
    }
    if (robot->m_weapon2->range == 1 && robot->canAttack2())
    {
        Button * button_attack2 = menu->addButton(robot->m_weapon2->name);
        connect(button_attack2, SIGNAL(leftButtonClicked()), this, SLOT(do_attack2()));
    }
}

void SceneMain::displayMenu4()
{
    menu = drawMenu(Menu::Layout::horizontal, Menu_x, Menu_y);

    if (selectedRobot->m_pilot->id == 54)
    {
        for (int i = 0; i < selectedRobot->m_passengers.length(); ++i)
        {
            Robot * robot = selectedRobot->m_passengers[i];
            if (robot->m_active)
            {

                Button * button = menu->addButton(robot->m_property.robotName);
                button->id = i;
                connect(button, SIGNAL(leftButtonClickedWithParameter(int)), this, SLOT(launch(int)));
            }
        }
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

        if (captain)
        {
            captain = 0;
            selectedRobot->m_inMainShip = false;
        }

        selectedRobot = nullptr;
    }


    selectedWeapon = nullptr;

}

void SceneMain::cancel()
{
    deleteMenu();

    if (selectedRobot){
        map->move(selectedRobot, originalPosition.x, originalPosition.y);
        if (captain)
        {
            captain->AddPassenger(selectedRobot);
            remove(selectedRobot);
            captain = nullptr;
        }
    }

    map->UnshowMoveRange();
    inMoveStatus  = false;
    selectedRobot = nullptr;
    selectedWeapon = nullptr;


}


void SceneMain::next_turn()
{
    // 检查是否胜利
    if (!map->hasAnyRobot(1))
    {
        story->execute(-1);

        robotActionFinished();

        map->UpdateExpTable(exp_table);

        this->stage++;
        if (this->stage == 27)
            this->stage = 1;

        this->loadStage(this->stage);
        map->UpdateRobotLevelUsingExpTable(exp_table);

        round = 1;
        story->setStage(stage);
        story->execute(round);
        return;
    }

    game->musicManager->PlayBackgroundMusic(1); // 敌方音乐

    ++round;

    // 执行本回合脚本 对话或增员
    story->execute(round);


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


    AI();

    // 我方机体初始状态
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

    // 母舰机体加血 加血战机体
    map->UpdateRobotsAtSupply();
    Robot * c = map->getCaptain();

    if (c && c->m_passengers.length() > 0)
    {
        for (int i = 0; i < c->m_passengers.length(); ++i)
        {
            Robot * robot = c->m_passengers[i];
            if (robot)
            {
                int hp = robot->m_hp_total * 0.3;
                robot->m_hp = robot->m_hp + hp > robot->m_hp_total ? robot->m_hp_total : robot->m_hp + hp;

                robot->setActive();
                robot->clearSpirit();
            }
        }

    }

    game->musicManager->PlayBackgroundMusic(0); // 我方音乐

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
//    if (this->stage == 3)
//        this->stage = 1;

    this->loadStage(this->stage);
}
void SceneMain::AI()
{
    auto enemys = AI::GetRobotList(map);
    for (int i = 0; i < enemys.length(); ++i)
    {
        selectedRobot = enemys[i];
        AI_robot();
    }
}

void SceneMain::AICore()
{

}
void SceneMain::AI_robot()
{
    deleteMenu();
    map->UnshowMoveRange();
    inMoveStatus  = false;

    qDebug() << "AI of robot in " << selectedRobot->m_x << selectedRobot->m_y;

    if (round < selectedRobot->m_robotBehavior)
    {
        Robot * enemy;
        if (enemy = selectedRobot->canAttack1())
        {
            selectedWeapon = selectedRobot->m_weapon1;
            attack(enemy);
            return;
        }
        else if (enemy = selectedRobot->canAttack2())
        {
            selectedWeapon = selectedRobot->m_weapon2;
            attack(enemy);
            return;
        }
        else {
            qDebug() << "本回合不行动, round = " << round << "";
            selectedRobot->setNotActive();
            selectedRobot = 0;
            selectedWeapon = 0;
        }
        return;
    }

    // get All robots that can attack
    QVector<Robot *> enemys = AI::GetAllAttackTargetRobots(map, selectedRobot);

    // Select one target
    // 1. 可以一击击落的机体
    // 2. 造成伤害最高的机体
    Robot * target = 0;
    int maxDamage = 0;
    for (int i = 0; i < enemys.length(); ++i)
    {
        Robot * enemy = enemys[i];
        qDebug() << "can attack" << enemy->m_x << enemy->m_y;

        int damage1 = 0;
        int damage2 = 0;
        if (enemy->m_tmp_ai_weapon1)
        {
            damage1 = BattleGround::getDamage(selectedRobot, enemy, selectedRobot->m_weapon1);
        }
        if (enemy->m_tmp_ai_weapon2)
        {
            damage2 = BattleGround::getDamage(selectedRobot, enemy, selectedRobot->m_weapon2);
        }

        if (damage1 > enemy->m_hp){
            target = enemy;
            selectedWeapon = selectedRobot->m_weapon1;
            break;
        }
        if (damage2 > enemy->m_hp){
            target = enemy;
            selectedWeapon = selectedRobot->m_weapon2;
            break;
        }

        int damage = qMax(damage1, damage2);
        if (damage > maxDamage)
        {
            maxDamage = damage;
            selectedWeapon = damage1 > damage2 ? selectedRobot->m_weapon1 : selectedRobot->m_weapon2;
            target = enemy;
        }
    }


    if (target)
    {
        // 攻击目标
        // 无须移动的情况
        if (map->canAttack(selectedRobot, selectedWeapon, target))
        {
            attack(target);
        }
        else {
            // 先移动在攻击
            QVector<QVector<int> > m = map->calculateMoveRange(selectedRobot);
            int x = target->m_x;
            int y = target->m_y;
            int target_x = x;
            int target_y = y;
            bool canAttack = true;
            if      (m[x][y-1]>=0 && !map->robots[x][y-1]) target_y = y-1;
            else if (m[x][y+1]>=0 && !map->robots[x][y+1]) target_y = y+1;
            else if (m[x-1][y]>=0 && !map->robots[x-1][y]) target_x = x-1;
            else if (m[x+1][y]>=0 && !map->robots[x+1][y]) target_x = x+1;
            else {
                //不能攻击了
                canAttack = false;
            }

            deleteMenu();

            if (canAttack)
            {
                map->move(selectedRobot, target_x, target_y);

                attack(target);
            }
            else {
                map->AI_move(selectedRobot);

                selectedRobot->setNotActive();
                selectedRobot = 0;
                selectedWeapon = 0;
            }

        }
    }
    else {
        // 移动后无法攻击
        map->AI_move(selectedRobot);

        selectedRobot->setNotActive();
        selectedRobot = 0;
        selectedWeapon = 0;
    }


}
void SceneMain::attack1()
{
    selectedWeapon = selectedRobot->m_weapon1;
    showAttackRange();
}

void SceneMain::attack2()
{
    selectedWeapon = selectedRobot->m_weapon2;
    showAttackRange();
}

void SceneMain::do_attack1()
{
    selectedWeapon = selectedRobot->m_weapon1;
    game->musicManager->PlayButtonClicked();
    attack(enemy);
}

void SceneMain::do_attack2()
{
    selectedWeapon = selectedRobot->m_weapon2;
    game->musicManager->PlayButtonClicked();
    attack(enemy);
}

void SceneMain::setActive()
{
    deleteMenu();

    map->UnshowMoveRange();

    selectedRobot->setActive();

    selectedRobot = 0;
}

void SceneMain::addExp()
{
    selectedRobot->gotExp(50);
    robotStatus->showRobot(selectedRobot);
}

void SceneMain::transform(int id)
{
    qDebug() << "变形为" << id;

    selectedRobot->ChangeId(id);

    robotStatus->showRobot(selectedRobot);

    deleteMenu();
    displayMenu(selectedRobot);

    map->UnshowMoveRange();
    map->showMoveRange(selectedRobot);
}

void SceneMain::launch(int id)
{
    qDebug() << "launch id=" << id;
    captain = selectedRobot;
    selectedRobot = selectedRobot->m_passengers[id];
    selectedRobot->setxy(originalPosition.x, originalPosition.y);

    captain->DeletePassenger(selectedRobot);

    inMoveStatus = true;


    add(selectedRobot);

    deleteMenu();
    map->UnshowMoveRange();

    map->showMoveRange(selectedRobot);
    displayMenu(selectedRobot);
}

void SceneMain::saveToFile()
{
    QFile data(config->saveFilePath);
    if (data.open(QFile::WriteOnly)) {
        QTextStream out(&data);
        out << stage << "\n";
        out << round << "\n";

        // 最先保存舰长
        for (int i = 0; i < map->width; ++i)
        {
            for (int j = 0; j < map->height; ++j)
            {
                if (map->robots[i][j] && map->robots[i][j]->m_pilot->id == 54)
                {
                    Robot * robot = map->robots[i][j];
                    out << robot->m_x << ",";
                    out << robot->m_y << ",";
                    out << robot->m_id << ",";
                    out << robot->m_robotBehavior << ",";
                    out << robot->m_pilot->id << ",";
                    out << robot->m_player << ",";
                    out << robot->m_active << ",";
                    out << robot->m_level << ",";
                    out << robot->m_pilot->exp << ",";
                    out << robot->m_hp << ",";
                    out << robot->m_inMainShip << ",";
                    out << robot->m_pilot->spirit << "\n";
                }
            }
        }

        //在保存其他
        for (int i = 0; i < map->width; ++i)
        {
            for (int j = 0; j < map->height; ++j)
            {
                if (map->robots[i][j] && map->robots[i][j]->m_pilot->id != 54)
                {
                    Robot * robot = map->robots[i][j];
                    out << robot->m_x << ",";
                    out << robot->m_y << ",";
                    out << robot->m_id << ",";
                    out << robot->m_robotBehavior << ",";
                    out << robot->m_pilot->id << ",";
                    out << robot->m_player << ",";
                    out << robot->m_active << ",";
                    out << robot->m_level << ",";
                    out << robot->m_pilot->exp << ",";
                    out << robot->m_hp << ",";
                    out << robot->m_inMainShip << ",";
                    out << robot->m_pilot->spirit << "\n";
                }
            }
        }



        data.close();
    }

    saveToFile_expTable();



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

        robotData.inMainShip = QString(t[i++]).toInt();

        robotData.sprit = QString(t[i++]).toInt();

        map->placeRobotRunTime(robotData);
    }

    LoadFromFile_expTable();

    file.close();
}

void SceneMain::saveToFile_expTable()
{
    // 保存每个人的经验值
    QFile data2(config->saveFilePath_exp);
    if (data2.open(QFile::WriteOnly)) {
        QTextStream out(&data2);

        map->UpdateExpTable(exp_table);
        for (int i = 0; i < exp_table.length(); ++i)
        {
            out << exp_table[i] << "\n";
        }


        data2.close();
    }
}
void SceneMain::LoadFromFile_expTable()
{
    QString filename = config->saveFilePath_exp;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << filename << "open failed";
        return;

    }

    exp_table.clear();

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        int e = line.toInt();
        exp_table.push_back(e);
    }

    file.close();
}

bool SceneMain::canUseSpirit(int id)
{
    if (selectedRobot->m_inMainShip)
        return false;

    switch(id)
    {
    case 0: return selectedRobot->m_hp < selectedRobot->m_hp_total;
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

    map->showAttackGif(selectedRobot->m_x, selectedRobot->m_y, enemy->m_x, enemy->m_y);



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

    if (selectedRobot->m_weapon1->range == 1)
    {
        map->showAttackRange(selectedRobot, selectedRobot->m_weapon1);
    }
    else if (selectedRobot->m_weapon2->range == 1)
    {
        map->showAttackRange(selectedRobot, selectedRobot->m_weapon2);
    }
    else {
        map->showCannotAttackRange(selectedRobot);
    }
}
void SceneMain::attackDone()
{
    deleteMenu();

    if (enemy->m_hp <= 0)
    {
        map->removeRobot(enemy);

        // 获得经验
        int diffLevel = enemy->m_level - selectedRobot->m_level;
        int exp = enemy->m_property.exp_dievalue * enemy->m_level;
        if (diffLevel >= 0)
        {
            diffLevel = diffLevel > 8 ? 8 : diffLevel;
            exp = exp * (diffLevel+2)*0.5;
        }
        else {
            diffLevel = diffLevel < -5 ? -5 : diffLevel;
            exp = exp / (diffLevel * -1 * 2);
        }


        selectedRobot->gotExp(exp);
        map->showText(selectedRobot->m_x, selectedRobot->m_y, "EXP +" + QString::number(exp));

    }
    if (selectedRobot->m_hp <= 0)
    {
        map->removeRobot(selectedRobot);

    }

    robotActionFinished();


    if (battle)
    {
        delete battle;
        battle = 0;
    }

}



void SceneMain::use_sprit_begin(int id)
{
    selectedRobot->m_pilot->spirit -= PeopleProperty::spirit_consume_table[id];

    selectedRobot->m_spirit[id] = true;

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
    selectedRobot->m_hp += 50;

    if (selectedRobot->m_hp > selectedRobot->m_hp_total)
    {
        selectedRobot->m_hp = selectedRobot->m_hp_total;
    }
    map->showText(selectedRobot->m_x, selectedRobot->m_y, QString("+50"));
    robotStatus->showRobot(selectedRobot);


    use_sprit_end(0);
}
void SceneMain::use_sprit_1()	//加速
{
    use_sprit_begin(1);
    
    qDebug() << "use_sprit_1";
    selectedRobot->m_spirit[1] = true;

    map->UnshowMoveRange();
    map->showMoveRange(selectedRobot);

    use_sprit_end(1);
}
void SceneMain::use_sprit_2()	//瞄准
{
    use_sprit_begin(2);
    
    qDebug() << "use_sprit_2";
    selectedRobot->m_spirit[2] = true;


    use_sprit_end(2);
}
void SceneMain::use_sprit_3()	//防守
{
    use_sprit_begin(3);
    
    qDebug() << "use_sprit_3";
    selectedRobot->m_spirit[3] = true;


    use_sprit_end(3);
}
void SceneMain::use_sprit_4()	//强攻
{
    use_sprit_begin(4);
    
    qDebug() << "use_sprit_4";
    selectedRobot->m_spirit[4] = true;


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
    selectedRobot->m_spirit[6] = true;


    use_sprit_end(6);
}
void SceneMain::use_sprit_7()	//疾风
{
    use_sprit_begin(7);
    
    qDebug() << "use_sprit_7";
    selectedRobot->m_spirit[7] = true;

    map->UnshowMoveRange();
    map->showMoveRange(selectedRobot);

    use_sprit_end(7);
}
void SceneMain::use_sprit_8()	//回避
{
    use_sprit_begin(8);
    
    qDebug() << "use_sprit_8";
    selectedRobot->m_spirit[8] = true;


    use_sprit_end(8);
}
void SceneMain::use_sprit_9()	//潜力
{
    use_sprit_begin(9);
    
    qDebug() << "use_sprit_9";

    int hp_plus = selectedRobot->m_hp_total - selectedRobot->m_hp;
    selectedRobot->m_hp += hp_plus;

    if (selectedRobot->m_hp > selectedRobot->m_hp_total)
    {
        selectedRobot->m_hp = selectedRobot->m_hp_total;
    }
    map->showText(selectedRobot->m_x, selectedRobot->m_y, QString("+" + QString::number(hp_plus)));
    robotStatus->showRobot(selectedRobot);

    use_sprit_end(9);
}
void SceneMain::use_sprit_10()	//热血
{
    use_sprit_begin(10);
    
    qDebug() << "use_sprit_10";
    selectedRobot->m_spirit[10] = true;


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
