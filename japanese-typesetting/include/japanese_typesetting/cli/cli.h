/**
 * @file cli.h
 * @brief コマンドラインインターフェースの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_CLI_CLI_H
#define JAPANESE_TYPESETTING_CLI_CLI_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <string>
#include <vector>
#include <map>

namespace japanese_typesetting {
namespace cli {

/**
 * @struct CommandLineOptions
 * @brief コマンドラインオプションを表す構造体
 */
struct CommandLineOptions {
    std::string inputFile;            ///< 入力ファイルパス
    std::string outputFile;           ///< 出力ファイルパス
    std::string outputFormat;         ///< 出力フォーマット（pdf, epub, html）
    std::string styleFile;            ///< スタイルファイルパス
    bool vertical;                    ///< 縦書きフラグ
    double pageWidth;                 ///< ページ幅（mm）
    double pageHeight;                ///< ページ高さ（mm）
    double marginTop;                 ///< 上マージン（mm）
    double marginBottom;              ///< 下マージン（mm）
    double marginLeft;                ///< 左マージン（mm）
    double marginRight;               ///< 右マージン（mm）
    std::string fontFamily;           ///< フォントファミリー
    double fontSize;                  ///< フォントサイズ（pt）
    double lineHeight;                ///< 行の高さ（倍率）
    bool verbose;                     ///< 詳細出力フラグ
    bool help;                        ///< ヘルプ表示フラグ
    bool version;                     ///< バージョン表示フラグ
    std::map<std::string, std::string> extraOptions; ///< その他のオプション
};

/**
 * @class CommandLineInterface
 * @brief コマンドラインインターフェースを実装するクラス
 */
class CommandLineInterface {
public:
    /**
     * @brief コンストラクタ
     */
    CommandLineInterface();

    /**
     * @brief デストラクタ
     */
    ~CommandLineInterface();

    /**
     * @brief コマンドライン引数を解析する
     * @param argc 引数の数
     * @param argv 引数の配列
     * @return 解析されたオプション
     */
    CommandLineOptions parseCommandLine(int argc, char* argv[]);

    /**
     * @brief ヘルプメッセージを表示する
     */
    void showHelp() const;

    /**
     * @brief バージョン情報を表示する
     */
    void showVersion() const;

    /**
     * @brief メイン処理を実行する
     * @param options コマンドラインオプション
     * @return 成功した場合は0、エラーの場合は非0
     */
    int run(CommandLineOptions options);

private:
    /**
     * @brief 入力ファイルを読み込む
     * @param filePath ファイルパス
     * @return 読み込まれた文書
     */
    core::document::Document loadDocument(const std::string& filePath);

    /**
     * @brief スタイルファイルを読み込む
     * @param filePath ファイルパス
     * @return 読み込まれたスタイル
     */
    core::style::Style loadStyle(const std::string& filePath);

    /**
     * @brief 文書を組版する
     * @param document 文書
     * @param style スタイル
     * @param options コマンドラインオプション
     * @return 組版されたテキストブロックのリスト
     */
    std::vector<core::typesetting::TextBlock> typesetDocument(
        const core::document::Document& document,
        const core::style::Style& style,
        const CommandLineOptions& options);

    /**
     * @brief 組版結果を出力する
     * @param blocks 組版されたテキストブロックのリスト
     * @param options コマンドラインオプション
     * @return 成功した場合はtrue
     */
    bool outputResult(
        const std::vector<core::typesetting::TextBlock>& blocks,
        const CommandLineOptions& options);

    /**
     * @brief エラーメッセージを表示する
     * @param message エラーメッセージ
     */
    void showError(const std::string& message) const;

    /**
     * @brief 情報メッセージを表示する
     * @param message 情報メッセージ
     */
    void showInfo(const std::string& message) const;
};

} // namespace cli
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_CLI_CLI_H
