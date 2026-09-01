#ifndef WORLD_H
#define WORLD_H

#include "Room.h"
#include <set>
#include <string>
#include <unordered_map>

class World {
private:
    std::unordered_map<int, Room> m_rooms;

public:
    void addRoom(const Room& room);
    Room* getRoom(int roomId);
    const Room* getRoom(int roomId) const;

    bool canMove(int fromRoom, Direction direction, QuestStage stage,
                 const std::set<int>& defeatedEnemies, std::string& reason) const;
    int moveTarget(int fromRoom, Direction direction) const;
};

#endif
