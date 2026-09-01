#include "Enemy.h"
#include <utility>

Enemy::Enemy(int spawnId, const std::string& name, int hp, int attack, int defense,
             int agility, double hitRate, double dodgeRate,
             int expReward, int moraReward,
             std::vector<std::pair<int, int>> drops)
    : Character(name, hp, hp, attack, defense, agility, hitRate, dodgeRate),
      m_spawnId(spawnId), m_expReward(expReward), m_moraReward(moraReward),
      m_drops(std::move(drops)) {}

int Enemy::getSpawnId() const { return m_spawnId; }
int Enemy::getExpReward() const { return m_expReward; }
int Enemy::getMoraReward() const { return m_moraReward; }
const std::vector<std::pair<int, int>>& Enemy::getDrops() const { return m_drops; }
bool Enemy::isBoss() const { return false; }
