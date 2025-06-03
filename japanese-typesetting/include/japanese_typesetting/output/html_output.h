/**
 * @file html_output.h
 * @brief HTML出力エンジンの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_OUTPUT_HTML_OUTPUT_H
#define JAPANESE_TYPESETTING_OUTPUT_HTML_OUTPUT_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <string>
#include <vector>
#include <map>

namespace japanese_typesetting {
namespace output {

/**
 * @struct HtmlOutputOptions
 * @brief HTML出力オプションを表す構造体
 */
struct HtmlOutputOptions {
    std::string fontFamily;           ///< フォントファミリー
    double fontSize;                  ///< フォントサイズ（pt）
    double lineHeight;                ///< 行の高さ（倍率）
    bool vertical;                    ///< 縦書きフラグ
    std::string title;                ///< タイトル
    std::string author;               ///< 著者
    std::string description;          ///< 説明
    std::string language;             ///< 言語
    bool embedCss;                    ///< CSSを埋め込むかどうか
    bool embedFonts;                  ///< フォントを埋め込むかどうか
    bool responsive;                  ///< レスポンシブデザインにするかどうか
    bool generateToc;                 ///< 目次を生成するかどうか
    std::string cssPath;              ///< 外部CSSファイルパス（空の場合は生成）
    std::vector<std::string> fontPaths; ///< 外部フォントファイルパス
    std::map<std::string, std::string> extraOptions; ///< その他のオプション
};

/**
 * @class HtmlOutputEngine
 * @brief HTML出力エンジンを実装するクラス
 */
class HtmlOutputEngine {
public:
    /**
     * @brief コンストラクタ
     */
    HtmlOutputEngine();

    /**
     * @brief デストラクタ
     */
    ~HtmlOutputEngine();

    /**
     * @brief 文書をHTMLとして出力する
     * @param document 文書
     * @param style スタイル
     * @param outputPath 出力ファイルパス
     * @param options 出力オプション
     * @return 成功した場合はtrue
     */
    bool outputHtml(
        const core::document::Document& document,
        const core::style::Style& style,
        const std::string& outputPath,
        const HtmlOutputOptions& options);

    /**
     * @brief 文書をHTMLとして出力する（複数ファイル）
     * @param document 文書
     * @param style スタイル
     * @param outputDir 出力ディレクトリパス
     * @param options 出力オプション
     * @return 成功した場合はtrue
     */
    bool outputHtmlMultiFile(
        const core::document::Document& document,
        const core::style::Style& style,
        const std::string& outputDir,
        const HtmlOutputOptions& options);

private:
    /**
     * @brief HTMLドキュメントを生成する
     * @param document 文書
     * @param style スタイル
     * @param options 出力オプション
     * @return HTML内容
     */
    std::string generateHtml(
        const core::document::Document& document,
        const core::style::Style& style,
        const HtmlOutputOptions& options);

    /**
     * @brief CSSスタイルシートを生成する
     * @param style スタイル
     * @param options 出力オプション
     * @return CSS内容
     */
    std::string generateCss(
        const core::style::Style& style,
        const HtmlOutputOptions& options);

    /**
     * @brief 目次を生成する
     * @param document 文書
     * @param options 出力オプション
     * @return HTML目次内容
     */
    std::string generateToc(
        const core::document::Document& document,
        const HtmlOutputOptions& options);

    /**
     * @brief 組版結果をHTMLに変換する
     * @param blocks 組版されたテキストブロックのリスト
     * @param options 出力オプション
     * @return HTML内容
     */
    std::string blocksToHtml(
        const std::vector<core::typesetting::TextBlock>& blocks,
        const HtmlOutputOptions& options);

    /**
     * @brief フォントをBase64エンコードする
     * @param fontPath フォントファイルパス
     * @return Base64エンコードされたフォントデータ
     */
    std::string encodeFont(const std::string& fontPath);

    /**
     * @brief 複数ファイル出力用のHTMLを生成する
     * @param document 文書
     * @param style スタイル
     * @param options 出力オプション
     * @return HTMLファイルのマップ（相対パス -> 内容）
     */
    std::map<std::string, std::string> generateMultiFileHtml(
        const core::document::Document& document,
        const core::style::Style& style,
        const HtmlOutputOptions& options);

    core::typesetting::TypesettingEngine m_typesettingEngine; ///< 組版エンジン
};

} // namespace output
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_OUTPUT_HTML_OUTPUT_H
