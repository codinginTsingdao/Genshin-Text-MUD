#include "GameFactory.h"
#include "BossEnemy.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace {
std::string lowerAscii(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return c < 128 ? static_cast<char>(std::tolower(c)) : static_cast<char>(c);
    });
    return s;
}
}

Item GameFactory::createItem(int id) {
    switch (id) {
        case 1: return {1, "无锋剑", ItemType::Weapon, EquipmentSlot::Weapon, "旅行者最初使用的单手剑。", false, 0, {8, 0}};
        case 2: return {2, "西风剑", ItemType::Weapon, EquipmentSlot::Weapon, "西风骑士团制式单手剑。", false, 0, {15, 0}};
        case 3: return {3, "旅行者外衣", ItemType::Armor, EquipmentSlot::Armor, "轻便的旅行装。", false, 0, {0, 3}};
        case 4: return {4, "骑士护甲", ItemType::Armor, EquipmentSlot::Armor, "琴赠予的简易骑士护甲。", false, 0, {0, 8}};
        case 5: return {5, "苹果", ItemType::Consumable, EquipmentSlot::None, "恢复少量生命。", true, 25, {0, 0}};
        case 6: return {6, "甜甜花酿鸡", ItemType::Consumable, EquipmentSlot::None, "恢复中量生命。", true, 55, {0, 0}};
        case 7: return {7, "治疗药剂", ItemType::Consumable, EquipmentSlot::None, "恢复大量生命。", true, 80, {0, 0}};
        case 8: return {8, "风龙的眼泪", ItemType::Quest, EquipmentSlot::None, "蕴含异常风元素的泪滴。", true, 0, {0, 0}};
        case 9: return {9, "净化结晶", ItemType::Quest, EquipmentSlot::None, "解除风之封印所需的净化材料。", true, 0, {0, 0}};
        case 10: return {10, "风之钥匙", ItemType::Quest, EquipmentSlot::None, "温迪交给你的古老钥匙。", true, 0, {0, 0}};
        case 11: return {11, "北地烟熏鸡", ItemType::Consumable, EquipmentSlot::None, "恢复40点体力。", true, 0, {0, 0}, 40, 0};
        default: throw std::invalid_argument("unknown item id");
    }
}

int GameFactory::itemIdFromName(const std::string& raw) {
    std::string s = lowerAscii(raw);
    if (s == "无锋剑" || s == "dullblade" || s == "dull blade" || s == "sword") return 1;
    if (s == "西风剑" || s == "favonius" || s == "favonius sword") return 2;
    if (s == "旅行者外衣" || s == "traveler coat" || s == "coat") return 3;
    if (s == "骑士护甲" || s == "knight armor" || s == "armor") return 4;
    if (s == "苹果" || s == "apple") return 5;
    if (s == "甜甜花酿鸡" || s == "chicken" || s == "sweet madame") return 6;
    if (s == "治疗药剂" || s == "potion") return 7;
    if (s == "风龙的眼泪" || s == "tear") return 8;
    if (s == "净化结晶" || s == "crystal") return 9;
    if (s == "风之钥匙" || s == "key") return 10;
    if (s == "北地烟熏鸡" || s == "smoked chicken" || s == "northern smoked chicken") return 11;
    return 0;
}

std::unique_ptr<Enemy> GameFactory::createEnemy(int id) {
    switch (id) {
        // 低语森林：教学怪
    case 1001: return std::make_unique<Enemy>(id, "丘丘人", 55, 13, 5, 8, 0.82, 0.05, 45, 25, std::vector<std::pair<int, int>>{{5, 1}});
        // 风起地：风史莱姆
    case 1101: return std::make_unique<Enemy>(id, "风史莱姆", 50, 14, 4, 15, 0.84, 0.10, 50, 30, std::vector<std::pair<int, int>>{{9, 1}});
        // 风起地：远程型敌人
    case 1102: return std::make_unique<Enemy>(id, "丘丘人射手", 65, 16, 5, 12, 0.88, 0.08, 60, 35, std::vector<std::pair<int, int>>{{9, 1}, { 5, 1 }});
        // 风起地：中期精英
    case 1103: return std::make_unique<Enemy>(id, "深渊法师", 85, 19, 8, 10, 0.87, 0.08, 85, 55, std::vector<std::pair<int, int>>{{9, 1}, { 11, 1 }});
        // 风龙废墟：高血高防
    case 1201: return std::make_unique<Enemy>(id, "丘丘暴徒", 110, 22, 12, 8, 0.84, 0.05, 100, 65, std::vector<std::pair<int, int>>{{6, 1}});
        // 风龙废墟：Boss 前精英怪
    case 1202: return std::make_unique<Enemy>(id, "遗迹守卫", 150, 26, 16, 7, 0.82, 0.03, 150, 100, std::vector<std::pair<int, int>>{{7, 1}, { 11, 1 }});
        // 最终 Boss
    case 2001: return std::make_unique<BossEnemy>(id, "风魔龙", 220, 90, 25, 10, 14, 0.90, 0.08, 300, 300);
    default: return nullptr;
    }
}


bool GameFactory::enemyMatches(int id, const std::string& raw) {
    std::string q = lowerAscii(raw);
    auto enemy = createEnemy(id);
    if (!enemy) return false;
    // 没输入敌人名字时，直接匹配当前敌人
    if (q.empty()) return true;
    // 中文全名直接匹配
    if (lowerAscii(enemy->getName()) == q) return true;
    if (id == 1001 && (q == "hilichurl" || q == "丘丘人")) return true;
    if (id == 1101 && (q == "slime" || q == "wind slime" || q == "风史莱姆")) return true;
    if (id == 1102 && (q == "archer" || q == "hilichurl archer" || q == "丘丘人射手")) return true;
    if (id == 1103 && (q == "mage" || q == "abyss mage" || q == "深渊法师")) return true;
    if (id == 1201 && (q == "brute" || q == "hilichurl brute" || q == "丘丘暴徒")) return true;
    if (id == 1202 && (q == "ruin guard" || q == "guard" || q == "遗迹守卫")) return true;
    if (id == 2001 && (q == "dvalin" || q == "stormterror" || q == "风魔龙")) return true;
    return false;
}

World GameFactory::createWorld() {
    World world;

    Room r1(1, "低语森林", "鹰翔海滩", "海风扑面而来。你从陌生的沙地醒来，远处的林间小路通向蒙德方向。");
    Room r2(2, "低语森林", "森林小径", "风吹过树叶。一位红衣侦察骑士正警惕地观察四周。");
    Room r3(3, "低语森林", "丘丘人营地", "木制拒马和篝火挡住道路，几只丘丘人盘踞在这里。");
    Room r4(4, "低语森林", "蒙德城门", "巨大的城墙与风车映入眼帘，守卫示意你可以入城。");

    Room r5(5, "蒙德城", "中央广场", "自由之都的广场十分热闹，风神像在阳光下投下长长的影子。");
    Room r6(6, "蒙德城", "西风骑士团", "庄严的骑士团大厅中，代理团长琴正在处理风龙危机。");
    Room r7(7, "蒙德城", "铁匠铺", "炉火熊熊，铁匠瓦格纳正在敲打一柄长剑。");
    Room r8(8, "蒙德城", "北城门", "城门之外的道路通向风起地，空气中的风元素比城内更强。");

    Room r9(9, "风起地遗迹", "风起地", "巨树的枝叶遮蔽天空。温迪站在树下，似乎早已知道你会来。");
    Room r10(10, "风起地遗迹", "大树下", "紊乱的风元素在树根附近聚集，一只风史莱姆守着发光的结晶。");
    Room r11(11, "风起地遗迹", "古代遗迹入口", "残破石柱之间传来弓弦声，前方刻着第一纪元的古老文字。");
    Room r12(12, "风起地遗迹", "封印祭坛", "三道风之封印在祭坛上盘旋，深渊力量正试图维持最后一道屏障。");

    Room r13(13, "风龙废墟", "外围废墟", "破碎的石桥通向废墟深处，狂风中仍有魔物巡逻。");
    Room r14(14, "风龙废墟", "风墙", "巨大的风墙横在道路中央，残余的深渊力量令它时强时弱。");
    Room r15(15, "风龙废墟", "高塔", "高塔之巅直面天空。风魔龙的咆哮已经近在耳边。");
    Room r16(16, "风龙废墟", "天空战场", "云层翻涌，特瓦林展开双翼。净化它的最后一战开始了。");

    r1.addExit(Direction::North, 2);
    r2.addExit(Direction::South, 1); r2.addExit(Direction::East, 3);
    r3.addExit(Direction::West, 2); r3.addExit(Direction::North, 4);
    r4.addExit(Direction::South, 3); r4.addExit(Direction::North, 5);
    r5.addExit(Direction::South, 4); r5.addExit(Direction::East, 6); r5.addExit(Direction::West, 7); r5.addExit(Direction::North, 8);
    r6.addExit(Direction::West, 5); r7.addExit(Direction::East, 5);
    r8.addExit(Direction::South, 5); r8.addExit(Direction::North, 9);
    r9.addExit(Direction::South, 8); r9.addExit(Direction::North, 10);
    r10.addExit(Direction::South, 9); r10.addExit(Direction::East, 11);
    r11.addExit(Direction::West, 10); r11.addExit(Direction::North, 12);
    r12.addExit(Direction::South, 11); r12.addExit(Direction::North, 13);
    r13.addExit(Direction::South, 12); r13.addExit(Direction::North, 14);
    r14.addExit(Direction::South, 13); r14.addExit(Direction::North, 15);
    r15.addExit(Direction::South, 14); r15.addExit(Direction::North, 16);
    r16.addExit(Direction::South, 15);

    r2.addNPC(NPC("安柏", "西风骑士团侦察骑士"));
    r5.addNPC(NPC("温迪", "吟游诗人"));
    r6.addNPC(NPC("琴", "西风骑士团代理团长"));
    r7.addNPC(NPC("瓦格纳", "铁匠"));
    r9.addNPC(NPC("温迪", "吟游诗人"));
    r3.addEnemy(1001); r10.addEnemy(1101); r11.addEnemy(1102); r12.addEnemy(1103);
    r13.addEnemy(1201); r14.addEnemy(1202); r16.addEnemy(2001);

    for (const Room* r : {&r1,&r2,&r3,&r4,&r5,&r6,&r7,&r8,&r9,&r10,&r11,&r12,&r13,&r14,&r15,&r16}) world.addRoom(*r);
    return world;
}
