#include "Game.h"
#include "GameFactory.h"
#include "SaveManager.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>

#ifdef _WIN32
#include <conio.h>
#endif

namespace {
const char* CYAN = "\033[36m";
const char* BLUE = "\033[37m";
const char* MAGENTA = "\033[35m";
const char* GREEN = "\033[32m";
const char* YELLOW = "\033[33m";
const char* RED = "\033[31m";
const char* RESET = "\033[0m";

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

// Windows 下完全沿用原项目的“按任意键继续”体验；其他平台用于编译测试。
void waitAnyKey() {
#ifdef _WIN32
    (void)_getch();
#else
    std::string line;
    std::getline(std::cin, line);
#endif
}

void pauseWithOriginalText() {
    std::cout << "\n按任意键继续...";
    waitAnyKey();
}

std::string trim(const std::string& s) {
    const std::size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const std::size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

std::string lowerAscii(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
        return c < 128 ? static_cast<char>(std::tolower(c)) : static_cast<char>(c);
    });
    return s;
}

std::string directionName(Direction d) {
    switch (d) {
        case Direction::North: return "北(north)";
        case Direction::South: return "南(south)";
        case Direction::East: return "东(east)";
        case Direction::West: return "西(west)";
        default: return "?";
    }
}

bool ownsItem(const Player& player, int id) {
    if (player.inventory().quantityOf(id) > 0) return true;
    for (const auto& entry : player.equipment().equippedItems()) {
        if (entry.second.id == id) return true;
    }
    return false;
}
}

Game::Game() : m_player("旅行者"), m_world(GameFactory::createWorld()) {}

void Game::showTitle()
{
    std::cout << "\033[36m==================================================================================================================================================";
    std::cout << R"(
  捕　　　　　捕　捕　　　　风风风风风风风风　　　　　　　的　　　的　　　　　　异异异异异异异异异　　　　　　乡　　　　　　    　　人
　捕　　捕捕捕捕捕捕捕　　　风　　　　　　风　　　　　　的　　　　的　　　　　　异　　　　　　　异　　　　　乡　　　　　　　　    　人
捕捕捕　　　　捕　　　　　　风　　　　风　风　　　　的的的的的　　的的的的　　　异异异异异异异异异　　　　乡　　　　乡　　　　　　　人
　捕　　捕捕捕捕捕捕捕　　　风　风　　风　风　　　　的　　　的　的　　　的　　　异　　　　　　　　异　　乡乡乡乡乡乡　　　　　　　　人
　捕　　捕　　捕　　捕　　　风　　风　风　风　　　　的　　　的　的　　　的　　　异异异异异异异异异异　　　　　乡　　　　　　　　　人　人
　捕捕　捕捕捕捕捕捕捕　　　风　　　风　　风　　　　的　　　的　　的　　的　　　　异　　　　　异　　　　　　乡　　　　　　　　　　人　人
捕捕　　捕　　捕　　捕　　　风　　　风　　风　　　　的的的的的　　　的　的　　　　异　　　　　异　　　　　乡　　　　　乡　　　　人　　　人
　捕　　捕捕捕捕捕捕捕　　　风　　风　风　风　风　　的　　　的　　　的　的　　异异异异异异异异异异异　　乡乡乡乡乡乡乡　　　　　人　　　人
　捕　　捕　　捕　　捕　　　风　风　　风　风　风　　的　　　的　　　　　的　　　　异　　　　　异　　　　　　　　　乡　　　　　人　　　　　人
　捕　　捕　　捕　　捕　　风　　　　　风　风　风　　的　　　的　　　　　的　　　　异　　　　　异　　　　　　　乡乡　　　　　人　　　　　　　人
捕捕　　捕　　捕　捕捕　　风　　　　　　　　风　　　的的的的的　　　的的的　　　异　　　　　　异　　　　乡乡乡　　　　　　人　　　　　　　　　人
)" << std::endl;
    std::cout << "==================================================================================================================================================\n\n\n" << std::endl;
    std::cout << "按任意键继续...\033[0m" << std::endl;
    waitAnyKey();
    clearScreen();
}

void Game::inputPlayerName()
{
    clearScreen();
    std::string tempName;
    std::cout << "\033[37m=========================================================================\n";
    std::cout << "请输入旅行者的名字：";
    std::getline(std::cin, tempName);
    tempName = trim(tempName);
    if (tempName.empty()) tempName = "旅行者";
    m_player.setName(tempName);
    std::cout << "=========================================================================";
    std::cout << "\n 您好！尊敬的" << m_player.getName() << ",欢迎游玩这款游戏！！\n\n按任意键继续...\033[0m";
    waitAnyKey();
    clearScreen();
}

void Game::showStory()
{
    clearScreen();
    std::cout << "\033[37m================================================================================================================================" << std::endl;
    std::cout << "一觉醒来，你发现睁开眼却发现自己看到的不是熟悉的天花板，而是一处傍水的沙地，作为资深的原皮，你立刻明白了这里是鹰翔海滩,以及这" << std::endl;
    std::cout << "意味着什么——没错，你来到了提瓦特大陆，只可惜你并没有向导，不过，凭借着你对提瓦特大陆的了解，想必能够活出更加精彩的人生吧？不" << std::endl;
    std::cout << "论这到底是梦还是真正的穿越，总之，先决定你的第一个行动吧！！！" << std::endl;
    std::cout << "\033=================================================================================================================================" << std::endl;
    std::cout << "\n按任意键继续...\033[0m";
    waitAnyKey();
    clearScreen();
    std::cout << "\033[37m\n序章:听从风的指引\n\033[0m";
    waitAnyKey();
    clearScreen();
}
// ============================================================================

void Game::newGame() {
    m_player = Player("旅行者");
    m_world = GameFactory::createWorld();
    m_quest = QuestManager();
    m_defeatedEnemies.clear();
    m_currentRoom = 1;

    inputPlayerName();

    // 初始物品：使用现有 Inventory / Equipment 正式接入 Player。
    m_player.addMora(50);
    m_player.inventory().addItem(GameFactory::createItem(1));
    m_player.inventory().addItem(GameFactory::createItem(3));
    m_player.inventory().addItem(GameFactory::createItem(5), 3);
    m_player.inventory().addItem(GameFactory::createItem(6), 1);
    m_player.equipment().equip(m_player.inventory(), 1);
    m_player.equipment().equip(m_player.inventory(), 3);

    showStory();
    m_inGame = true;
}

bool Game::loadGame() {
    std::string error;
    if (!SaveManager::load(m_savePath, m_player, m_currentRoom, m_quest,
                           m_defeatedEnemies, error)) {
        std::cout << RED << "读取失败：" << error << RESET << "\n";
        return false;
    }
    m_world = GameFactory::createWorld();
    m_inGame = true;
    std::cout << GREEN << "存档读取成功。" << RESET << "\n";
    return true;
}

void Game::showMenu() {
    clearScreen();
    std::cout << "\033[36m=====主菜单=====\n";
    std::cout << "1.开始游戏\n";
    std::cout << "2.继续游戏 \n";
    std::cout << "3.结束游戏 \n";
    std::cout << "请输入选项:\033[0m";

    std::string choice;
    if (!std::getline(std::cin, choice)) {
        m_running = false;
        return;
    }
    choice = trim(choice);

    if (choice == "1") {
        std::cout << "\033[36m\n>>开始新游戏\n\033[0m";
        waitAnyKey();
        clearScreen();
        newGame();
        gameLoop();
    }
    else if (choice == "2") {
        std::cout << "\n>>读取存档，继续游戏\n";
        if (loadGame()) {
            waitAnyKey();
            gameLoop();
        } else {
            pauseWithOriginalText();
        }
    }
    else if (choice == "3") {
        std::cout << "\n>>退出游戏\n";
        m_running = false;
    }
    else {
        std::cout << "\n未识别到有效指令";
        pauseWithOriginalText();
    }
}

void Game::run() {
    showTitle();
    while (m_running) {
        showMenu();
    }
}

// 原仓库的 1~6 行动菜单保持不变，只把按钮背后的占位功能换成真实模块。
void Game::gameLoop()
{
    while (m_running && m_inGame)
    {
        clearScreen();
        const Room* room = m_world.getRoom(m_currentRoom);

        if (room && m_currentRoom == 1) {
            // 开场房间保持原来的显示文本。
            std::cout << "\033[36m===== 鹰翔海滩 =====\033[0m\n";
            std::cout << "海风扑面而来，海浪拍打着岸边的礁石。\n";
            std::cout << "你身处陌生的提瓦特大陆，接下来要做什么？\n\n";
        } else if (room) {
            // 后续房间沿用同一种标题/正文布局，不改变 UI 风格。
            std::cout << "\033[36m===== " << room->getName() << " =====\033[0m\n";
            std::cout << room->getDescription() << "\n";
            std::cout << "你接下来要做什么？\n\n";
        }

        std::cout << "1. 四处探索 \n";
        std::cout << "2. 查看角色状态 \n";
        std::cout << "3. 打开背包 \n";
        std::cout << "4. 保存游戏 \n";
        std::cout << "5. 返回主菜单 \n";
        std::cout << "6. 退出游戏 \n";
        std::cout << "请输入你的行动：";

        std::string op;
        if (!std::getline(std::cin, op)) {
            m_running = false;
            return;
        }
        op = trim(op);

        if (op == "1") {
            exploreLoop();
        }
        else if (op == "2") {
            std::cout << "\n【角色状态界面】";
            showStatus();
            pauseWithOriginalText();
        }
        else if (op == "3") {
            std::cout << "\n【背包界面】";
            showInventory();
            pauseWithOriginalText();
        }
        else if (op == "4") {
            saveGame();
            pauseWithOriginalText();
        }
        else if (op == "5") {
            m_inGame = false;
            return;
        }
        else if (op == "6") {
            m_inGame = false;
            m_running = false;
            return;
        }
        else {
            std::cout << "\n 无效输入！";
            pauseWithOriginalText();
        }
    }
}

void Game::exploreLoop() {
    m_exploring = true;
    clearScreen();
    showRoom();
    std::cout << CYAN << "\n输入 help 查看探索指令，输入 menu 或 返回 回到行动菜单。" << RESET << "\n";

    while (m_running && m_inGame && m_exploring) {
        std::cout << "\n" << CYAN << "> " << RESET;
        std::string input;
        if (!std::getline(std::cin, input)) {
            m_running = false;
            return;
        }
        Command command = m_parser.parse(input);
        executeCommand(command);
    }
}

void Game::executeCommand(const Command& command) {
    switch (command.type) {
        case CommandType::Help: showHelp(); break;
        case CommandType::Look: showRoom(); break;
        case CommandType::North: move(Direction::North); break;
        case CommandType::South: move(Direction::South); break;
        case CommandType::East: move(Direction::East); break;
        case CommandType::West: move(Direction::West); break;
        case CommandType::Talk: talk(command.argument); break;
        case CommandType::Attack: attack(command.argument); break;
        case CommandType::Inventory: showInventory(); break;
        case CommandType::Use: useItem(command.argument); break;
        case CommandType::Equip: equipItem(command.argument); break;
        case CommandType::Unequip: unequipItem(command.argument); break;
        case CommandType::Status: showStatus(); break;
        case CommandType::Save: saveGame(); break;
        case CommandType::Load:
            if (loadGame()) showRoom();
            break;
        case CommandType::NewGame:
            std::cout << YELLOW << "将重新开始当前游戏。" << RESET << "\n";
            newGame();
            showRoom();
            break;
        case CommandType::Menu:
            m_exploring = false;
            break;
        case CommandType::Quit:
            m_exploring = false;
            m_inGame = false;
            m_running = false;
            break;
        case CommandType::Unknown:
        default:
            std::cout << RED << "无法识别该指令。输入 help 查看命令列表。" << RESET << "\n";
            break;
    }
}

void Game::showHelp() const {
    std::cout << "\n";
    std::cout << "====================== 探索指令 ======================\n";

    std::cout << "\n【场景与移动】\n";
    std::cout << "  help / 帮助          查看全部指令\n";
    std::cout << "  look / 查看          查看当前场景\n";
    std::cout << "  north / 北           向北移动\n";
    std::cout << "  south / 南           向南移动\n";
    std::cout << "  east / 东            向东移动\n";
    std::cout << "  west / 西            向西移动\n";

    std::cout << "\n【交互与战斗】\n";
    std::cout << "  talk / 对话          与当前场景中的人物对话\n";
    std::cout << "  attack / 攻击        攻击当前场景中的敌人\n";

    std::cout << "\n【角色与物品】\n";
    std::cout << "  inventory / 背包     查看背包与当前装备\n";
    std::cout << "  use / 使用           使用背包中的物品\n";
    std::cout << "  equip / 装备         装备武器或护甲\n";
    std::cout << "  unequip / 卸下       卸下当前装备\n";
    std::cout << "  status / 状态        查看角色和任务状态\n";

    std::cout << "\n【系统】\n";
    std::cout << "  save / 保存          保存当前进度\n";
    std::cout << "  load / 读取          读取游戏存档\n";
    std::cout << "  menu / 返回          返回行动菜单\n";
    std::cout << "  quit / 退出          退出游戏\n";

    std::cout << "\n======================================================\n";
}
void Game::showRoom() const {
    const Room* room = m_world.getRoom(m_currentRoom);
    if (!room) return;

    std::cout << "\n" << CYAN << "===== " << room->getName() << " =====" << RESET << "\n";
    std::cout << room->getDescription() << "\n";

    if (!room->getNPCs().empty()) {
        std::cout << GREEN << "人物：" << RESET;
        for (std::size_t i = 0; i < room->getNPCs().size(); ++i) {
            if (i) std::cout << "、";
            std::cout << room->getNPCs()[i].getName();
        }
        std::cout << "\n";
    }

    bool hasEnemy = false;
    for (int spawnId : room->getEnemySpawnIds()) {
        if (m_defeatedEnemies.count(spawnId)) continue;
        auto enemy = GameFactory::createEnemy(spawnId);
        if (!enemy) continue;
        if (!hasEnemy) std::cout << RED << "敌人：" << RESET;
        else std::cout << "、";
        std::cout << enemy->getName();
        hasEnemy = true;
    }
    if (hasEnemy) std::cout << "\n";

    std::cout << "\n【可前往】\n";
    bool hasExit = false;
    for (Direction d : { Direction::North, Direction::South, Direction::East, Direction::West }) {
        int targetId = room->getExit(d);
        if (targetId == 0) continue;
        hasExit = true;
        const Room* targetRoom = m_world.getRoom(targetId);
        std::cout << "  " << directionName(d) << " -> ";
        if (targetRoom) {
            std::cout << targetRoom->getName();
            if (!targetRoom->getNPCs().empty()) {
                std::cout << "  [人物：";
                for (std::size_t i = 0; i < targetRoom->getNPCs().size(); ++i) {
                    if (i > 0) std::cout << "、";
                    std::cout << targetRoom->getNPCs()[i].getName();
                }
                std::cout << "]";
            }
            bool firstEnemy = true;
            for (int spawnId : targetRoom->getEnemySpawnIds()) {
                if (m_defeatedEnemies.count(spawnId)) continue;
                auto enemy = GameFactory::createEnemy(spawnId);
                if (!enemy) continue;
                if (firstEnemy) { std::cout << "  [敌人："; firstEnemy = false; }
                else std::cout << "、";
                std::cout << enemy->getName();
            }
            if (!firstEnemy) std::cout << "]";
        }
        std::cout << "\n";
    }
    if (!hasExit) std::cout << "  无\n";


    std::cout << YELLOW << "主线：" << m_quest.stageName() << " —— "
              << m_quest.currentObjective() << RESET << "\n";
}

void Game::move(Direction direction) {
    std::string reason;
    if (!m_world.canMove(m_currentRoom, direction, m_quest.getStage(), m_defeatedEnemies, reason)) {
        std::cout << YELLOW << reason << RESET << "\n";
        return;
    }

    m_currentRoom = m_world.moveTarget(m_currentRoom, direction);

    if (m_currentRoom == 5 && m_quest.getStage() == QuestStage::HilichurlCleared) {
        m_quest.advanceTo(QuestStage::EnteredMondstadt);
        std::cout << GREEN << "【主线推进】你第一次进入蒙德城。风龙危机正在城中蔓延。" << RESET << "\n";
    }
    if (m_currentRoom == 13 && m_quest.getStage() == QuestStage::SealsBroken) {
        m_quest.advanceTo(QuestStage::StormterrorLair);
        std::cout << GREEN << "【主线推进】风龙废墟已经开放。" << RESET << "\n";
    }

    showRoom();
}

bool Game::roomHasNpc(const std::string& query, std::string& canonicalName) const {
    const Room* room = m_world.getRoom(m_currentRoom);
    if (!room) return false;

    const std::string q = lowerAscii(trim(query));
    for (const NPC& npc : room->getNPCs()) {
        if (q.empty() || lowerAscii(npc.getName()) == q) {
            canonicalName = npc.getName();
            return true;
        }
        if ((npc.getName() == "安柏" && q == "amber") ||
            (npc.getName() == "琴" && (q == "jean" || q == "qin")) ||
            (npc.getName() == "温迪" && q == "venti") ||
            (npc.getName() == "瓦格纳" && q == "wagner")) {
            canonicalName = npc.getName();
            return true;
        }
    }
    return false;
}

void Game::talk(const std::string& npcName) {
    std::string npc;
    if (!roomHasNpc(npcName, npc)) {
        std::cout << "这里没有你要找的人。可以先输入 look 查看当前场景。\n";
        return;
    }

    if (npc == "安柏") {
        if (m_quest.getStage() == QuestStage::Awakening) {
            std::cout << GREEN << "安柏：‘你好！我是西风骑士团侦察骑士安柏。前面的丘丘人营地挡住了去蒙德城的路，能帮我清理掉它们吗？’" << RESET << "\n";
            m_quest.advanceTo(QuestStage::MetAmber);
            std::cout << "【主线更新】" << m_quest.currentObjective() << "\n";
        } else if (!m_quest.atLeast(QuestStage::HilichurlCleared)) {
            std::cout << "安柏：‘营地就在东边，小心丘丘人！’\n";
        } else {
            std::cout << "安柏：‘干得漂亮！从北边穿过城门就能进入蒙德。’\n";
        }
        return;
    }

    if (npc == "琴") {
        if (!m_quest.atLeast(QuestStage::EnteredMondstadt)) {
            std::cout << "琴：‘欢迎来到西风骑士团。’\n";
        } else if (m_quest.getStage() == QuestStage::EnteredMondstadt) {
            std::cout << GREEN << "琴：‘风魔龙特瓦林受到了深渊力量影响。我们需要净化它，而不是杀死它。温迪或许知道更多。’" << RESET << "\n";
            m_quest.advanceTo(QuestStage::CityBriefing);
            if (!ownsItem(m_player, 4)) {
                m_player.inventory().addItem(GameFactory::createItem(4));
                std::cout << "琴交给你【骑士护甲】。可以输入 equip 骑士护甲 装备。\n";
            }
            std::cout << "【主线更新】" << m_quest.currentObjective() << "\n";
        } else {
            std::cout << "琴：‘愿风护佑你的行动。’\n";
        }
        return;
    }

    if (npc == "温迪") {
        if (m_currentRoom == 5) {
            if (m_quest.getStage() == QuestStage::CityBriefing) {
                std::cout << GREEN << "温迪：‘特瓦林的痛苦来自深渊污染。这滴风龙的眼泪会指引你去风起地，我们在那里寻找净化材料。’" << RESET << "\n";
                if (m_player.inventory().quantityOf(8) == 0) m_player.inventory().addItem(GameFactory::createItem(8));
                m_quest.advanceTo(QuestStage::DragonTearAccepted);
                std::cout << "你获得任务物品【风龙的眼泪】。\n";
                std::cout << "【主线更新】" << m_quest.currentObjective() << "\n";
            } else if (!m_quest.atLeast(QuestStage::CityBriefing)) {
                std::cout << "温迪：‘现在还不是讲故事的时候。先去骑士团看看吧。’\n";
            } else {
                std::cout << "温迪：‘风会告诉你下一步该往哪里走。’\n";
            }
        } else if (m_currentRoom == 9) {
            if (m_quest.getStage() == QuestStage::DragonTearAccepted) {
                std::cout << GREEN << "温迪：‘遗迹里的三处魔物分别维持着一道封印。带上这把风之钥匙，取得三枚净化结晶。’" << RESET << "\n";
                if (m_player.inventory().quantityOf(10) == 0) m_player.inventory().addItem(GameFactory::createItem(10));
                m_quest.advanceTo(QuestStage::SearchingCrystals);
                std::cout << "你获得任务物品【风之钥匙】。\n";
                std::cout << "【主线更新】" << m_quest.currentObjective() << "\n";
            } else if (m_quest.getStage() == QuestStage::SearchingCrystals) {
                std::cout << "温迪：‘三道封印已经解除 " << m_quest.getSealsBroken() << "/3，道路就在前方。’\n";
            } else if (m_quest.atLeast(QuestStage::SealsBroken)) {
                std::cout << "温迪：‘去吧。让特瓦林重新听见自由的风。’\n";
            } else {
                std::cout << "温迪：‘诶？你是不是来得有点早？’\n";
            }
        }
        return;
    }

    if (npc == "瓦格纳") {
        std::cout << "瓦格纳：‘先把风龙危机解决了吧。好武器可不会自己挥起来。’\n";
        return;
    }
}

void Game::attack(const std::string& enemyName) {
    const Room* room = m_world.getRoom(m_currentRoom);
    if (!room) return;

    int targetId = 0;
    int aliveCount = 0;
    for (int spawnId : room->getEnemySpawnIds()) {
        if (m_defeatedEnemies.count(spawnId)) continue;
        ++aliveCount;
        if (GameFactory::enemyMatches(spawnId, enemyName)) targetId = spawnId;
    }

    if (aliveCount == 0) {
        std::cout << "这里已经没有可以攻击的敌人。\n";
        return;
    }
    if (targetId == 0) {
        std::cout << "没有找到这个敌人。可以先输入 look 查看。\n";
        return;
    }

    auto enemy = GameFactory::createEnemy(targetId);
    if (!enemy) return;

    CombatResult result = m_combat.fight(m_player, *enemy, std::cin, std::cout);
    if (result == CombatResult::Victory) {
        handleEnemyVictory(*enemy);
    } else if (result == CombatResult::Defeat) {
        std::cout << RED << "你倒在了战斗中。可以从主菜单读取之前的存档，或开始新游戏。" << RESET << "\n";
        m_exploring = false;
        m_inGame = false;
    }
}

void Game::handleEnemyVictory(Enemy& enemy) {
    m_defeatedEnemies.insert(enemy.getSpawnId());
    m_player.gainExp(enemy.getExpReward());
    m_player.addMora(enemy.getMoraReward());

    std::cout << GREEN << "战斗胜利！获得 " << enemy.getExpReward() << " EXP、"
              << enemy.getMoraReward() << " 摩拉。" << RESET << "\n";

    for (const auto& [itemId, quantity] : enemy.getDrops()) {
        Item item = GameFactory::createItem(itemId);
        if (m_player.inventory().addItem(item, quantity)) {
            std::cout << "获得【" << item.name << "】x" << quantity << "。\n";
        } else {
            std::cout << YELLOW << "背包已满，未能拾取【" << item.name << "】。" << RESET << "\n";
        }
    }

    if (enemy.getSpawnId() == 1001 && !m_quest.atLeast(QuestStage::HilichurlCleared)) {
        m_quest.advanceTo(QuestStage::HilichurlCleared);
        std::cout << GREEN << "【主线推进】丘丘人营地已清理，通往蒙德城的道路开放。" << RESET << "\n";
    }

    if ((enemy.getSpawnId() == 1101 || enemy.getSpawnId() == 1102 || enemy.getSpawnId() == 1103)
        && m_quest.getStage() == QuestStage::SearchingCrystals) {
        if (m_quest.breakSeal()) {
            std::cout << GREEN << "【风之封印解除】当前进度 " << m_quest.getSealsBroken() << "/3。" << RESET << "\n";
            if (m_quest.getSealsBroken() == 3) {
                std::cout << GREEN << "三道封印全部解除！风龙废墟入口已经开放。" << RESET << "\n";
            }
        }
    }

    if (enemy.getSpawnId() == 2001) {
        m_quest.advanceTo(QuestStage::DragonPurified);
        showEnding();
        m_exploring = false;
        m_inGame = false;
    }
}

void Game::showInventory() const {
    std::cout << "\n========== 背包 ==========\n";

    const auto& items = m_player.inventory().items();

    if (items.empty()) {
        std::cout << "背包为空。\n";
    }
    else {
        // 这里显示的是“背包序号”1、2、3……，
        // 不再显示 Item 内部使用的 item.id。
        for (std::size_t i = 0; i < items.size(); ++i) {
            const auto& stack = items[i];

            std::cout << "[" << (i + 1) << "] "
                      << stack.item.name
                      << " x" << stack.quantity
                      << " - " << stack.item.description
                      << "\n";
        }
    }

    const Item* weapon =
        m_player.equipment().getEquippedItem(EquipmentSlot::Weapon);

    const Item* armor =
        m_player.equipment().getEquippedItem(EquipmentSlot::Armor);

    std::cout << "\n---------- 当前装备 ----------\n";
    std::cout << "武器：" << (weapon ? weapon->name : "无") << "\n";
    std::cout << "护甲：" << (armor ? armor->name : "无") << "\n";

    std::cout << "\n容量："
              << m_player.inventory().getUsedSlots()
              << "/"
              << m_player.inventory().getCapacity()
              << "\n";

    std::cout << "==============================\n";
}

void Game::useItem(const std::string& itemName) {
    if (itemName.empty()) {
        std::cout << "用法：use <item>，例如 use 苹果。\n";
        return;
    }
    int id = GameFactory::itemIdFromName(itemName);
    if (id == 0) {
        std::cout << "没有识别到这个物品名称。\n";
        return;
    }
    std::string message;
    if (!m_player.useConsumable(id, message)) {
        std::cout << YELLOW << message << RESET << "\n";
        return;
    }
    std::cout << GREEN << message << RESET << "\n";
}

void Game::equipItem(const std::string& itemName) {
    if (itemName.empty()) {
        std::cout << "用法：equip <item>，例如 equip 骑士护甲。\n";
        return;
    }
    int id = GameFactory::itemIdFromName(itemName);
    if (id == 0) {
        std::cout << "没有识别到这个装备。\n";
        return;
    }
    if (!m_player.equipment().equip(m_player.inventory(), id)) {
        std::cout << "装备失败：背包中没有该装备、物品不可装备，或背包无法完成替换。\n";
        return;
    }
    std::cout << GREEN << "装备成功。当前攻击 " << m_player.effectiveAttack()
              << "，防御 " << m_player.effectiveDefense() << "。" << RESET << "\n";
}

void Game::unequipItem(const std::string& slotName) {
    std::string s = lowerAscii(slotName);
    EquipmentSlot slot = EquipmentSlot::None;
    if (s == "weapon" || s == "武器") slot = EquipmentSlot::Weapon;
    if (s == "armor" || s == "护甲" || s == "防具") slot = EquipmentSlot::Armor;
    if (slot == EquipmentSlot::None) {
        std::cout << "用法：unequip weapon 或 unequip armor。\n";
        return;
    }
    if (!m_player.equipment().unequip(m_player.inventory(), slot)) {
        std::cout << "卸下失败：该部位没有装备，或背包已满。\n";
        return;
    }
    std::cout << GREEN << "装备已卸下。" << RESET << "\n";
}

void Game::showStatus() const {
    // 开头沿用原 Game.cpp 已有状态区文字，再补课程设计需要的战斗/任务信息。
    std::cout << "\n====角色状态====" << std::endl;
    std::cout << "名字：" << m_player.getName() << std::endl;
    std::cout << "等级：" << m_player.getLV() << std::endl;
    std::cout << "血量：" << m_player.getNHP() << "/" << m_player.getMHP() << std::endl;
    std::cout << "体力：" << m_player.getTP() << std::endl;
    std::cout << "元素充能：" << m_player.getENERGY() << std::endl;
    std::cout << "攻击：" << m_player.effectiveAttack() << "  防御：" << m_player.effectiveDefense() << std::endl;
    std::cout << "经验：" << m_player.getExp() << "/" << m_player.getLevel() * 100 << std::endl;
    std::cout << "摩拉：" << m_player.getMora() << std::endl;
    std::cout << "主线：" << m_quest.currentObjective() << std::endl;
}

void Game::saveGame() const {
    std::string error;
    if (SaveManager::save(m_savePath, m_player, m_currentRoom, m_quest,
                          m_defeatedEnemies, error)) {
        // 保留原来的成功提示。
        std::cout << "\n 游戏已保存！";
    } else {
        std::cout << RED << "\n 保存失败：" << error << RESET;
    }
}

void Game::showEnding() {
    clearScreen();
    std::cout << CYAN << "========================================================================================================================\n";
    std::cout << "终章：风与龙之歌\n";
    std::cout << "========================================================================================================================\n" << RESET;
    std::cout << "风之护盾在最后一击中破碎，净化结晶的光芒笼罩特瓦林。\n";
    std::cout << "深渊留下的污秽逐渐消散，巨龙的目光重新恢复清明。\n";
    std::cout << "温迪的琴声从高塔下方传来，狂暴的风终于变得柔和。\n\n";
    std::cout << GREEN << "你没有杀死风魔龙，而是完成了净化。蒙德重新迎来了自由的风。" << RESET << "\n";
    std::cout << "\n——《风与龙之歌》主线完成 ——\n";
    pauseWithOriginalText();
}
