/**
 * @file epub_output.h
 * @brief EPUB出力エンジンの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_OUTPUT_EPUB_OUTPUT_H
#define JAPANESE_TYPESETTING_OUTPUT_EPUB_OUTPUT_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <string>
#include <vector>
#include <map>

namespace japanese_typesetting {
namespace output {

/**
 * @struct EpubOutputOptions
 * @brief EPUB出力オプションを表す構造体
 */
struct EpubOutputOptions {
    std::string fontFamily;           ///< フォントファミリー
    double fontSize;                  ///< フォントサイズ（pt）
    double lineHeight;                ///< 行の高さ（倍率）
    bool vertical;                    ///< 縦書きフラグ
    std::string author;               ///< 著者
    std::string title;                ///< タイトル
    std::string publisher;            ///< 出版社
    std::string language;             ///< 言語
    std::string identifier;           ///< 識別子
    std::string description;          ///< 説明
    std::string rights;               ///< 権利
    std::string coverImagePath;       ///< 表紙画像パス
    std::map<std::string, std::string> extraMetadata; ///< その他のメタデータ
};

/**
 * @class EpubOutputEngine
 * @brief EPUB出力エンジンを実装するクラス
 */
class EpubOutputEngine {
public:
    /**
     * @brief コンストラクタ
     */
    EpubOutputEngine();

    /**
     * @brief デストラクタ
     */
    ~EpubOutputEngine();

    /**
     * @brief 文書をEPUBとして出力する
     * @param document 文書
     * @param style スタイル
     * @param outputPath 出力ファイルパス
     * @param options 出力オプション
     * @return 成功した場合はtrue
     */
    bool outputEpub(
        const core::document::Document& document,
        const core::style::Style& style,
        const std::string& outputPath,
        const EpubOutputOptions& options);

private:
    /**
     * @brief EPUBファイルを生成する
     * @param contentFiles コンテンツファイルのマップ（相対パス -> 内容）
     * @param outputPath 出力ファイルパス
     * @param options 出力オプション
     * @return 成功した場合はtrue
     */
    bool generateEpub(
        const std::map<std::string, std::string>& contentFiles,
        const std::string& outputPath,
        const EpubOutputOptions& options);

    /**
     * @brief 文書からHTMLコンテンツを生成する
     * @param document 文書
     * @param style スタイル
     * @param options 出力オプション
     * @return HTMLコンテンツファイルのマップ（相対パス -> 内容）
     */
    std::map<std::string, std::string> generateHtmlContent(
        const core::document::Document& document,
        const core::style::Style& style,
        const EpubOutputOptions& options);

    /**
     * @brief CSSスタイルシートを生成する
     * @param style スタイル
     * @param options 出力オプション
     * @return CSS内容
     */
    std::string generateCss(
        const core::style::Style& style,
        const EpubOutputOptions& options);

    /**
     * @brief OPFファイル（Open Packaging Format）を生成する
     * @param contentFiles コンテンツファイルのリスト
     * @param options 出力オプション
     * @return OPF内容
     */
    std::string generateOpf(
        const std::vector<std::string>& contentFiles,
        const EpubOutputOptions& options);

    /**
     * @brief NCXファイル（Navigation Control File）を生成する
     * @param document 文書
     * @param options 出力オプション
     * @return NCX内容
     */
    std::string generateNcx(
        const core::document::Document& document,
        const EpubOutputOptions& options);

    /**
     * @brief コンテナファイル（container.xml）を生成する
     * @return コンテナファイル内容
     */
    std::string generateContainer();

    /**
     * @brief 一時ディレクトリを作成する
     * @return 一時ディレクトリパス
     */
    std::string createTempDir();

    /**
     * @brief 一時ディレクトリを削除する
     * @param dirPath ディレクトリパス
     */
    void removeTempDir(const std::string& dirPath);

    /**
     * @brief 組版結果をHTMLに変換する
     * @param blocks 組版されたテキストブロックのリスト
     * @param options 出力オプション
     * @return HTML内容
     */
    std::string blocksToHtml(
        const std::vector<core::typesetting::TextBlock>& blocks,
        const EpubOutputOptions& options);

    core::typesetting::TypesettingEngine m_typesettingEngine; ///< 組版エンジン
};

} // namespace output
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_OUTPUT_EPUB_OUTPUT_H
