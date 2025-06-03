/**
 * @file pdf_output.h
 * @brief PDF出力エンジンの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_OUTPUT_PDF_OUTPUT_H
#define JAPANESE_TYPESETTING_OUTPUT_PDF_OUTPUT_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <string>
#include <vector>
#include <map>

namespace japanese_typesetting {
namespace output {

/**
 * @struct PdfOutputOptions
 * @brief PDF出力オプションを表す構造体
 */
struct PdfOutputOptions {
    double pageWidth;                 ///< ページ幅（mm）
    double pageHeight;                ///< ページ高さ（mm）
    double marginTop;                 ///< 上マージン（mm）
    double marginBottom;              ///< 下マージン（mm）
    double marginLeft;                ///< 左マージン（mm）
    double marginRight;               ///< 右マージン（mm）
    std::string fontFamily;           ///< フォントファミリー
    double fontSize;                  ///< フォントサイズ（pt）
    double lineHeight;                ///< 行の高さ（倍率）
    bool vertical;                    ///< 縦書きフラグ
    std::string author;               ///< 著者
    std::string title;                ///< タイトル
    std::string subject;              ///< 件名
    std::string keywords;             ///< キーワード
    std::string creator;              ///< 作成者
    std::map<std::string, std::string> extraOptions; ///< その他のオプション
};

/**
 * @class PdfOutputEngine
 * @brief PDF出力エンジンを実装するクラス
 */
class PdfOutputEngine {
public:
    /**
     * @brief コンストラクタ
     */
    PdfOutputEngine();

    /**
     * @brief デストラクタ
     */
    ~PdfOutputEngine();

    /**
     * @brief 文書をPDFとして出力する
     * @param document 文書
     * @param style スタイル
     * @param outputPath 出力ファイルパス
     * @param options 出力オプション
     * @return 成功した場合はtrue
     */
    bool outputPdf(
        const core::document::Document& document,
        const core::style::Style& style,
        const std::string& outputPath,
        const PdfOutputOptions& options);

private:
    /**
     * @brief WeasyPrintを使用してPDFを生成する
     * @param htmlContent HTML内容
     * @param cssContent CSS内容
     * @param outputPath 出力ファイルパス
     * @param options 出力オプション
     * @return 成功した場合はtrue
     */
    bool generatePdfWithWeasyPrint(
        const std::string& htmlContent,
        const std::string& cssContent,
        const std::string& outputPath,
        const PdfOutputOptions& options);

    /**
     * @brief 文書からHTMLを生成する
     * @param document 文書
     * @param style スタイル
     * @param options 出力オプション
     * @return HTML内容
     */
    std::string generateHtml(
        const core::document::Document& document,
        const core::style::Style& style,
        const PdfOutputOptions& options);

    /**
     * @brief スタイルからCSSを生成する
     * @param style スタイル
     * @param options 出力オプション
     * @return CSS内容
     */
    std::string generateCss(
        const core::style::Style& style,
        const PdfOutputOptions& options);

    /**
     * @brief 組版結果をHTMLに変換する
     * @param blocks 組版されたテキストブロックのリスト
     * @param options 出力オプション
     * @return HTML内容
     */
    std::string blocksToHtml(
        const std::vector<core::typesetting::TextBlock>& blocks,
        const PdfOutputOptions& options);

    /**
     * @brief 一時ファイルを作成する
     * @param content ファイル内容
     * @param extension ファイル拡張子
     * @return 一時ファイルパス
     */
    std::string createTempFile(
        const std::string& content,
        const std::string& extension);

    /**
     * @brief 一時ファイルを削除する
     * @param filePath ファイルパス
     */
    void removeTempFile(const std::string& filePath);

    core::typesetting::TypesettingEngine m_typesettingEngine; ///< 組版エンジン
};

} // namespace output
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_OUTPUT_PDF_OUTPUT_H
