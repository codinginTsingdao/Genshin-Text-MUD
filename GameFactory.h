#ifndef GAME_FACTORY_H
#define GAME_FACTORY_H

#include "Enemy.h"
#include "Item.h"
#include "World.h"
#include <memory>
#include <string>

class GameFactory {
public:
    static Item createItem(int itemId);
    static int itemIdFromName(const std::string& name);
    static std::unique_ptr<Enemy> createEnemy(int spawnId);
    static bool enemyMatches(int spawnId, const std::string& query);
    static World createWorld();
};

#endif
