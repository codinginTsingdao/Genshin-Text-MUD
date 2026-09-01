#include "Equipment.h"
#include <utility>

bool Equipment::isValidEquipment(const Item& item) const {
    return (item.type == ItemType::Weapon && item.slot == EquipmentSlot::Weapon) ||
           (item.type == ItemType::Armor && item.slot == EquipmentSlot::Armor);
}

bool Equipment::equip(Inventory& inventory, int itemId) {
    const ItemStack* stack = inventory.findItem(itemId);
    if (!stack || !isValidEquipment(stack->item)) return false;

    const Item newItem = stack->item;
    Inventory updatedInventory = inventory;
    auto updatedEquipment = m_equippedItems;

    if (!updatedInventory.removeItem(newItem.id)) return false;

    auto old = updatedEquipment.find(newItem.slot);
    if (old != updatedEquipment.end()) {
        if (!updatedInventory.addItem(old->second)) return false;
        old->second = newItem;
    } else {
        updatedEquipment.emplace(newItem.slot, newItem);
    }

    inventory = std::move(updatedInventory);
    m_equippedItems = std::move(updatedEquipment);
    return true;
}

bool Equipment::unequip(Inventory& inventory, EquipmentSlot slot) {
    auto it = m_equippedItems.find(slot);
    if (it == m_equippedItems.end()) return false;

    Inventory updatedInventory = inventory;
    if (!updatedInventory.addItem(it->second)) return false;

    auto updatedEquipment = m_equippedItems;
    updatedEquipment.erase(slot);
    inventory = std::move(updatedInventory);
    m_equippedItems = std::move(updatedEquipment);
    return true;
}

const Item* Equipment::getEquippedItem(EquipmentSlot slot) const {
    auto it = m_equippedItems.find(slot);
    return it == m_equippedItems.end() ? nullptr : &it->second;
}

bool Equipment::hasEquipped(EquipmentSlot slot) const { return getEquippedItem(slot) != nullptr; }

StatBonus Equipment::totalBonus() const {
    StatBonus total;
    for (const auto& entry : m_equippedItems) {
        total.attack += entry.second.bonus.attack;
        total.defense += entry.second.bonus.defense;
    }
    return total;
}

const std::map<EquipmentSlot, Item>& Equipment::equippedItems() const { return m_equippedItems; }
void Equipment::clear() { m_equippedItems.clear(); }
