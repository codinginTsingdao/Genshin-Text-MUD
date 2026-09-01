#include "Player.h"
#include <algorithm>
#include <sstream>

Player::Player(const std::string& name)
    : Character(name, 100, 100, 18, 8, 12, 0.88, 0.08), m_inventory(20) {}

int Player::getLevel() const { return m_level; }
int Player::getExp() const { return m_exp; }
int Player::getMora() const { return m_mora; }

void Player::setLevel(int level) { m_level = std::max(1, level); }
void Player::setExp(int exp) { m_exp = std::max(0, exp); }
void Player::setMora(int mora) { m_mora = std::max(0, mora); }


void Player::gainExp(int amount) {
    if (amount <= 0) return;
    m_exp += amount;
    while (m_exp >= m_level * 100) {
        m_exp -= m_level * 100;
        ++m_level;
        setMaxHp(getMaxHp() + 12);
        setAttack(getAttack() + 3);
        setDefense(getDefense() + 2);
        setCurrentHp(getMaxHp());
    }
}

void Player::addMora(int amount) { if (amount > 0) m_mora += amount; }
bool Player::spendMora(int amount) {
    if (amount < 0 || m_mora < amount) return false;
    m_mora -= amount;
    return true;
}

Inventory& Player::inventory() { return m_inventory; }
const Inventory& Player::inventory() const { return m_inventory; }
Equipment& Player::equipment() { return m_equipment; }
const Equipment& Player::equipment() const { return m_equipment; }

int Player::effectiveAttack() const { return getAttack() + m_equipment.totalBonus().attack; }
int Player::effectiveDefense() const { return getDefense() + m_equipment.totalBonus().defense; }

bool Player::useConsumable(int itemId, std::string& message) {
    const ItemStack* stack = m_inventory.findItem(itemId);
    if (!stack) {
        message = "背包中没有这个物品。";
        return false;
    }

    if (stack->item.type != ItemType::Consumable) {
        message = "这个物品不能直接使用。";
        return false;
    }

    Item item = stack->item;

    const bool canRestoreHp =
        item.healAmount > 0 && getCurrentHp() < getMaxHp();

    if (!m_inventory.removeItem(itemId, 1)) {
        message = "使用物品失败。";
        return false;
    }

    const int hpBefore = getCurrentHp();

    if (item.healAmount > 0) {
        heal(item.healAmount);
    }


    const int hpRecovered = getCurrentHp() - hpBefore;

    std::ostringstream out;
    out << "使用「" << item.name << "」";

    bool firstEffect = true;
    if (hpRecovered > 0) {
        out << "，恢复 " << hpRecovered << " 点生命";
        firstEffect = false;
    }


    out << "。";
    message = out.str();
    return true;
}
