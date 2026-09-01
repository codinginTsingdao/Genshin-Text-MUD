#include "CombatSystem.h"
#include "BossEnemy.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

CombatSystem::CombatSystem() : m_rng(std::random_device{}()) {}

bool CombatSystem::roll(double probability) {
    probability = std::clamp(probability, 0.0, 1.0);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(m_rng) < probability;
}

bool CombatSystem::attackHits(const Character& attacker, const Character& defender) {
    return roll(attacker.getHitRate()) && !roll(defender.getDodgeRate());
}

int CombatSystem::calculateDamage(const Character& attacker, const Character& defender) {
    std::uniform_int_distribution<int> jitter(-2, 3);
    return std::max(1, attacker.effectiveAttack() - defender.effectiveDefense() / 2 + jitter(m_rng));
}

void CombatSystem::playerAttack(Player& player, Enemy& enemy, std::ostream& out) {
    if (!attackHits(player, enemy)) {
        out << "你的攻击落空了。\n";
        return;
    }

    int damage = calculateDamage(player, enemy);
    if (auto* boss = dynamic_cast<BossEnemy*>(&enemy); boss && boss->shieldActive()) {
        int dealt = boss->damageShield(damage);
        out << "你击中风之护盾，削减 " << dealt << " 点护盾。"
            << " [护盾 " << boss->getShield() << "/" << boss->getMaxShield() << "]\n";
        if (!boss->shieldActive()) {
            boss->enterPhaseTwo();
            out << "\n>>> 风之护盾破碎！风魔龙降落在平台上，战斗进入第二阶段。\n"
                << ">>> 它的攻击变得更加猛烈，并会周期性释放风息。\n\n";
        }
        return;
    }

    enemy.takeDamage(damage);
    out << "你对「" << enemy.getName() << "」造成 " << damage << " 点伤害。"
        << " [HP " << enemy.getCurrentHp() << "/" << enemy.getMaxHp() << "]\n";
}

void CombatSystem::enemyAttack(Enemy& enemy, Player& player, int turn, std::ostream& out) {
    if (!enemy.isAlive()) return;
    if (!attackHits(enemy, player)) {
        out << "「" << enemy.getName() << "」的攻击没有命中。\n";
        return;
    }

    int damage = calculateDamage(enemy, player);
    if (auto* boss = dynamic_cast<BossEnemy*>(&enemy);
        boss && boss->getPhase() == 2 && turn % 3 == 0) {
        damage = damage * 3 / 2 + 4;
        out << "风魔龙仰头蓄力——【风息】席卷整个平台！\n";
    }

    player.takeDamage(damage);
    out << "你受到 " << damage << " 点伤害。"
        << " [HP " << player.getCurrentHp() << "/" << player.getMaxHp() << "]\n";
}

bool CombatSystem::useItemTurn(Player& player, std::istream& in, std::ostream& out) {
    std::vector<int> consumableIds;

    out << "可用消耗品：\n";

    for (const auto& stack : player.inventory().items()) {
        if (stack.item.type != ItemType::Consumable) continue;

        consumableIds.push_back(stack.item.id);
        const std::size_t index = consumableIds.size();

        out << "  " << index << ". " << stack.item.name
            << " x" << stack.quantity << "（";

        bool hasEffect = false;

        if (stack.item.healAmount > 0) {
            out << "HP +" << stack.item.healAmount;
            hasEffect = true;
        }

        if (stack.item.restoreTP > 0) {
            if (hasEffect) out << "，";
            out << "TP +" << stack.item.restoreTP;
            hasEffect = true;
        }

        if (stack.item.restoreEnergy > 0) {
            if (hasEffect) out << "，";
            out << "元素充能 +" << stack.item.restoreEnergy;
            hasEffect = true;
        }

        if (!hasEffect) out << "无恢复效果";

        out << "）\n";
    }

    if (consumableIds.empty()) {
        out << "背包里没有可使用的恢复物品。\n";
        return false;
    }

    out << "输入物品序号（0 取消）：";

    std::string line;
    std::getline(in, line);

    std::istringstream parser(line);
    int choice = 0;

    if (!(parser >> choice)) {
        out << "请输入正确的物品序号。\n";
        return false;
    }

    if (choice == 0) {
        return false;
    }

    if (choice < 1 || choice > static_cast<int>(consumableIds.size())) {
        out << "无效的物品序号。\n";
        return false;
    }

    const int itemId = consumableIds[choice - 1];

    std::string message;
    if (!player.useConsumable(itemId, message)) {
        out << message << "\n";
        return false;
    }

    out << message << "\n";
    return true;
}

CombatResult CombatSystem::fight(Player& player, Enemy& enemy, std::istream& in, std::ostream& out) {
    out << "\n========== 战斗开始：" << enemy.getName() << " ==========\n";
    if (auto* boss = dynamic_cast<BossEnemy*>(&enemy)) {
        out << "第一阶段目标：击破风之护盾 [" << boss->getShield() << "/" << boss->getMaxShield() << "]\n";
    }

    const bool enemyFirst = enemy.getAgility() > player.getAgility();
    int turn = 1;

    while (player.isAlive() && enemy.isAlive()) {
        out << "\n--- 第 " << turn << " 回合 ---\n";

        if (enemyFirst) {
            enemyAttack(enemy, player, turn, out);
            if (!player.isAlive()) return CombatResult::Defeat;
        }

        bool actionDone = false;
        while (!actionDone) {
            out << "选择行动：1.普通攻击  2.使用物品  3.逃跑\n> ";
            std::string choice;
            if (!std::getline(in, choice)) return CombatResult::Escaped;

            if (choice == "1" || choice == "attack" || choice == "攻击") {
                playerAttack(player, enemy, out);
                actionDone = true;
            } else if (choice == "2" || choice == "item" || choice == "物品") {
                actionDone = useItemTurn(player, in, out);
            } else if (choice == "3" || choice == "escape" || choice == "逃跑") {
                if (enemy.isBoss()) {
                    out << "风墙封锁了天空战场，你无法逃离最终决战。\n";
                    continue;
                }
                if (roll(0.55)) {
                    out << "你成功脱离了战斗。\n";
                    return CombatResult::Escaped;
                }
                out << "逃跑失败！\n";
                actionDone = true;
            } else {
                out << "无法识别这个战斗指令。\n";
            }
        }

        if (!enemy.isAlive()) return CombatResult::Victory;

        if (!enemyFirst) {
            enemyAttack(enemy, player, turn, out);
            if (!player.isAlive()) return CombatResult::Defeat;
        }
        ++turn;
    }

    return enemy.isAlive() ? CombatResult::Defeat : CombatResult::Victory;
}
