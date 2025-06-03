/**
 * @file line_break.h
 * @brief 行分割アルゴリズムの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_CORE_TYPESETTING_LINE_BREAK_H
#define JAPANESE_TYPESETTING_CORE_TYPESETTING_LINE_BREAK_H

#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_rules.h"
#include "japanese_typesetting/core/unicode/unicode.h"
#include <string>
#include <vector>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

/**
 * @struct BreakPoint
 * @brief テキスト内の分割可能な位置を表す構造体
 */
struct BreakPoint {
    size_t position;      ///< テキスト内の位置
    double penalty;       ///< 分割のペナルティ値（低いほど好ましい）
    bool mandatory;       ///< 強制的な分割点かどうか
};

/**
 * @class LineBreaker
 * @brief 行分割アルゴリズムを実装するクラス
 */
class LineBreaker {
public:
    /**
     * @brief コンストラクタ
     * @param rules 組版ルール
     * @param unicodeHandler Unicodeハンドラ
     */
    LineBreaker(const TypesettingRules& rules, const unicode::UnicodeHandler& unicodeHandler);

    /**
     * @brief デストラクタ
     */
    ~LineBreaker();

    /**
     * @brief 行分割を行う
     * @param text 分割するテキスト（UTF-32）
     * @param style スタイル
     * @param maxWidth 最大幅
     * @param vertical 縦書きの場合はtrue
     * @return 分割された行のリスト
     */
    std::vector<std::u32string> breakLines(const std::u32string& text, const style::Style& style, double maxWidth, bool vertical);

private:
    /**
     * @brief 分割可能な位置を検出する
     * @param text テキスト（UTF-32）
     * @return 分割可能な位置のリスト
     */
    std::vector<BreakPoint> findBreakPoints(const std::u32string& text);

    /**
     * @brief 最適な分割位置を計算する
     * @param text テキスト（UTF-32）
     * @param breakPoints 分割可能な位置のリスト
     * @param style スタイル
     * @param maxWidth 最大幅
     * @param vertical 縦書きの場合はtrue
     * @return 最適な分割位置のリスト
     */
    std::vector<size_t> calculateOptimalBreaks(const std::u32string& text, const std::vector<BreakPoint>& breakPoints, const style::Style& style, double maxWidth, bool vertical);

    /**
     * @brief 文字の幅を計算する
     * @param character 文字（UTF-32）
     * @param style スタイル
     * @param vertical 縦書きの場合はtrue
     * @return 文字の幅
     */
    double calculateCharacterWidth(char32_t character, const style::Style& style, bool vertical);

    const TypesettingRules& m_rules;                 ///< 組版ルール
    const unicode::UnicodeHandler& m_unicodeHandler; ///< Unicodeハンドラ
};

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_TYPESETTING_LINE_BREAK_H
