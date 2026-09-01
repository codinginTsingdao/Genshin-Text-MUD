#include "CommandParser.h"
#include <algorithm>
#include <cctype>
#include <sstream>

std::string CommandParser::trim(const std::string& text) {
    std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::string CommandParser::lowerAscii(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return c < 128 ? static_cast<char>(std::tolower(c)) : static_cast<char>(c);
    });
    return text;
}

Command CommandParser::parse(const std::string& input) const {
    std::string line = trim(input);
    if (line.empty()) return {};

    std::string verb;
    std::string argument;
    std::size_t pos = line.find_first_of(" \t");
    if (pos == std::string::npos) verb = line;
    else {
        verb = line.substr(0, pos);
        argument = trim(line.substr(pos + 1));
    }
    verb = lowerAscii(verb);

    if (verb == "help" || verb == "帮助") return {CommandType::Help, argument};
    if (verb == "look" || verb == "查看") return {CommandType::Look, argument};
    if (verb == "map" ||verb == "地图") return { CommandType::Map, argument };
    if (verb == "north" || verb == "n" || verb == "北") return {CommandType::North, argument};
    if (verb == "south" || verb == "s" || verb == "南") return {CommandType::South, argument};
    if (verb == "east" || verb == "e" || verb == "东") return {CommandType::East, argument};
    if (verb == "west" || verb == "w" || verb == "西") return {CommandType::West, argument};
    if (verb == "talk" || verb == "对话") return {CommandType::Talk, argument};
    if (verb == "attack" || verb == "攻击") return {CommandType::Attack, argument};
    if (verb == "inventory" || verb == "inv" || verb == "背包") return {CommandType::Inventory, argument};
    if (verb == "use" || verb == "使用") return {CommandType::Use, argument};
    if (verb == "equip" || verb == "装备") return {CommandType::Equip, argument};
    if (verb == "unequip" || verb == "卸下") return {CommandType::Unequip, argument};
    if (verb == "status" || verb == "状态") return {CommandType::Status, argument};
    if (verb == "save" || verb == "保存") return {CommandType::Save, argument};
    if (verb == "load" || verb == "读取") return {CommandType::Load, argument};
    if (verb == "new" || verb == "新游戏") return {CommandType::NewGame, argument};
    if (verb == "menu" || verb == "菜单" || verb == "返回" || verb == "back") return {CommandType::Menu, argument};
    if (verb == "quit" || verb == "exit" || verb == "退出") return {CommandType::Quit, argument};
    return {CommandType::Unknown, argument};
}
