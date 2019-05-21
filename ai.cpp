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
            if (map->robots[i][j] && map->robots[i][j]->player != robot->player)
            {
                return map->robots[i][j];
            }
        }
    }
}
