/**
 * @file typesetting_engine.h
 * @brief 組版エンジンの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_CORE_TYPESETTING_ENGINE_H
#define JAPANESE_TYPESETTING_CORE_TYPESETTING_ENGINE_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_rules.h"
#include "japanese_typesetting/core/unicode/unicode.h"
#include <string>
#include <vector>
#include <memory>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

/**
 * @struct TextLine
 * @brief 組版された1行のテキストを表す構造体
 */
struct TextLine {
    std::u32string text;      ///< 行のテキスト（UTF-32）
    double width;             ///< 行の幅
    double height;            ///< 行の高さ
    double baseline;          ///< ベースラインの位置
    bool hasLineBreak;        ///< 明示的な改行があるかどうか
};

/**
 * @struct TextBlock
 * @brief 組版されたテキストブロックを表す構造体
 */
struct TextBlock {
    std::vector<TextLine> lines;  ///< 行のリスト
    double width;                 ///< ブロックの幅
    double height;                ///< ブロックの高さ
};

/**
 * @class TypesettingEngine
 * @brief 日本語組版エンジンのクラス
 */
class TypesettingEngine {
public:
    /**
     * @brief コンストラクタ
     */
    TypesettingEngine();

    /**
     * @brief デストラクタ
     */
    ~TypesettingEngine();

    /**
     * @brief 組版ルールを設定
     * @param rules 組版ルール
     */
    void setTypesettingRules(const TypesettingRules& rules);

    /**
     * @brief 組版ルールを取得
     * @return 組版ルール
     */
    const TypesettingRules& getTypesettingRules() const;

    /**
     * @brief Unicodeハンドラを設定
     * @param handler Unicodeハンドラ
     */
    void setUnicodeHandler(const unicode::UnicodeHandler& handler);

    /**
     * @brief Unicodeハンドラを取得
     * @return Unicodeハンドラ
     */
    const unicode::UnicodeHandler& getUnicodeHandler() const;

    /**
     * @brief テキストを組版する
     * @param text 組版するテキスト（UTF-8）
     * @param style スタイル
     * @param width 最大幅
     * @param vertical 縦書きの場合はtrue
     * @return 組版されたテキストブロック
     */
    TextBlock typeset(const std::string& text, const style::Style& style, double width, bool vertical = true);

    /**
     * @brief 文書を組版する
     * @param document 組版する文書
     * @param style スタイル
     * @param width 最大幅
     * @return 組版されたテキストブロックのリスト
     */
    std::vector<TextBlock> typesetDocument(const document::Document& document, const style::Style& style, double width);

private:
    /**
     * @brief 行分割を行う
     * @param text 分割するテキスト（UTF-32）
     * @param style スタイル
     * @param maxWidth 最大幅
     * @param vertical 縦書きの場合はtrue
     * @return 分割された行のリスト
     */
    std::vector<TextLine> breakLines(const std::u32string& text, const style::Style& style, double maxWidth, bool vertical);

    /**
     * @brief 禁則処理を適用する
     * @param lines 行のリスト
     * @param style スタイル
     * @param vertical 縦書きの場合はtrue
     */
    void applyProhibitionRules(std::vector<TextLine>& lines, const style::Style& style, bool vertical);

    /**
     * @brief 文字詰め処理を適用する
     * @param lines 行のリスト
     * @param style スタイル
     * @param maxWidth 最大幅
     * @param vertical 縦書きの場合はtrue
     */
    void applyJustification(std::vector<TextLine>& lines, const style::Style& style, double maxWidth, bool vertical);

    /**
     * @brief ぶら下げ処理を適用する
     * @param lines 行のリスト
     * @param style スタイル
     * @param vertical 縦書きの場合はtrue
     */
    void applyHanging(std::vector<TextLine>& lines, const style::Style& style, bool vertical);

    /**
     * @brief 文字の幅を計算する
     * @param character 文字（UTF-32）
     * @param style スタイル
     * @param vertical 縦書きの場合はtrue
     * @return 文字の幅
     */
    double calculateCharacterWidth(char32_t character, const style::Style& style, bool vertical);

    /**
     * @brief 文字の高さを計算する
     * @param character 文字（UTF-32）
     * @param style スタイル
     * @param vertical 縦書きの場合はtrue
     * @return 文字の高さ
     */
    double calculateCharacterHeight(char32_t character, const style::Style& style, bool vertical);

    /**
     * @brief テキストの幅を計算する
     * @param text テキスト（UTF-32）
     * @param style スタイル
     * @param vertical 縦書きの場合はtrue
     * @return テキストの幅
     */
    double calculateTextWidth(const std::u32string& text, const style::Style& style, bool vertical);

    TypesettingRules m_rules;                 ///< 組版ルール
    unicode::UnicodeHandler m_unicodeHandler; ///< Unicodeハンドラ
};

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_TYPESETTING_ENGINE_H
