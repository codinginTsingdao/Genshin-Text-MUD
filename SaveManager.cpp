#include "SaveManager.h"
#include "GameFactory.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>

bool SaveManager::save(const std::string& path, const Player& player, int roomId,
                       const QuestManager& quest, const std::set<int>& defeatedEnemies,
                       std::string& error) {
    std::ofstream out(path);
    if (!out) {
        error = "无法创建存档文件：" + path;
        return false;
    }

    out << "GENSHINN_SAVE 1\n";
    out << "NAME " << std::quoted(player.getName()) << "\n";
    out << "HP " << player.getCurrentHp() << ' ' << player.getMaxHp() << "\n";
    out << "BASE " << player.getAttack() << ' ' << player.getDefense() << ' '
        << player.getAgility() << ' ' << player.getHitRate() << ' ' << player.getDodgeRate() << "\n";
    out << "PROGRESS " << player.getLevel() << ' ' << player.getExp() << ' '
        << player.getMora() << ' ' << player.getTP() << ' ' << player.getEnergy() << "\n";
    out << "ROOM " << roomId << "\n";
    out << "QUEST " << static_cast<int>(quest.getStage()) << ' ' << quest.getSealsBroken() << "\n";

    out << "INVENTORY " << player.inventory().items().size() << "\n";
    for (const auto& stack : player.inventory().items()) {
        out << stack.item.id << ' ' << stack.quantity << "\n";
    }

    out << "EQUIPMENT " << player.equipment().equippedItems().size() << "\n";
    for (const auto& entry : player.equipment().equippedItems()) {
        out << static_cast<int>(entry.first) << ' ' << entry.second.id << "\n";
    }

    out << "DEFEATED " << defeatedEnemies.size() << "\n";
    for (int id : defeatedEnemies) out << id << "\n";
    out << "END\n";

    if (!out) {
        error = "写入存档时发生错误。";
        return false;
    }
    return true;
}

bool SaveManager::load(const std::string& path, Player& player, int& roomId,
                       QuestManager& quest, std::set<int>& defeatedEnemies,
                       std::string& error) {
    std::ifstream in(path);
    if (!in) {
        error = "没有找到存档文件：" + path;
        return false;
    }

    std::string magic;
    int version = 0;
    if (!(in >> magic >> version) || magic != "GENSHINN_SAVE" || version != 1) {
        error = "存档格式不正确或版本不受支持。";
        return false;
    }

    std::string key, name;
    int hp, maxHp, attack, defense, agility;
    double hitRate, dodgeRate;
    int level, exp, mora, tp, energy;
    int questStage, seals;

    if (!(in >> key) || key != "NAME" || !(in >> std::quoted(name))) { error = "存档缺少 NAME 字段。"; return false; }
    if (!(in >> key >> hp >> maxHp) || key != "HP") { error = "存档 HP 字段损坏。"; return false; }
    if (!(in >> key >> attack >> defense >> agility >> hitRate >> dodgeRate) || key != "BASE") { error = "存档 BASE 字段损坏。"; return false; }
    if (!(in >> key >> level >> exp >> mora >> tp >> energy) || key != "PROGRESS") { error = "存档 PROGRESS 字段损坏。"; return false; }
    if (!(in >> key >> roomId) || key != "ROOM" || roomId < 1 || roomId > 16) { error = "存档 ROOM 字段损坏。"; return false; }
    if (!(in >> key >> questStage >> seals) || key != "QUEST" || questStage < 0 || questStage > 9) { error = "存档 QUEST 字段损坏。"; return false; }

    std::size_t invCount = 0;
    if (!(in >> key >> invCount) || key != "INVENTORY" || invCount > 100) { error = "存档 INVENTORY 字段损坏。"; return false; }
    std::vector<std::pair<int,int>> inventory;
    for (std::size_t i = 0; i < invCount; ++i) {
        int id, qty;
        if (!(in >> id >> qty) || qty <= 0) { error = "存档物品数据损坏。"; return false; }
        inventory.push_back({id, qty});
    }

    std::size_t equipCount = 0;
    if (!(in >> key >> equipCount) || key != "EQUIPMENT" || equipCount > 2) { error = "存档 EQUIPMENT 字段损坏。"; return false; }
    std::vector<std::pair<EquipmentSlot,int>> equipment;
    for (std::size_t i = 0; i < equipCount; ++i) {
        int slot, id;
        if (!(in >> slot >> id) || slot < 1 || slot > 2) { error = "存档装备数据损坏。"; return false; }
        equipment.push_back({static_cast<EquipmentSlot>(slot), id});
    }

    std::size_t defeatedCount = 0;
    if (!(in >> key >> defeatedCount) || key != "DEFEATED" || defeatedCount > 1000) { error = "存档 DEFEATED 字段损坏。"; return false; }
    std::set<int> defeated;
    for (std::size_t i = 0; i < defeatedCount; ++i) {
        int id;
        if (!(in >> id)) { error = "存档敌人状态损坏。"; return false; }
        defeated.insert(id);
    }
    if (!(in >> key) || key != "END") { error = "存档缺少结束标记。"; return false; }

    try {
        Player loaded(name);
        loaded.setMaxHp(maxHp);
        loaded.setAttack(attack);
        loaded.setDefense(defense);
        loaded.setAgility(agility);
        loaded.setHitRate(hitRate);
        loaded.setDodgeRate(dodgeRate);
        loaded.setLevel(level);
        loaded.setExp(exp);
        loaded.setMora(mora);
        loaded.setTP(tp);
        loaded.setEnergy(energy);
        loaded.setCurrentHp(hp);
        loaded.inventory().clear();
        loaded.equipment().clear();

        for (const auto& [id, qty] : inventory) {
            if (!loaded.inventory().addItem(GameFactory::createItem(id), qty)) {
                error = "恢复背包时失败。";
                return false;
            }
        }
        for (const auto& [slot, id] : equipment) {
            (void)slot;
            Item item = GameFactory::createItem(id);
            if (!loaded.inventory().addItem(item, 1) || !loaded.equipment().equip(loaded.inventory(), id)) {
                error = "恢复装备时失败。";
                return false;
            }
        }

        player = std::move(loaded);
        quest.setStage(static_cast<QuestStage>(questStage));
        quest.setSealsBroken(seals);
        defeatedEnemies = std::move(defeated);
    } catch (...) {
        error = "存档中存在未知物品或非法数据。";
        return false;
    }

    return true;
}
