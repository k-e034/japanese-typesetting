/**
 * @file typesetting.cpp
 * @brief 組版処理の実装
 */

#include "japanese_typesetting/core/typesetting/typesetting.h"
#include <algorithm>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

TypesettingEngine::TypesettingEngine()
    : m_characterFittingEnabled(true) {
    // 禁則ルールをデフォルトで有効化
    m_prohibitionRules[static_cast<int>(ProhibitionRule::LineStart)] = true;
    m_prohibitionRules[static_cast<int>(ProhibitionRule::LineEnd)] = true;
    m_prohibitionRules[static_cast<int>(ProhibitionRule::Hanging)] = true;
    m_prohibitionRules[static_cast<int>(ProhibitionRule::WordBreak)] = true;

    // JIS X 4051に基づく行頭禁則文字の初期設定
    // 句読点
    addLineStartProhibitedCharacter(U'、');
    addLineStartProhibitedCharacter(U'。');
    addLineStartProhibitedCharacter(U'，');
    addLineStartProhibitedCharacter(U'．');
    addLineStartProhibitedCharacter(U'？');
    addLineStartProhibitedCharacter(U'！');
    // 閉じ括弧類
    addLineStartProhibitedCharacter(U'）');
    addLineStartProhibitedCharacter(U'］');
    addLineStartProhibitedCharacter(U'｝');
    addLineStartProhibitedCharacter(U'」');
    addLineStartProhibitedCharacter(U'』');
    addLineStartProhibitedCharacter(U'】');
    addLineStartProhibitedCharacter(U'〕');
    addLineStartProhibitedCharacter(U'〉');
    addLineStartProhibitedCharacter(U'》');

    // JIS X 4051に基づく行末禁則文字の初期設定
    // 開き括弧類
    addLineEndProhibitedCharacter(U'（');
    addLineEndProhibitedCharacter(U'［');
    addLineEndProhibitedCharacter(U'｛');
    addLineEndProhibitedCharacter(U'「');
    addLineEndProhibitedCharacter(U'『');
    addLineEndProhibitedCharacter(U'【');
    addLineEndProhibitedCharacter(U'〔');
    addLineEndProhibitedCharacter(U'〈');
    addLineEndProhibitedCharacter(U'《');
}

TypesettingEngine::~TypesettingEngine() {
    // 特に何もしない
}

bool TypesettingEngine::process(const document::Document& document) {
    // 実際の処理は派生クラスで実装
    return true;
}

void TypesettingEngine::setProhibitionRule(ProhibitionRule rule, bool enabled) {
    m_prohibitionRules[static_cast<int>(rule)] = enabled;
}

bool TypesettingEngine::isProhibitionRuleEnabled(ProhibitionRule rule) const {
    return m_prohibitionRules[static_cast<int>(rule)];
}

void TypesettingEngine::addLineStartProhibitedCharacter(char32_t character) {
    if (std::find(m_lineStartProhibitedChars.begin(), m_lineStartProhibitedChars.end(), character) == m_lineStartProhibitedChars.end()) {
        m_lineStartProhibitedChars.push_back(character);
    }
}

void TypesettingEngine::addLineEndProhibitedCharacter(char32_t character) {
    if (std::find(m_lineEndProhibitedChars.begin(), m_lineEndProhibitedChars.end(), character) == m_lineEndProhibitedChars.end()) {
        m_lineEndProhibitedChars.push_back(character);
    }
}

void TypesettingEngine::setCharacterFitting(bool enabled) {
    m_characterFittingEnabled = enabled;
}

bool TypesettingEngine::isCharacterFittingEnabled() const {
    return m_characterFittingEnabled;
}

std::string TypesettingEngine::applyLineStartProhibition(const std::string& text) {
    // 実際の実装は派生クラスで行う
    return text;
}

std::string TypesettingEngine::applyLineEndProhibition(const std::string& text) {
    // 実際の実装は派生クラスで行う
    return text;
}

std::string TypesettingEngine::applyHangingProhibition(const std::string& text) {
    // 実際の実装は派生クラスで行う
    return text;
}

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting
