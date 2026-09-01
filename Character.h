#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
private:
    std::string m_name;
    int m_currentHp;
    int m_maxHp;
    int m_attack;
    int m_defense;
    int m_agility;
    double m_hitRate;
    double m_dodgeRate;
    bool m_alive;

public:
    Character(std::string name, int currentHp, int maxHp, int attack,
              int defense, int agility, double hitRate, double dodgeRate);
    virtual ~Character() = default;

    const std::string& getName() const;
    void setName(const std::string& name);

    int getCurrentHp() const;
    int getMaxHp() const;
    int getAttack() const;
    int getDefense() const;
    int getAgility() const;
    double getHitRate() const;
    double getDodgeRate() const;
    bool isAlive() const;

    void setCurrentHp(int hp);
    void setMaxHp(int hp);
    void setAttack(int attack);
    void setDefense(int defense);
    void setAgility(int agility);
    void setHitRate(double hitRate);
    void setDodgeRate(double dodgeRate);
    void setAlive(bool alive);

    virtual int effectiveAttack() const;
    virtual int effectiveDefense() const;

    void takeDamage(int damage);
    void heal(int amount);
    void revive(int hp = 1);
};

#endif
