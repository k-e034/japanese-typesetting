/**
 * @file unicode.cpp
 * @brief Unicode処理の実装
 */

#include "japanese_typesetting/core/unicode/unicode.h"
#include <unicode/unistr.h>
#include <unicode/normlzr.h>
#include <unicode/uchar.h>
#include <unicode/utf.h>
#include <algorithm> // std::find用に追加

namespace japanese_typesetting {
namespace core {
namespace unicode {

UnicodeHandler::UnicodeHandler() {
    // 日本語文字の範囲を設定
    m_japaneseRanges.push_back(std::make_pair(0x3040, 0x309F)); // ひらがな
    m_japaneseRanges.push_back(std::make_pair(0x30A0, 0x30FF)); // カタカナ
    m_japaneseRanges.push_back(std::make_pair(0x4E00, 0x9FFF)); // 漢字（CJK統合漢字）
    m_japaneseRanges.push_back(std::make_pair(0xFF00, 0xFFEF)); // 全角ASCII・記号

    // 句読点
    m_punctuations.push_back(U'、');
    m_punctuations.push_back(U'。');
    m_punctuations.push_back(U'，');
    m_punctuations.push_back(U'．');
    m_punctuations.push_back(U'？');
    m_punctuations.push_back(U'！');

    // 開き括弧
    m_openingBrackets.push_back(U'（');
    m_openingBrackets.push_back(U'［');
    m_openingBrackets.push_back(U'｛');
    m_openingBrackets.push_back(U'「');
    m_openingBrackets.push_back(U'『');
    m_openingBrackets.push_back(U'【');
    m_openingBrackets.push_back(U'〔');
    m_openingBrackets.push_back(U'〈');
    m_openingBrackets.push_back(U'《');

    // 閉じ括弧
    m_closingBrackets.push_back(U'）');
    m_closingBrackets.push_back(U'］');
    m_closingBrackets.push_back(U'｝');
    m_closingBrackets.push_back(U'」');
    m_closingBrackets.push_back(U'』');
    m_closingBrackets.push_back(U'】');
    m_closingBrackets.push_back(U'〕');
    m_closingBrackets.push_back(U'〉');
    m_closingBrackets.push_back(U'》');
}

UnicodeHandler::~UnicodeHandler() {
    // 特に何もしない
}

std::u32string UnicodeHandler::utf8ToUtf32(const std::string& utf8String) const {
    icu::UnicodeString ustr = icu::UnicodeString::fromUTF8(utf8String);
    std::u32string result;
    result.reserve(ustr.length());
    
    for (int32_t i = 0; i < ustr.length(); ) {
        UChar32 c = ustr.char32At(i);
        result.push_back(static_cast<char32_t>(c));
        i += U16_LENGTH(c);
    }
    
    return result;
}

std::string UnicodeHandler::utf32ToUtf8(const std::u32string& utf32String) const {
    icu::UnicodeString ustr;
    for (char32_t ch : utf32String) {
        ustr.append(static_cast<UChar32>(ch));
    }
    
    std::string result;
    ustr.toUTF8String(result);
    return result;
}

bool UnicodeHandler::isJapaneseCharacter(char32_t character) const {
    for (const auto& range : m_japaneseRanges) {
        if (character >= range.first && character <= range.second) {
            return true;
        }
    }
    return false;
}

bool UnicodeHandler::isFullWidthCharacter(char32_t character) const {
    // ICUを使用して全角かどうかを判定
    return u_getIntPropertyValue(character, UCHAR_EAST_ASIAN_WIDTH) == U_EA_FULLWIDTH ||
           u_getIntPropertyValue(character, UCHAR_EAST_ASIAN_WIDTH) == U_EA_WIDE;
}

bool UnicodeHandler::isHalfWidthCharacter(char32_t character) const {
    // ICUを使用して半角かどうかを判定
    return u_getIntPropertyValue(character, UCHAR_EAST_ASIAN_WIDTH) == U_EA_HALFWIDTH ||
           u_getIntPropertyValue(character, UCHAR_EAST_ASIAN_WIDTH) == U_EA_NARROW;
}

bool UnicodeHandler::isPunctuation(char32_t character) const {
    return std::find(m_punctuations.begin(), m_punctuations.end(), character) != m_punctuations.end();
}

bool UnicodeHandler::isOpeningBracket(char32_t character) const {
    return std::find(m_openingBrackets.begin(), m_openingBrackets.end(), character) != m_openingBrackets.end();
}

bool UnicodeHandler::isClosingBracket(char32_t character) const {
    return std::find(m_closingBrackets.begin(), m_closingBrackets.end(), character) != m_closingBrackets.end();
}

std::string UnicodeHandler::normalize(const std::string& text) const {
    UErrorCode status = U_ZERO_ERROR;
    icu::UnicodeString ustr = icu::UnicodeString::fromUTF8(text);
    icu::Normalizer2 const *normalizer = icu::Normalizer2::getNFKCInstance(status);
    
    if (U_FAILURE(status)) {
        return text; // エラーが発生した場合は元のテキストを返す
    }
    
    icu::UnicodeString normalized = normalizer->normalize(ustr, status);
    if (U_FAILURE(status)) {
        return text;
    }
    
    std::string result;
    normalized.toUTF8String(result);
    return result;
}

} // namespace unicode
} // namespace core
} // namespace japanese_typesetting
