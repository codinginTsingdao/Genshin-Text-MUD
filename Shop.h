#ifndef SHOP_H
#define SHOP_H

#include "Item.h"
#include "Player.h"
#include <string>
#include <vector>

struct ShopEntry {
    int itemId;
    int buyPrice;
};

class Shop {
private:
    std::vector<ShopEntry> m_catalog;

public:
    Shop();

    const std::vector<ShopEntry>& catalog() const;
    int sellPrice(const Item& item) const;
    bool buy(Player& player, int itemId, std::string& message) const;
    bool sell(Player& player, int itemId, std::string& message) const;
};

#endif
