#ifndef ROOM_H
#define ROOM_H

#include "GameTypes.h"
#include "NPC.h"
#include <string>
#include <unordered_map>
#include <vector>

struct DirectionHash {
    std::size_t operator()(Direction d) const noexcept {
        return static_cast<std::size_t>(d);
    }
};

class Room {
private:
    int m_id{0};
    std::string m_area;
    std::string m_name;
    std::string m_description;
    std::unordered_map<Direction, int, DirectionHash> m_exits;
    std::vector<NPC> m_npcs;
    std::vector<int> m_enemySpawnIds;

public:
    Room() = default;
    Room(int id, std::string area, std::string name, std::string description);

    int getId() const;
    const std::string& getArea() const;
    const std::string& getName() const;
    const std::string& getDescription() const;

    void addExit(Direction direction, int roomId);
    int getExit(Direction direction) const;
    const std::unordered_map<Direction, int, DirectionHash>& getExits() const;

    void addNPC(const NPC& npc);
    void addEnemy(int spawnId);
    const std::vector<NPC>& getNPCs() const;
    const std::vector<int>& getEnemySpawnIds() const;
};

#endif
