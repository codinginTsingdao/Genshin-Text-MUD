#include "NPC.h"

NPC::NPC(const std::string& name, const std::string& role)
    : Character(name, 1, 1, 0, 0, 0, 1.0, 0.0), m_role(role) {}
const std::string& NPC::getRole() const { return m_role; }
