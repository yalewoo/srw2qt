#include "story.h"

#include "game.h"
extern Game * game;

#include "storydiag.h"

#include <QTextCodec>

#include "config.h"
extern Config * config;

Story::Story()
{
    QString filename = config->conversion_path;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << filename << "open failed";
        exit(-1);

    }


    QTextStream in(&file);

    QString line = in.readLine();

    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    in.setCodec(codec);
    while ( in.atEnd()==0 )
    {
        line = codec->fromUnicode(in.readLine());


        QStringList t = line.split(",");

        if (t.size() == 0)
            continue;

        int turn = QString(t[0]).toInt();
        int index = QString(t[1]).toInt();

        int people = QString(t[2]).toInt();
        int robot = QString(t[3]).toInt();

        QString says = t[4];

        while (conversitions.length() <= turn)
        {
            conversitions.push_back(QVector<Conversation>());
        }
        Conversation cons(people, robot, says);
        conversitions[turn].push_back(cons);
    }
}

void Story::showConversition(int turn)
{
    if (conversitions.length() > turn && conversitions[turn].length()>0)
    {
        StoryDiag * storyDiag = new StoryDiag(conversitions[turn]);

        storyDiag->setSize(game->scene->map->width*32, game->scene->map->height*32);
        game->scene->addItem(storyDiag);

        waitVariableToBeTrueAllEvents(storyDiag->finished);


        game->scene->showDiagDone();
        game->scene->removeItem(storyDiag);
        delete storyDiag;
    }
}


