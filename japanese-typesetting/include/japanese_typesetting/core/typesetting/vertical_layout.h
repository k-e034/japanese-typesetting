/**
 * @file vertical_layout.h
 * @brief 縦書き・横書き切り替え機能の詳細実装
 */

#ifndef JAPANESE_TYPESETTING_CORE_TYPESETTING_VERTICAL_LAYOUT_H
#define JAPANESE_TYPESETTING_CORE_TYPESETTING_VERTICAL_LAYOUT_H

#include "japanese_typesetting/core/unicode/unicode.h"
#include <string>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

/**
 * @class VerticalLayoutProcessor
 * @brief 縦書き・横書き切り替え処理を実装するクラス
 */
class VerticalLayoutProcessor {
public:
    /**
     * @brief コンストラクタ
     * @param unicodeHandler Unicodeハンドラ
     */
    VerticalLayoutProcessor(const unicode::UnicodeHandler& unicodeHandler);

    /**
     * @brief デストラクタ
     */
    ~VerticalLayoutProcessor();

    /**
     * @brief 文字を縦書き用に変換する
     * @param character 変換する文字（UTF-32）
     * @return 縦書き用に変換された文字（UTF-32）
     */
    char32_t convertToVertical(char32_t character) const;

    /**
     * @brief 文字を横書き用に変換する
     * @param character 変換する文字（UTF-32）
     * @return 横書き用に変換された文字（UTF-32）
     */
    char32_t convertToHorizontal(char32_t character) const;

    /**
     * @brief テキストを縦書き用に変換する
     * @param text 変換するテキスト（UTF-32）
     * @return 縦書き用に変換されたテキスト（UTF-32）
     */
    std::u32string convertToVertical(const std::u32string& text) const;

    /**
     * @brief テキストを横書き用に変換する
     * @param text 変換するテキスト（UTF-32）
     * @return 横書き用に変換されたテキスト（UTF-32）
     */
    std::u32string convertToHorizontal(const std::u32string& text) const;

    /**
     * @brief 文字の回転角度を取得する
     * @param character 文字（UTF-32）
     * @param vertical 縦書きの場合はtrue
     * @return 回転角度（度数法）
     */
    int getCharacterRotation(char32_t character, bool vertical) const;

private:
    const unicode::UnicodeHandler& m_unicodeHandler; ///< Unicodeハンドラ
};

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_TYPESETTING_VERTICAL_LAYOUT_H
