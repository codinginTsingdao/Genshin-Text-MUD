#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include "Player.h"
#include "QuestManager.h"
#include <set>
#include <string>

class SaveManager {
public:
    static bool save(const std::string& path, const Player& player, int roomId,
                     const QuestManager& quest, const std::set<int>& defeatedEnemies,
                     std::string& error);

    static bool load(const std::string& path, Player& player, int& roomId,
                     QuestManager& quest, std::set<int>& defeatedEnemies,
                     std::string& error);
};

#endif
