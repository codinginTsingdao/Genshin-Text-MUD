#include "Room.h"
#include <utility>

Room::Room(int id, std::string area, std::string name, std::string description)
    : m_id(id), m_area(std::move(area)), m_name(std::move(name)),
      m_description(std::move(description)) {}

int Room::getId() const { return m_id; }
const std::string& Room::getArea() const { return m_area; }
const std::string& Room::getName() const { return m_name; }
const std::string& Room::getDescription() const { return m_description; }
void Room::addExit(Direction direction, int roomId) { m_exits[direction] = roomId; }
int Room::getExit(Direction direction) const {
    auto it = m_exits.find(direction);
    return it == m_exits.end() ? 0 : it->second;
}
const std::unordered_map<Direction, int, DirectionHash>& Room::getExits() const { return m_exits; }
void Room::addNPC(const NPC& npc) { m_npcs.push_back(npc); }
void Room::addEnemy(int spawnId) { m_enemySpawnIds.push_back(spawnId); }
const std::vector<NPC>& Room::getNPCs() const { return m_npcs; }
const std::vector<int>& Room::getEnemySpawnIds() const { return m_enemySpawnIds; }
