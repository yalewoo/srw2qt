#include "story.h"

#include "game.h"
extern Game * game;

#include "storydiag.h"

#include <QTextCodec>

#include "datareader/config.h"
extern Config * config;

Story::Story(int stage)
{
    m_stage = stage;
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

                storyDiag->setSize(game->scene->map->width*32, game->scene->map->height*32);
                game->scene->addItem(storyDiag);

                waitVariableToBeTrueAllEvents(storyDiag->finished);


                game->scene->showDiagDone();
                game->scene->removeItem(storyDiag);
                delete storyDiag;
                conversations.clear();
            }
            else if (type == QString("add enemy"))
            {
                // 放置结束
                game->musicManager->PlayOnceMusic("8D.wav");
                game->scene->map->placeEnemy(enemys);
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
    }
}


