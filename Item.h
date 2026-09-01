#ifndef ITEM_H
#define ITEM_H

#include <string>

enum class ItemType { Consumable, Weapon, Armor, Quest };
enum class EquipmentSlot { None, Weapon, Armor };

struct StatBonus {
    int attack{0};
    int defense{0};
};

struct Item {
    int id{0};
    std::string name;
    ItemType type{ItemType::Consumable};
    EquipmentSlot slot{EquipmentSlot::None};
    std::string description;
    bool stackable{true};
    int healAmount{0};
    StatBonus bonus;
};

#endif
