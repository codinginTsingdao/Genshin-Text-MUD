#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Equipment.h"
#include "Inventory.h"
#include <string>

class Player : public Character {
private:
    int m_level{1};
    int m_exp{0};
    int m_mora{0};
    int m_tp{80};
    int m_energy{0};
    Inventory m_inventory;
    Equipment m_equipment;

public:
    explicit Player(const std::string& name = "旅行者");

    int getLevel() const;
    int getExp() const;
    int getMora() const;
    int getTP() const;
    int getMaxTP() const;
    int getEnergy() const;

    // 兼容你们原 Player 命名
    int getLV() const { return getLevel(); }
    int getNHP() const { return getCurrentHp(); }
    int getMHP() const { return getMaxHp(); }
    int getENERGY() const { return getEnergy(); }

    // 兼容原仓库中的 setter 命名
    void setLV(int level) { setLevel(level); }
    void setNHP(int hp) { setCurrentHp(hp); }
    void setMHP(int hp) { setMaxHp(hp); }
    void setENERGY(int energy) { setEnergy(energy); }

    void setLevel(int level);
    void setExp(int exp);
    void setMora(int mora);
    void setTP(int tp);
    void setEnergy(int energy);

    void gainExp(int amount);
    void addMora(int amount);
    bool spendMora(int amount);

    Inventory& inventory();
    const Inventory& inventory() const;
    Equipment& equipment();
    const Equipment& equipment() const;

    int effectiveAttack() const override;
    int effectiveDefense() const override;

    bool useConsumable(int itemId, std::string& message);
};

#endif
