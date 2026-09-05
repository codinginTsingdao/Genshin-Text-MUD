#include "Shop.h"
#include "GameFactory.h"
#include <sstream>

Shop::Shop()
    : m_catalog{{12, 100}, {2, 180}, {13, 140}} {}

const std::vector<ShopEntry>& Shop::catalog() const { return m_catalog; }

int Shop::sellPrice(const Item& item) const {
    if (item.type == ItemType::Quest) return 0;

    switch (item.id) {
        case 1: return 20;
        case 2: return 90;
        case 3: return 15;
        case 5: return 3;
        case 6: return 12;
        case 7: return 25;
        case 11: return 15;
        case 12: return 50;
        case 13: return 70;
        default: return 0;
    }
}

bool Shop::buy(Player& player, int itemId, std::string& message) const {
    const ShopEntry* selected = nullptr;
    for (const ShopEntry& entry : m_catalog) {
        if (entry.itemId == itemId) {
            selected = &entry;
            break;
        }
    }

    if (!selected) {
        message = "铁匠铺没有出售这个物品。";
        return false;
    }

    const Item item = GameFactory::createItem(itemId);
    if (player.getMora() < selected->buyPrice) {
        std::ostringstream out;
        out << "摩拉不足。购买「" << item.name << "」需要 "
            << selected->buyPrice << " 摩拉。";
        message = out.str();
        return false;
    }

    if (!player.inventory().addItem(item)) {
        message = "背包空间不足，无法购买。";
        return false;
    }

    if (!player.spendMora(selected->buyPrice)) {
        player.inventory().removeItem(item.id);
        message = "交易失败，摩拉和背包均未改变。";
        return false;
    }

    std::ostringstream out;
    out << "购买「" << item.name << "」成功，花费 "
        << selected->buyPrice << " 摩拉。";
    message = out.str();
    return true;
}

bool Shop::sell(Player& player, int itemId, std::string& message) const {
    const ItemStack* stack = player.inventory().findItem(itemId);
    if (!stack) {
        message = "背包中没有这个物品。";
        return false;
    }

    const Item item = stack->item;
    if (item.type == ItemType::Quest) {
        message = "任务物品不能出售。";
        return false;
    }

    const int price = sellPrice(item);
    if (price <= 0) {
        message = "瓦格纳不收购这个物品。";
        return false;
    }

    if (!player.inventory().removeItem(itemId)) {
        message = "出售失败，摩拉和背包均未改变。";
        return false;
    }

    player.addMora(price);
    std::ostringstream out;
    out << "出售「" << item.name << "」成功，获得 " << price << " 摩拉。";
    message = out.str();
    return true;
}
