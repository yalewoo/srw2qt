#include "robotstatusview.h"


#include <QBrush>

#include "game.h"
extern Game * game;

#include "people.h"
#include "weapon.h"

#include "robot.h"

#include "config.h"
#include "imageresourcemanager.h"
extern Config * config;

#include <QDebug>

RobotStatusView::RobotStatusView(int width, int height)
{



    setOpacity(0);

    setRect(0, 0, width, height);

    QString s = QString("初始");

    text = new QGraphicsTextItem(s, this);
    text->setScale(1);
    text->setDefaultTextColor(Qt::black);

    game->scene->add(text);

    pilotImage = new QGraphicsPixmapItem(this);

    robotImage = new QGraphicsPixmapItem(this);

    game->scene->add(pilotImage);
    game->scene->add(robotImage);
}

void RobotStatusView::setString(QString s)
{
    text->setPlainText(s);
}

void RobotStatusView::showPilotImage(Robot *robot)
{
    pilotImage->setPixmap(ImageResourceManager::getPeopleImage(robot->m_pilot->id));
}

void RobotStatusView::showRobotImage(Robot *robot)
{
    robotImage->setPixmap(ImageResourceManager::getRobotImage(robot->m_id));
}

void RobotStatusView::showRobot(Robot *robot)
{
    People * pilot = robot->m_pilot;

    QString s;
    QString tmp;
    static QString t2[2] = {QString("我方"), QString("敌方")};
    s += robot->m_property.robotName + "\t" + t2[robot->m_player] +"\n";
    s += QString("水平\t") + QString::number(robot->m_level) + "\t类型\t";
    static QString table[256] = {QString("空"), QString("陆"), QString("海")};
    s += table[robot->m_property.type] + "\n机动\t";
    s += QString::number(robot->m_move) + "\n";
    s += QString("精神\t") + QString::number(pilot->spirit) + "/" + QString::number(pilot->spirit_total);


    s += "\n\n";


    s += QString("强度\t") + QString::number(robot->m_strength);
    s += "  ";
    s += QString("HP\t") + QString::number(robot->m_hp) + " / " + QString::number(robot->m_hp_total);
    s += "\n";
    s += QString("防卫\t") + QString::number(robot->m_defense);
    s += "  ";
    s += QString("EXP\t") + QString::number(pilot->exp);
    s += "\n";
    s += QString("速度\t") + QString::number(robot->m_speed);
    s += "  ";
    s += QString("升级还需  ") + QString::number(Robot::exp_update_table[robot->m_level] - pilot->exp);
    s += "\n\n";

    s+= QString("驾驶员\t") + pilot->property.name + "\n\n";

    Weapon * weapon1 = robot->m_weapon1;
    Weapon * weapon2 = robot->m_weapon2;
    if (weapon1)
    {
        s += weapon1->name + QString("\n命中 ") + QString::number(weapon1->hitRadio) + "\t";
        s += QString("射程\t") + QString::number(weapon1->range) + " ";

        int sky = weapon2->firepower[0] == 0 ? 0 :  weapon2->firepower[2] + robot->m_strength;
        int land = weapon2->firepower[1] == 0 ? 0 :  weapon2->firepower[1] + robot->m_strength;
        int sea = weapon2->firepower[2] == 0 ? 0 :  weapon2->firepower[0] + robot->m_strength;

        tmp.sprintf("\n  火力：空 %d\t陆 %d\t海 %d", sky, land, sea);
        s += tmp;
    }
    if (weapon2)
    {
        s += "\n\n";
        s += weapon2->name + QString("\n命中 ") + QString::number(weapon2->hitRadio) + "\t";
        s += QString("射程\t") + QString::number(weapon2->range) + " ";

        int sky = weapon2->firepower[0] == 0 ? 0 :  weapon2->firepower[2] + robot->m_strength;
        int land = weapon2->firepower[1] == 0 ? 0 :  weapon2->firepower[1] + robot->m_strength;
        int sea = weapon2->firepower[2] == 0 ? 0 :  weapon2->firepower[0] + robot->m_strength;

        tmp.sprintf("\n  火力：空 %d\t陆 %d\t海 %d", sky, land, sea);
        s += tmp;
    }


    setString(s);

    QFile file(config->html_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString html = file.readAll();
    file.close();

    html = html.replace("\n", "");
    html = html.replace("\t", "");
    html = html.replace("##robotName##", robot->m_property.robotName);
    html = html.replace("##hp##", QString::number(robot->m_hp));
    html = html.replace("##hp_total##", QString::number(robot->m_hp_total));
    html = html.replace("##level##", QString::number( robot->m_level ));
    html = html.replace("##type##", QString( table[robot->m_property.type] ));
    html = html.replace("##move##", QString::number( robot->m_move ));

    html = html.replace("##strength#", QString::number( robot->m_strength  ) );
    html = html.replace("##spirit#", QString::number( robot->m_pilot->spirit  ) );
    html = html.replace("##spirit_total#", QString::number( robot->m_pilot->spirit_total  ) );
    html = html.replace("##defense#", QString::number( robot->m_defense  ) );
    html = html.replace("##exp#", QString::number( pilot->exp  ) );
    html = html.replace("##speed#", QString::number( robot->m_speed  ) );
    html = html.replace("##level_up_exp#", QString::number(Robot::exp_update_table[robot->m_level] - pilot->exp) );


    html = html.replace("##pilot#", QString( robot->m_pilot->property.name  ) );

    int sky1 = 0;
    int land1 = 0;
    int sea1 = 0;
    int sky2 = 0;
    int land2 = 0;
    int sea2 = 0;
    if (weapon1)
    {
        sky1 = weapon1->firepower[0] == 0 ? 0 :  weapon1->firepower[2] + robot->m_strength;
        land1 = weapon1->firepower[1] == 0 ? 0 :  weapon1->firepower[1] + robot->m_strength;
        sea1 = weapon1->firepower[2] == 0 ? 0 :  weapon1->firepower[0] + robot->m_strength;
    }
    if (weapon2)
    {
        sky2 = weapon2->firepower[0] == 0 ? 0 :  weapon2->firepower[2] + robot->m_strength;
        land2 = weapon2->firepower[1] == 0 ? 0 :  weapon2->firepower[1] + robot->m_strength;
        sea2 = weapon2->firepower[2] == 0 ? 0 :  weapon2->firepower[0] + robot->m_strength;
    }

    html = html.replace("##weapon_1_name#", QString( robot->m_weapon1->name  ) );
    html = html.replace("##weapon_1_hitRadio#", QString::number( robot->m_weapon1->hitRadio  ) );
    html = html.replace("##weapon_1_range#", QString::number( robot->m_weapon1->range  ) );
    html = html.replace("##sky1#", QString::number( sky1  ) );
    html = html.replace("##land1#", QString::number( land1  ) );
    html = html.replace("##sea1#", QString::number( sea1 ) );
    html = html.replace("##weapon_2_name#", QString( robot->m_weapon2->name  ) );
    html = html.replace("##weapon_2_hitRadio#", QString::number( robot->m_weapon2->hitRadio  ) );
    html = html.replace("##weapon_2_range#", QString::number( robot->m_weapon2->range  ) );
    html = html.replace("##sky2#", QString::number( sky2  ) );
    html = html.replace("##land2#", QString::number( land2  ) );
    html = html.replace("##sea2#", QString::number( sea2  ) );

    QString usedSpirit = "";
    for (int i = 0; i < 19; ++i)
    {
        if (PeopleProperty::spirit_type_table[i] == 0)
        {
            continue;
        }
        if (robot->m_spirit[i])
        {
            usedSpirit += QString(robot->m_pilot->property.spirit_name[i]) + " ";
        }
    }
    if (usedSpirit.length() == 0)
    {
        html = html.replace("##used_spirit#", "&nbsp;");

    }
    else {
        html = html.replace("##used_spirit#", "Buff: " + usedSpirit);

    }

    text->setHtml(html);
    qDebug() << html << "\n";

    showPilotImage(robot);
    showRobotImage(robot);

}

void RobotStatusView::updateXY(int x, int y)
{
    setPos(x, y);
    text->setPos(this->x(), this->y());
    pilotImage->setPos(this->x() + 220, this->y() + 370);
    robotImage->setPos(this->x() + 10, this->y() + 10);

}
