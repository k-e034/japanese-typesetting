/**
 * @file output_format_switcher.h
 * @brief 出力フォーマット切り替え機能の実装
 */

#ifndef JAPANESE_TYPESETTING_OUTPUT_FORMAT_SWITCHER_H
#define JAPANESE_TYPESETTING_OUTPUT_FORMAT_SWITCHER_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/output/pdf_output.h"
#include "japanese_typesetting/output/epub_output.h"
#include "japanese_typesetting/output/html_output.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace japanese_typesetting {
namespace output {

/**
 * @enum OutputFormat
 * @brief 出力フォーマットの種類を表す列挙型
 */
enum class OutputFormat {
    PDF,    ///< PDF形式
    EPUB,   ///< EPUB形式
    HTML,   ///< HTML形式（単一ファイル）
    HTML_MULTI, ///< HTML形式（複数ファイル）
    CUSTOM  ///< カスタム形式（プラグイン拡張用）
};

/**
 * @struct OutputOptions
 * @brief 出力オプションの基底構造体
 */
struct OutputOptions {
    std::string fontFamily;           ///< フォントファミリー
    double fontSize;                  ///< フォントサイズ（pt）
    double lineHeight;                ///< 行の高さ（倍率）
    bool vertical;                    ///< 縦書きフラグ
    std::string title;                ///< タイトル
    std::string author;               ///< 著者
    std::map<std::string, std::string> extraOptions; ///< その他のオプション
};

/**
 * @class OutputFormatSwitcher
 * @brief 出力フォーマット切り替え機能を提供するクラス
 */
class OutputFormatSwitcher {
public:
    /**
     * @brief シングルトンインスタンスを取得する
     * @return OutputFormatSwitcherのインスタンス
     */
    static OutputFormatSwitcher& getInstance();

    /**
     * @brief 文書を指定された形式で出力する
     * @param document 文書
     * @param style スタイル
     * @param outputPath 出力ファイルパス
     * @param format 出力フォーマット
     * @param options 出力オプション
     * @return 成功した場合はtrue
     */
    bool outputDocument(
        const core::document::Document& document,
        const core::style::Style& style,
        const std::string& outputPath,
        OutputFormat format,
        const OutputOptions& options);

    /**
     * @brief 利用可能な出力フォーマットの一覧を取得する
     * @return 出力フォーマットの一覧
     */
    std::vector<OutputFormat> getAvailableFormats() const;

    /**
     * @brief 出力フォーマットの名前を取得する
     * @param format 出力フォーマット
     * @return フォーマット名
     */
    std::string getFormatName(OutputFormat format) const;

    /**
     * @brief 出力フォーマットの説明を取得する
     * @param format 出力フォーマット
     * @return フォーマットの説明
     */
    std::string getFormatDescription(OutputFormat format) const;

    /**
     * @brief 出力フォーマットの拡張子を取得する
     * @param format 出力フォーマット
     * @return ファイル拡張子
     */
    std::string getFormatExtension(OutputFormat format) const;

    /**
     * @brief カスタム出力フォーマットを登録する
     * @param name フォーマット名
     * @param description フォーマットの説明
     * @param extension ファイル拡張子
     * @param outputFunc 出力関数
     * @return 登録されたフォーマット
     */
    OutputFormat registerCustomFormat(
        const std::string& name,
        const std::string& description,
        const std::string& extension,
        std::function<bool(const core::document::Document&, const core::style::Style&, const std::string&, const OutputOptions&)> outputFunc);

    /**
     * @brief カスタム出力フォーマットを登録解除する
     * @param format 出力フォーマット
     * @return 成功した場合はtrue
     */
    bool unregisterCustomFormat(OutputFormat format);

private:
    /**
     * @brief コンストラクタ
     */
    OutputFormatSwitcher();

    /**
     * @brief デストラクタ
     */
    ~OutputFormatSwitcher();

    /**
     * @brief PDF出力オプションを変換する
     * @param options 基本出力オプション
     * @return PDF出力オプション
     */
    PDFOutputOptions convertToPDFOptions(const OutputOptions& options) const;

    /**
     * @brief EPUB出力オプションを変換する
     * @param options 基本出力オプション
     * @return EPUB出力オプション
     */
    EpubOutputOptions convertToEpubOptions(const OutputOptions& options) const;

    /**
     * @brief HTML出力オプションを変換する
     * @param options 基本出力オプション
     * @return HTML出力オプション
     */
    HtmlOutputOptions convertToHtmlOptions(const OutputOptions& options) const;

    PDFOutputEngine m_pdfEngine;      ///< PDF出力エンジン
    EpubOutputEngine m_epubEngine;    ///< EPUB出力エンジン
    HtmlOutputEngine m_htmlEngine;    ///< HTML出力エンジン

    struct CustomFormatInfo {
        std::string name;             ///< フォーマット名
        std::string description;      ///< フォーマットの説明
        std::string extension;        ///< ファイル拡張子
        std::function<bool(const core::document::Document&, const core::style::Style&, const std::string&, const OutputOptions&)> outputFunc; ///< 出力関数
    };

    std::map<OutputFormat, CustomFormatInfo> m_customFormats; ///< カスタムフォーマット情報
    OutputFormat m_nextCustomFormatId;  ///< 次のカスタムフォーマットID
};

} // namespace output
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_OUTPUT_FORMAT_SWITCHER_H
