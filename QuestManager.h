#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include "GameTypes.h"
#include <string>

class QuestManager {
private:
    QuestStage m_stage{QuestStage::Awakening};
    int m_sealsBroken{0};

public:
    QuestStage getStage() const;
    void setStage(QuestStage stage);
    int getSealsBroken() const;
    void setSealsBroken(int count);

    bool atLeast(QuestStage stage) const;
    bool advanceTo(QuestStage stage);
    bool breakSeal();

    std::string currentObjective() const;
    std::string stageName() const;
};

#endif
