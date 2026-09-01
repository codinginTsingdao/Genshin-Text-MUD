#ifndef GAME_H
#define GAME_H

#include "CombatSystem.h"
#include "CommandParser.h"
#include "Player.h"
#include "QuestManager.h"
#include "World.h"
#include <set>
#include <string>

class Game {
private:
    Player m_player;
    World m_world;
    QuestManager m_quest;
    CommandParser m_parser;
    CombatSystem m_combat;
    std::set<int> m_defeatedEnemies;
    int m_currentRoom{1};
    bool m_running{true};
    bool m_inGame{false};
    bool m_exploring{false};
    const std::string m_savePath{"savegame.txt"};

    void newGame();
    bool loadGame();
    void exploreLoop();
    void executeCommand(const Command& command);

    void showRoom() const;
    void showHelp() const;
    void showStatus() const;
    void showInventory() const;
    void move(Direction direction);
    void talk(const std::string& npcName);
    void attack(const std::string& enemyName);
    void useItem(const std::string& itemName);
    void equipItem(const std::string& itemName);
    void unequipItem(const std::string& slotName);
    void saveGame() const;
    void handleEnemyVictory(Enemy& enemy);
    void showEnding();

    bool roomHasNpc(const std::string& query, std::string& canonicalName) const;

public:
    Game();

    // 保留原仓库的 UI 接口，不改变标题、菜单和剧情风格
    void showTitle();
    void showMenu();
    void inputPlayerName();
    void showStory();
    void gameLoop();
    void run();
};

#endif
