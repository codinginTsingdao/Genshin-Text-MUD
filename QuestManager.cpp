#include "QuestManager.h"
#include <algorithm>

QuestStage QuestManager::getStage() const { return m_stage; }
void QuestManager::setStage(QuestStage stage) { m_stage = stage; }
int QuestManager::getSealsBroken() const { return m_sealsBroken; }
void QuestManager::setSealsBroken(int count) { m_sealsBroken = std::clamp(count, 0, 3); }
bool QuestManager::atLeast(QuestStage stage) const {
    return static_cast<int>(m_stage) >= static_cast<int>(stage);
}
bool QuestManager::advanceTo(QuestStage stage) {
    if (static_cast<int>(stage) <= static_cast<int>(m_stage)) return false;
    m_stage = stage;
    return true;
}
bool QuestManager::breakSeal() {
    if (m_sealsBroken >= 3) return false;
    ++m_sealsBroken;
    if (m_sealsBroken >= 3) advanceTo(QuestStage::SealsBroken);
    return true;
}

std::string QuestManager::stageName() const {
    switch (m_stage) {
        case QuestStage::Awakening: return "初到蒙德";
        case QuestStage::MetAmber: return "侦察骑士的委托";
        case QuestStage::HilichurlCleared: return "前往蒙德城";
        case QuestStage::EnteredMondstadt: return "风龙危机";
        case QuestStage::CityBriefing: return "寻找温迪";
        case QuestStage::DragonTearAccepted: return "风龙的眼泪";
        case QuestStage::SearchingCrystals: return "解除风之封印";
        case QuestStage::SealsBroken: return "进入风龙废墟";
        case QuestStage::StormterrorLair: return "决战风魔龙";
        case QuestStage::DragonPurified: return "风与龙之歌";
    }
    return "未知";
}

std::string QuestManager::currentObjective() const {
    switch (m_stage) {
        case QuestStage::Awakening: return "沿森林小径前进，寻找可以询问情况的人。";
        case QuestStage::MetAmber: return "前往丘丘人营地并击败丘丘人。";
        case QuestStage::HilichurlCleared: return "通过蒙德城门进入蒙德城。";
        case QuestStage::EnteredMondstadt: return "前往西风骑士团，与琴对话。";
        case QuestStage::CityBriefing: return "回到中央广场，与温迪对话。";
        case QuestStage::DragonTearAccepted: return "前往风起地寻找温迪。";
        case QuestStage::SearchingCrystals:
            return "探索风起地遗迹并解除三道风之封印（" + std::to_string(m_sealsBroken) + "/3）。";
        case QuestStage::SealsBroken: return "封印已解除，前往风龙废墟。";
        case QuestStage::StormterrorLair: return "登上高塔，在天空战场净化风魔龙。";
        case QuestStage::DragonPurified: return "主线已经完成。";
    }
    return "";
}
