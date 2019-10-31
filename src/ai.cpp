#include "ai.h"

AI::AI()
{

}

Robot * AI::GetEnemy(Map *map, Robot *robot)
{
    for (int i = 1; i < map->width - 1; ++i)
    {
        for (int j = 1; j < map->height - 1; ++j)
        {
            if (map->robots[i][j] && map->robots[i][j]->m_player != robot->m_player)
            {
                return map->robots[i][j];
            }
        }
    }
}
QVector<Robot *> AI::GetAllAttackTargetRobots(Map *map, Robot *robot)
{
    for (int i = 0; i < map->width; ++i)
    {
        for (int j = 0; j < map->height; ++j)
        {
            if (map->robots[i][j] && map->robots[i][j]->m_player != robot->m_player)
            {
                map->robots[i][j]->m_tmp_ai_weapon1 = false;
                map->robots[i][j]->m_tmp_ai_weapon2 = false;

            }
        }
    }
    QMap<QPair<int, int>, bool> m;

    QVector<Robot *> enemys;
    // 远程
    if (robot->m_weapon1)
    {
        QVector<QVector<int> > w1 = map->calculateAttackRange(robot, robot->m_weapon1);
        for (int i = 0; i < map->width; ++i)
        {
            for (int j = 0; j < map->height; ++j)
            {
                if (map->robots[i][j] && map->robots[i][j]->m_player != robot->m_player)
                {
                    if (w1[i][j] >= 0 && map->canAttack(robot, robot->m_weapon1, map->robots[i][j]))
                    {
                        if (!m.contains(QPair<int,int>(i, j)))
                        {
                            m.insert(QPair<int,int>(i, j), true);
                            enemys.push_back(map->robots[i][j]);
                        }
                        map->robots[i][j]->m_tmp_ai_weapon1 = true;
                    }
                }
            }
        }
    }
    if (robot->m_weapon2)
    {
        QVector<QVector<int> > w1 = map->calculateAttackRange(robot, robot->m_weapon2);
        for (int i = 0; i < map->width; ++i)
        {
            for (int j = 0; j < map->height; ++j)
            {
                if (map->robots[i][j] && map->robots[i][j]->m_player != robot->m_player)
                {
                    if (w1[i][j] > 0 && map->canAttack(robot, robot->m_weapon1, map->robots[i][j]))
                    {
                        if (!m.contains(QPair<int,int>(i, j)))
                        {
                            m.insert(QPair<int,int>(i, j), true);
                            enemys.push_back(map->robots[i][j]);
                        }
                        map->robots[i][j]->m_tmp_ai_weapon2 = true;
                    }
                }
            }
        }
    }

    // 移动后近攻
    QVector<QVector<int> > movemap = map->calculateMoveRange(robot);
    for (int i = 0; i < map->width; ++i)
    {
        for (int j = 0; j < map->height; ++j)
        {
            if (map->robots[i][j] && map->robots[i][j]->m_player != robot->m_player)
            {
                if (i == 0 || j == 0)
                {

                }
                else if ((   movemap[i-1][j] >= 0 && !map->robots[i-1][j])
                         || (movemap[i+1][j] >= 0 &&  !map->robots[i+1][j])
                         || (movemap[i][j-1] >= 0 &&  !map->robots[i][j-1])
                         || (movemap[i][j+1] >= 0 &&  !map->robots[i][j+1])  )
                {
                    if (robot->m_weapon1->range == 1 && robot->m_weapon1->firepower[map->robots[i][j]->m_property.type] > 0 )
                    {
                        if (!m.contains(QPair<int,int>(i, j)))
                        {
                            m.insert(QPair<int,int>(i, j), true);
                            enemys.push_back(map->robots[i][j]);
                        }
                        map->robots[i][j]->m_tmp_ai_weapon1 = true;
                    }
                    if ( robot->m_weapon2->range == 1 && robot->m_weapon2->firepower[map->robots[i][j]->m_property.type] > 0 )
                    {
                        if (!m.contains(QPair<int,int>(i, j)))
                        {
                            m.insert(QPair<int,int>(i, j), true);
                            enemys.push_back(map->robots[i][j]);
                        }
                        map->robots[i][j]->m_tmp_ai_weapon2 = true;
                    }
                }

            }
        }
    }

    return enemys;
}
QVector<Robot *> AI::GetRobotList(Map *map)
{
    QVector<Robot *> robots;
    QVector<Robot *> enemys;
    for (int i = 0; i < map->width; ++i)
    {
        for (int j = 0; j < map->height; ++j)
        {
            if (map->robots[i][j])
            {
                if (map->robots[i][j]->m_player == 0)
                {
                    robots.push_back(map->robots[i][j]);
                }
                else {
                    enemys.push_back(map->robots[i][j]);
                }
            }
        }
    }

    QVector<int> enemys_rank;
    for (int enemy_index = 0; enemy_index < enemys.length(); ++enemy_index)
    {
        Robot * enemy = enemys[enemy_index];
        QVector<QVector<int> > m = map->calculateMoveRange(enemy, -1, -1, 999, true);

        qDebug() << enemy->m_x << enemy->m_y << "===\n";
        int min = 999;
        for (int k = 0; k < robots.length(); ++k)
        {
            Robot * robot = robots[k];
            int x = robot->m_x;
            int y = robot->m_y;
            if (999 - m[x][y] < min)
            {
                qDebug() << x << y << 999 - m[x][y];
                min = 999 - m[x][y];
            }
        }

        enemys_rank.push_back(min);
    }

    for (int i = 1; i < enemys_rank.length(); ++i)
    {
        int tmp = enemys_rank[i];
        Robot * tmp2 = enemys[i];
        int j = i;
        while (j - 1 >= 0 && tmp < enemys_rank[j - 1])
        {
            enemys[j] = enemys[j-1];
            enemys_rank[j] = enemys_rank[j-1];
            --j;
        }
        enemys_rank[j] = tmp;
        enemys[j] = tmp2;
    }

    qDebug() << "order enemys_rank";
    for (int i = 0; i < enemys_rank.length(); ++i)
    {

        qDebug() << enemys_rank[i] << enemys[i]->m_x << enemys[i]->m_y;
    }

    return enemys;
}
