#include "Character.h"
#include <algorithm>
#include <utility>

Character::Character(std::string name, int currentHp, int maxHp, int attack,
                     int defense, int agility, double hitRate, double dodgeRate)
    : m_name(std::move(name)),
      m_currentHp(std::max(0, currentHp)),
      m_maxHp(std::max(1, maxHp)),
      m_attack(std::max(0, attack)),
      m_defense(std::max(0, defense)),
      m_agility(std::max(0, agility)),
      m_hitRate(std::clamp(hitRate, 0.0, 1.0)),
      m_dodgeRate(std::clamp(dodgeRate, 0.0, 1.0)),
      m_alive(currentHp > 0) {
    if (m_currentHp > m_maxHp) m_currentHp = m_maxHp;
}

const std::string& Character::getName() const { return m_name; }
void Character::setName(const std::string& name) { m_name = name; }
int Character::getCurrentHp() const { return m_currentHp; }
int Character::getMaxHp() const { return m_maxHp; }
int Character::getAttack() const { return m_attack; }
int Character::getDefense() const { return m_defense; }
int Character::getAgility() const { return m_agility; }
double Character::getHitRate() const { return m_hitRate; }
double Character::getDodgeRate() const { return m_dodgeRate; }
bool Character::isAlive() const { return m_alive; }

void Character::setCurrentHp(int hp) {
    m_currentHp = std::clamp(hp, 0, m_maxHp);
    m_alive = m_currentHp > 0;
}
void Character::setMaxHp(int hp) {
    m_maxHp = std::max(1, hp);
    if (m_currentHp > m_maxHp) m_currentHp = m_maxHp;
    m_alive = m_currentHp > 0;
}
void Character::setAttack(int attack) { m_attack = std::max(0, attack); }
void Character::setDefense(int defense) { m_defense = std::max(0, defense); }
void Character::setAgility(int agility) { m_agility = std::max(0, agility); }
void Character::setHitRate(double hitRate) { m_hitRate = std::clamp(hitRate, 0.0, 1.0); }
void Character::setDodgeRate(double dodgeRate) { m_dodgeRate = std::clamp(dodgeRate, 0.0, 1.0); }
void Character::setAlive(bool alive) {
    m_alive = alive;
    if (!alive) m_currentHp = 0;
    else if (m_currentHp <= 0) m_currentHp = 1;
}

int Character::effectiveAttack() const { return m_attack; }
int Character::effectiveDefense() const { return m_defense; }

void Character::takeDamage(int damage) { setCurrentHp(m_currentHp - std::max(0, damage)); }
void Character::heal(int amount) { if (amount > 0) setCurrentHp(m_currentHp + amount); }
void Character::revive(int hp) { setCurrentHp(std::max(1, hp)); }
