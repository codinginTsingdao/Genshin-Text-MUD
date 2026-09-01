#include "World.h"

void World::addRoom(const Room& room) { m_rooms[room.getId()] = room; }
Room* World::getRoom(int roomId) {
    auto it = m_rooms.find(roomId);
    return it == m_rooms.end() ? nullptr : &it->second;
}
const Room* World::getRoom(int roomId) const {
    auto it = m_rooms.find(roomId);
    return it == m_rooms.end() ? nullptr : &it->second;
}

int World::moveTarget(int fromRoom, Direction direction) const {
    const Room* room = getRoom(fromRoom);
    return room ? room->getExit(direction) : 0;
}

bool World::canMove(int fromRoom, Direction direction, QuestStage stage,
                    const std::set<int>& defeatedEnemies, std::string& reason) const {
    int target = moveTarget(fromRoom, direction);
    if (target == 0) {
        reason = "这个方向没有路。";
        return false;
    }

    if (fromRoom == 3 && direction == Direction::North && defeatedEnemies.count(1001) == 0) {
        reason = "丘丘人封锁了通往蒙德城门的道路。先解决营地里的敌人。";
        return false;
    }
    if (target == 9 && static_cast<int>(stage) < static_cast<int>(QuestStage::DragonTearAccepted)) {
        reason = "你还不知道为什么要前往风起地。先在蒙德城推进主线。";
        return false;
    }
    if (target == 10 && static_cast<int>(stage) < static_cast<int>(QuestStage::SearchingCrystals)) {
        reason = "先与风起地大树下的温迪对话，取得风之钥匙。";
        return false;
    }
    if (target == 13 && static_cast<int>(stage) < static_cast<int>(QuestStage::SealsBroken)) {
        reason = "三道风之封印仍未解除，风龙废墟无法进入。";
        return false;
    }

    return true;
}
