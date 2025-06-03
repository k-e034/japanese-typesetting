/**
 * @file vertical_layout.cpp
 * @brief 縦書き・横書き切り替え機能の詳細実装
 */

#include "japanese_typesetting/core/typesetting/vertical_layout.h"
#include <map>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

// 横書き文字から縦書き文字への変換マップ
static const std::map<char32_t, char32_t> horizontalToVerticalMap = {
    {U'(', U'︵'}, // 左括弧
    {U')', U'︶'}, // 右括弧
    {U'[', U'﹇'}, // 左角括弧
    {U']', U'﹈'}, // 右角括弧
    {U'{', U'︷'}, // 左波括弧
    {U'}', U'︸'}, // 右波括弧
    {U'<', U'︿'}, // 左山括弧
    {U'>', U'﹀'}, // 右山括弧
    {U'«', U'︽'}, // 左二重山括弧
    {U'»', U'︾'}, // 右二重山括弧
    {U'—', U'︱'}, // ダッシュ
    {U'－', U'｜'}, // ハイフン
    {U'…', U'︙'}, // 三点リーダー
};

// 縦書き文字から横書き文字への変換マップ
static const std::map<char32_t, char32_t> verticalToHorizontalMap = {
    {U'︵', U'('}, // 左括弧
    {U'︶', U')'}, // 右括弧
    {U'﹇', U'['}, // 左角括弧
    {U'﹈', U']'}, // 右角括弧
    {U'︷', U'{'}, // 左波括弧
    {U'︸', U'}'}, // 右波括弧
    {U'︿', U'<'}, // 左山括弧
    {U'﹀', U'>'}, // 右山括弧
    {U'︽', U'«'}, // 左二重山括弧
    {U'︾', U'»'}, // 右二重山括弧
    {U'︱', U'—'}, // ダッシュ
    {U'｜', U'－'}, // ハイフン
    {U'︙', U'…'}, // 三点リーダー
};

VerticalLayoutProcessor::VerticalLayoutProcessor(const unicode::UnicodeHandler& unicodeHandler)
    : m_unicodeHandler(unicodeHandler) {
}

VerticalLayoutProcessor::~VerticalLayoutProcessor() {
    // 特に何もしない
}

char32_t VerticalLayoutProcessor::convertToVertical(char32_t character) const {
    // 横書き文字から縦書き文字への変換
    auto it = horizontalToVerticalMap.find(character);
    if (it != horizontalToVerticalMap.end()) {
        return it->second;
    }
    
    return character; // 変換がない場合は元の文字を返す
}

char32_t VerticalLayoutProcessor::convertToHorizontal(char32_t character) const {
    // 縦書き文字から横書き文字への変換
    auto it = verticalToHorizontalMap.find(character);
    if (it != verticalToHorizontalMap.end()) {
        return it->second;
    }
    
    return character; // 変換がない場合は元の文字を返す
}

std::u32string VerticalLayoutProcessor::convertToVertical(const std::u32string& text) const {
    std::u32string result;
    result.reserve(text.length());
    
    for (char32_t ch : text) {
        result.push_back(convertToVertical(ch));
    }
    
    return result;
}

std::u32string VerticalLayoutProcessor::convertToHorizontal(const std::u32string& text) const {
    std::u32string result;
    result.reserve(text.length());
    
    for (char32_t ch : text) {
        result.push_back(convertToHorizontal(ch));
    }
    
    return result;
}

int VerticalLayoutProcessor::getCharacterRotation(char32_t character, bool vertical) const {
    if (!vertical) {
        return 0; // 横書きの場合は回転なし
    }
    
    // 縦書きでの文字の回転角度を決定
    // アルファベット、数字は90度回転
    if ((character >= U'a' && character <= U'z') ||
        (character >= U'A' && character <= U'Z') ||
        (character >= U'0' && character <= U'9')) {
        return 90;
    }
    
    // 一部の記号も90度回転
    if (character == U'/' || character == U'\\' || 
        character == U'|' || character == U'-' || 
        character == U'_' || character == U'=' || 
        character == U'+' || character == U'*') {
        return 90;
    }
    
    // その他の文字は回転なし
    return 0;
}

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting
