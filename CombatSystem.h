#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "Enemy.h"
#include "GameTypes.h"
#include "Player.h"
#include <iosfwd>
#include <random>

class CombatSystem {
private:
    std::mt19937 m_rng;

    bool roll(double probability);
    int calculateDamage(const Character& attacker, const Character& defender);
    bool attackHits(const Character& attacker, const Character& defender);
    void playerAttack(Player& player, Enemy& enemy, std::ostream& out);
    void enemyAttack(Enemy& enemy, Player& player, int turn, std::ostream& out);
    bool useItemTurn(Player& player, std::istream& in, std::ostream& out);

public:
    CombatSystem();
    CombatResult fight(Player& player, Enemy& enemy, std::istream& in, std::ostream& out);
};

#endif
