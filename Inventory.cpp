#include "Inventory.h"
#include <algorithm>
#include <limits>

Inventory::Inventory(std::size_t capacity) : m_capacity(capacity) {}

bool Inventory::addItem(const Item& item, int quantity) {
    if (item.id <= 0 || quantity <= 0) return false;

    auto found = std::find_if(m_items.begin(), m_items.end(),
        [&item](const ItemStack& stack) { return stack.item.id == item.id; });

    if (found != m_items.end() && item.stackable) {
        if (quantity > std::numeric_limits<int>::max() - found->quantity) return false;
        found->quantity += quantity;
        return true;
    }

    if (item.stackable) {
        if (isFull()) return false;
        m_items.push_back(ItemStack{item, quantity});
        return true;
    }

    const std::size_t needed = static_cast<std::size_t>(quantity);
    if (needed > m_capacity - std::min(m_capacity, m_items.size())) return false;
    for (int i = 0; i < quantity; ++i) m_items.push_back(ItemStack{item, 1});
    return true;
}

bool Inventory::removeItem(int itemId, int quantity) {
    if (itemId <= 0 || quantity <= 0 || quantityOf(itemId) < quantity) return false;

    int remaining = quantity;
    for (auto it = m_items.begin(); it != m_items.end() && remaining > 0;) {
        if (it->item.id != itemId) {
            ++it;
            continue;
        }
        int removed = std::min(it->quantity, remaining);
        it->quantity -= removed;
        remaining -= removed;
        if (it->quantity == 0) it = m_items.erase(it);
        else ++it;
    }
    return true;
}

bool Inventory::discardItem(int itemId, int quantity) {
    const ItemStack* stack = findItem(itemId);
    if (!stack || stack->item.type == ItemType::Quest) return false;
    return removeItem(itemId, quantity);
}

const ItemStack* Inventory::findItem(int itemId) const {
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [itemId](const ItemStack& stack) { return stack.item.id == itemId; });
    return it == m_items.end() ? nullptr : &(*it);
}

const ItemStack* Inventory::findItemByName(const std::string& name) const {
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [&name](const ItemStack& stack) { return stack.item.name == name; });
    return it == m_items.end() ? nullptr : &(*it);
}

int Inventory::quantityOf(int itemId) const {
    long long total = 0;
    for (const auto& stack : m_items) {
        if (stack.item.id == itemId) total += stack.quantity;
    }
    if (total > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
    return static_cast<int>(total);
}

bool Inventory::isFull() const { return m_items.size() >= m_capacity; }
std::size_t Inventory::getUsedSlots() const { return m_items.size(); }
std::size_t Inventory::getCapacity() const { return m_capacity; }
const std::vector<ItemStack>& Inventory::items() const { return m_items; }
void Inventory::clear() { m_items.clear(); }
