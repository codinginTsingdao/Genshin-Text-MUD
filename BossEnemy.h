#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H

#include "Enemy.h"

class BossEnemy : public Enemy {
private:
    int m_shield;
    int m_maxShield;
    int m_phase{1};

public:
    BossEnemy(int spawnId, const std::string& name, int hp, int shield,
              int attack, int defense, int agility, double hitRate,
              double dodgeRate, int expReward, int moraReward);

    bool isBoss() const override;
    int getShield() const;
    int getMaxShield() const;
    int getPhase() const;
    bool shieldActive() const;
    int damageShield(int damage);
    void enterPhaseTwo();
    int effectiveAttack() const override;
};

#endif
