#include "BossEnemy.h"
#include <algorithm>

BossEnemy::BossEnemy(int spawnId, const std::string& name, int hp, int shield,
                     int attack, int defense, int agility, double hitRate,
                     double dodgeRate, int expReward, int moraReward)
    : Enemy(spawnId, name, hp, attack, defense, agility, hitRate, dodgeRate,
            expReward, moraReward),
      m_shield(std::max(0, shield)), m_maxShield(std::max(0, shield)) {}

bool BossEnemy::isBoss() const { return true; }
int BossEnemy::getShield() const { return m_shield; }
int BossEnemy::getMaxShield() const { return m_maxShield; }
int BossEnemy::getPhase() const { return m_phase; }
bool BossEnemy::shieldActive() const { return m_shield > 0; }
int BossEnemy::damageShield(int damage) {
    int before = m_shield;
    m_shield = std::max(0, m_shield - std::max(0, damage));
    return before - m_shield;
}
void BossEnemy::enterPhaseTwo() { m_phase = 2; }
int BossEnemy::effectiveAttack() const {
    return Enemy::effectiveAttack() + (m_phase == 2 ? 7 : 0);
}
