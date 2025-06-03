/**
 * @file ruby.h
 * @brief ルビ処理の詳細実装
 */

#ifndef JAPANESE_TYPESETTING_CORE_TYPESETTING_RUBY_H
#define JAPANESE_TYPESETTING_CORE_TYPESETTING_RUBY_H

#include "japanese_typesetting/core/style/style.h"
#include <string>
#include <vector>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

/**
 * @struct RubyText
 * @brief ルビテキストを表す構造体
 */
struct RubyText {
    std::u32string base;      ///< ベーステキスト（親文字）
    std::u32string ruby;      ///< ルビテキスト
    size_t startPos;          ///< 元のテキスト内での開始位置
    size_t endPos;            ///< 元のテキスト内での終了位置
};

/**
 * @class RubyProcessor
 * @brief ルビ処理を実装するクラス
 */
class RubyProcessor {
public:
    /**
     * @brief コンストラクタ
     */
    RubyProcessor();

    /**
     * @brief デストラクタ
     */
    ~RubyProcessor();

    /**
     * @brief テキスト内のルビ注釈を解析する
     * @param text 解析するテキスト（UTF-32）
     * @return 検出されたルビテキストのリスト
     */
    std::vector<RubyText> parseRuby(const std::u32string& text);

    /**
     * @brief ルビ付きテキストを描画するための情報を計算する
     * @param rubyText ルビテキスト
     * @param baseStyle ベーステキストのスタイル
     * @param rubyStyle ルビテキストのスタイル
     * @param vertical 縦書きの場合はtrue
     * @return ルビの配置情報（将来的に拡張）
     */
    void calculateRubyLayout(RubyText& rubyText, const style::Style& baseStyle, const style::Style& rubyStyle, bool vertical);

private:
    /**
     * @brief ルビ注釈の開始タグを検出する
     * @param text テキスト
     * @param startPos 検索開始位置
     * @return タグの位置、見つからない場合は-1
     */
    size_t findRubyStartTag(const std::u32string& text, size_t startPos);

    /**
     * @brief ルビ注釈の終了タグを検出する
     * @param text テキスト
     * @param startPos 検索開始位置
     * @return タグの位置、見つからない場合は-1
     */
    size_t findRubyEndTag(const std::u32string& text, size_t startPos);

    /**
     * @brief ルビテキストの開始タグを検出する
     * @param text テキスト
     * @param startPos 検索開始位置
     * @return タグの位置、見つからない場合は-1
     */
    size_t findRubyTextTag(const std::u32string& text, size_t startPos);
};

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CORE_TYPESETTING_RUBY_H
