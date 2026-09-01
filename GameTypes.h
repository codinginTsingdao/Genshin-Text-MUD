#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <string>

enum class Direction { North, South, East, West, Invalid };

enum class CommandType {
    Help, Look, Map, North, South, East, West,
    Talk, Attack, Inventory, Use, Equip, Unequip,
    Status, Save, Load, NewGame, Menu, Quit, Unknown
};

struct Command {
    CommandType type{CommandType::Unknown};
    std::string argument;
};

enum class QuestStage {
    Awakening = 0,
    MetAmber = 1,
    HilichurlCleared = 2,
    EnteredMondstadt = 3,
    CityBriefing = 4,
    DragonTearAccepted = 5,
    SearchingCrystals = 6,
    SealsBroken = 7,
    StormterrorLair = 8,
    DragonPurified = 9
};

enum class CombatResult { Victory, Defeat, Escaped };

#endif
