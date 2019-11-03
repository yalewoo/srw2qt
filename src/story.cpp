#include "story.h"

#include "game.h"
extern Game * game;

#include "storydiag.h"

#include <QTextCodec>

#include "datareader/config.h"
extern Config * config;

Story::Story(int stage, Scene * scene)
{
    m_stage = stage;
    this->scene = scene;
}
void Story::setStage(int stage)
{
    m_stage = stage;
}


void Story::execute(int turn)
{
    QString filename = config->getPlaybookPath(m_stage, turn);

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << filename << "open failed";
        return;

    }


    QTextStream in(&file);

    //QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    //in.setCodec(codec);
    QVector<Conversation> conversations;
    QVector<EnemyData> enemys;
    QVector<RunTimeRobotData> robots;

    QString state = "start";
    while ( in.atEnd()==0 )
    {
        //line = codec->fromUnicode(in.readLine());
        QString line = in.readLine();

        QStringList t = line.split(",");

        if (t.size() == 0)
            continue;


        QString type = QString(t[0]);
        if (state == type)
        {
            state = QString("start");
            if (type == QString("conversation"))
            {
                // 对话结束
                StoryDiag * storyDiag = new StoryDiag(conversations);

                storyDiag->setSize(this->scene->getConversationPosition());
                this->scene->addItem(storyDiag);

                waitVariableToBeTrueAllEvents(storyDiag->finished);


                this->scene->removeItem(storyDiag);
                delete storyDiag;
                conversations.clear();
            }
            else if (type == QString("add enemy"))
            {
                // 放置结束
                game->musicManager->PlayOnceMusic("8D.wav");
                game->scene->map->placeEnemy(enemys);
                enemys.clear();
            }
            else if (type == QString("add robot"))
            {
                // 放置结束
                game->musicManager->PlayOnceMusic("85.wav");
                game->scene->map->placeRobotRunTime(robots);
            }
            continue;
        }
        if (type == QString("conversation"))
        {
            state = QString("conversation");
            continue;
        }
        else if (type == QString("add enemy"))
        {
            state = QString("add enemy");
            continue;
        }
        else if (type == QString("add robot"))
        {
            state = QString("add robot");
            continue;
        }


        if (state == QString("conversation"))
        {
            int people = QString(t[0]).toInt();
            int robot = QString(t[1]).toInt();

            QString says = t[2];


            Conversation cons(people, robot, says);
            conversations.push_back(cons);




        }
        else if (state == QString("add enemy"))
        {
            EnemyData enemy;

            int i = 0;
            enemy.x = QString(t[i++]).toInt() + 1;
            enemy.y = QString(t[i++]).toInt() + 1;
            enemy.peopleId = QString(t[i++]).toInt();
            enemy.robotId = QString(t[i++]).toInt();
            enemy.robotLevel = QString(t[i++]).toInt();
            enemy.robotBehavior = QString(t[i++]).toInt();
            i++;    //people name
            i++;    //robot name
            enemy.robotBehavior = QString(t[i++]).toInt();

            enemys.push_back(enemy);
        }
        else if (state == QString("add robot"))
        {
            RunTimeRobotData robotData;
            int i = 0;
            robotData.x = QString(t[i++]).toInt() + 1;
            robotData.y = QString(t[i++]).toInt() + 1;
            robotData.level = QString(t[i++]).toInt();
            robotData.peopleId = QString(t[i++]).toInt();
            i++;    // people Name
            robotData.robotId = QString(t[i++]).toInt();

            robots.push_back(robotData);
        }
    }
}


