#ifndef NPC_H
#define NPC_H

#include "Character.h"
#include <string>

class NPC : public Character {
private:
    std::string m_role;

public:
    NPC(const std::string& name = "", const std::string& role = "");
    const std::string& getRole() const;
};

#endif
