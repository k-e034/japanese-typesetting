/**
 * @file unicode.h
 * @brief Unicode処理のヘッダファイル
 */

#ifndef JAPANESE_TYPESETTING_CORE_UNICODE_UNICODE_H
#define JAPANESE_TYPESETTING_CORE_UNICODE_UNICODE_H

#include <string>
#include <vector>

namespace japanese_typesetting {
namespace core {
namespace unicode {

/**
 * @class UnicodeHandler
 * @brief Unicode文字処理を行うクラス
 */
class UnicodeHandler {
public:
    /**
     * @brief コンストラクタ
     */
    UnicodeHandler();

    /**
     * @brief デストラクタ
     */
    ~UnicodeHandler();

    /**
     * @brief UTF-8文字列をUTF-32に変換
     * @param utf8String UTF-8文字列
     * @return UTF-32文字列（char32_tの配列）
     */
    std::u32string utf8ToUtf32(const std::string& utf8String) const;

    /**
     * @brief UTF-32文字列をUTF-8に変換
     * @param utf32String UTF-32文字列
     * @return UTF-8文字列
     */
    std::string utf32ToUtf8(const std::u32string& utf32String) const;

    /**
     * @brief 文字が日本語かどうかを判定
     * @param character 判定する文字
     * @return 日本語の場合はtrue
     */
    bool isJapaneseCharacter(char32_t character) const;

    /**
     * @brief 文字が全角かどうかを判定
     * @param character 判定する文字
     * @return 全角の場合はtrue
     */
    bool isFullWidthCharacter(char32_t character) const;

    /**
     * @brief 文字が半角かどうかを判定
     * @param character 判定する文字
     * @return 半角の場合はtrue
     */
    bool isHalfWidthCharacter(char32_t character) const;

    /**
     * @brief 文字が句読点かどうかを判定
     * @param character 判定する文字
     * @return 句読点の場合はtrue
     */
    bool isPunctuation(char32_t character) const;

    /**
     * @brief 文字が開き括弧かどうかを判定
     * @param character 判定する文字
     * @return 開き括弧の場合はtrue
     */
    bool isOpeningBracket(char32_t character) const;

    /**
     * @brief 文字が閉じ括弧かどうかを判定
     * @param character 判定する文字
     * @return 閉じ括弧の場合はtrue
     */
    bool isClosingBracket(char32_t character) const;

    /**
     * @brief 文字列を正規化する（NFKC）
     * @param text 正規化する文字列
     * @return 正規化された文字列
     */
    std::string normalize(const std::string& text) const;

private:
    // 内部実装用のデータ構造
    std::vector<std::pair<char32_t, char32_t>> m_japaneseRanges;
    std::vector<char32_t> m_punctuations;
    std::vector<char32_t> m_openingBrackets;
    std::vector<char32_t> m_closingBrackets;
};

} // namespace unicode
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_UNICODE_UNICODE_H
