#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <utility>
#include <vector>

class Enemy : public Character {
private:
    int m_spawnId;
    int m_expReward;
    int m_moraReward;
    std::vector<std::pair<int, int>> m_drops;

public:
    Enemy(int spawnId, const std::string& name, int hp, int attack, int defense,
          int agility, double hitRate, double dodgeRate,
          int expReward, int moraReward,
          std::vector<std::pair<int, int>> drops = {});
    virtual ~Enemy() = default;

    int getSpawnId() const;
    int getExpReward() const;
    int getMoraReward() const;
    const std::vector<std::pair<int, int>>& getDrops() const;
    virtual bool isBoss() const;
};

#endif
