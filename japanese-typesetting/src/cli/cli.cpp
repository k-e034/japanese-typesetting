/**
 * @file cli.cpp
 * @brief コマンドラインインターフェースの詳細実装
 */

#include "japanese_typesetting/cli/cli.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>

namespace japanese_typesetting {
namespace cli {

CommandLineInterface::CommandLineInterface() {
    // 特に初期化処理はない
}

CommandLineInterface::~CommandLineInterface() {
    // 特に何もしない
}

CommandLineOptions CommandLineInterface::parseCommandLine(int argc, char* argv[]) {
    CommandLineOptions options;
    
    // デフォルト値の設定
    options.outputFormat = "pdf";
    options.vertical = true;
    options.pageWidth = 210.0;  // A4
    options.pageHeight = 297.0; // A4
    options.marginTop = 20.0;
    options.marginBottom = 20.0;
    options.marginLeft = 20.0;
    options.marginRight = 20.0;
    options.fontFamily = "Mincho";
    options.fontSize = 10.5;
    options.lineHeight = 1.5;
    options.verbose = false;
    options.help = false;
    options.version = false;
    
    // 引数の解析
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            options.help = true;
        } else if (arg == "-v" || arg == "--version") {
            options.version = true;
        } else if (arg == "--verbose") {
            options.verbose = true;
        } else if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) {
                options.inputFile = argv[++i];
            } else {
                showError("入力ファイルが指定されていません");
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                options.outputFile = argv[++i];
            } else {
                showError("出力ファイルが指定されていません");
            }
        } else if (arg == "-f" || arg == "--format") {
            if (i + 1 < argc) {
                options.outputFormat = argv[++i];
                if (options.outputFormat != "pdf" && options.outputFormat != "epub" && options.outputFormat != "html") {
                    showError("無効な出力フォーマットです: " + options.outputFormat);
                    options.outputFormat = "pdf"; // デフォルトに戻す
                }
            } else {
                showError("出力フォーマットが指定されていません");
            }
        } else if (arg == "-s" || arg == "--style") {
            if (i + 1 < argc) {
                options.styleFile = argv[++i];
            } else {
                showError("スタイルファイルが指定されていません");
            }
        } else if (arg == "--horizontal") {
            options.vertical = false;
        } else if (arg == "--vertical") {
            options.vertical = true;
        } else if (arg == "--page-width") {
            if (i + 1 < argc) {
                options.pageWidth = std::stod(argv[++i]);
            } else {
                showError("ページ幅が指定されていません");
            }
        } else if (arg == "--page-height") {
            if (i + 1 < argc) {
                options.pageHeight = std::stod(argv[++i]);
            } else {
                showError("ページ高さが指定されていません");
            }
        } else if (arg == "--margin-top") {
            if (i + 1 < argc) {
                options.marginTop = std::stod(argv[++i]);
            } else {
                showError("上マージンが指定されていません");
            }
        } else if (arg == "--margin-bottom") {
            if (i + 1 < argc) {
                options.marginBottom = std::stod(argv[++i]);
            } else {
                showError("下マージンが指定されていません");
            }
        } else if (arg == "--margin-left") {
            if (i + 1 < argc) {
                options.marginLeft = std::stod(argv[++i]);
            } else {
                showError("左マージンが指定されていません");
            }
        } else if (arg == "--margin-right") {
            if (i + 1 < argc) {
                options.marginRight = std::stod(argv[++i]);
            } else {
                showError("右マージンが指定されていません");
            }
        } else if (arg == "--font-family") {
            if (i + 1 < argc) {
                options.fontFamily = argv[++i];
            } else {
                showError("フォントファミリーが指定されていません");
            }
        } else if (arg == "--font-size") {
            if (i + 1 < argc) {
                options.fontSize = std::stod(argv[++i]);
            } else {
                showError("フォントサイズが指定されていません");
            }
        } else if (arg == "--line-height") {
            if (i + 1 < argc) {
                options.lineHeight = std::stod(argv[++i]);
            } else {
                showError("行の高さが指定されていません");
            }
        } else if (arg.substr(0, 2) == "--") {
            // カスタムオプション
            std::string key = arg.substr(2);
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                options.extraOptions[key] = argv[++i];
            } else {
                options.extraOptions[key] = "true";
            }
        } else if (options.inputFile.empty()) {
            // 最初の非オプション引数は入力ファイルとして扱う
            options.inputFile = arg;
        } else if (options.outputFile.empty()) {
            // 2番目の非オプション引数は出力ファイルとして扱う
            options.outputFile = arg;
        } else {
            showError("無効な引数です: " + arg);
        }
    }
    
    return options;
}

void CommandLineInterface::showHelp() const {
    std::cout << "使用法: japanese-typesetting [オプション] 入力ファイル [出力ファイル]" << std::endl;
    std::cout << std::endl;
    std::cout << "オプション:" << std::endl;
    std::cout << "  -h, --help                 このヘルプメッセージを表示して終了" << std::endl;
    std::cout << "  -v, --version              バージョン情報を表示して終了" << std::endl;
    std::cout << "  --verbose                  詳細な出力を表示" << std::endl;
    std::cout << "  -i, --input FILE           入力ファイルを指定" << std::endl;
    std::cout << "  -o, --output FILE          出力ファイルを指定" << std::endl;
    std::cout << "  -f, --format FORMAT        出力フォーマットを指定 (pdf, epub, html)" << std::endl;
    std::cout << "  -s, --style FILE           スタイルファイルを指定" << std::endl;
    std::cout << "  --horizontal               横書きモードを使用" << std::endl;
    std::cout << "  --vertical                 縦書きモードを使用（デフォルト）" << std::endl;
    std::cout << "  --page-width WIDTH         ページ幅をmmで指定（デフォルト: 210.0）" << std::endl;
    std::cout << "  --page-height HEIGHT       ページ高さをmmで指定（デフォルト: 297.0）" << std::endl;
    std::cout << "  --margin-top SIZE          上マージンをmmで指定（デフォルト: 20.0）" << std::endl;
    std::cout << "  --margin-bottom SIZE       下マージンをmmで指定（デフォルト: 20.0）" << std::endl;
    std::cout << "  --margin-left SIZE         左マージンをmmで指定（デフォルト: 20.0）" << std::endl;
    std::cout << "  --margin-right SIZE        右マージンをmmで指定（デフォルト: 20.0）" << std::endl;
    std::cout << "  --font-family FAMILY       フォントファミリーを指定（デフォルト: Mincho）" << std::endl;
    std::cout << "  --font-size SIZE           フォントサイズをptで指定（デフォルト: 10.5）" << std::endl;
    std::cout << "  --line-height HEIGHT       行の高さを倍率で指定（デフォルト: 1.5）" << std::endl;
    std::cout << std::endl;
    std::cout << "例:" << std::endl;
    std::cout << "  japanese-typesetting input.txt output.pdf" << std::endl;
    std::cout << "  japanese-typesetting -f html --horizontal input.txt output.html" << std::endl;
    std::cout << "  japanese-typesetting --font-size 12 --line-height 1.8 input.txt" << std::endl;
}

void CommandLineInterface::showVersion() const {
    std::cout << "Japanese Typesetting Software バージョン 0.1.0" << std::endl;
    std::cout << "Copyright (C) 2025 Japanese Typesetting Project" << std::endl;
}

int CommandLineInterface::run(const CommandLineOptions& options) {
    // ヘルプまたはバージョン情報の表示
    if (options.help) {
        showHelp();
        return 0;
    }
    
    if (options.version) {
        showVersion();
        return 0;
    }
    
    // 入力ファイルのチェック
    if (options.inputFile.empty()) {
        showError("入力ファイルが指定されていません");
        showHelp();
        return 1;
    }
    
    // 出力ファイルのチェック
    if (options.outputFile.empty()) {
        // 出力ファイル名を入力ファイル名から生成
        std::string baseName = options.inputFile;
        size_t lastDot = baseName.find_last_of('.');
        if (lastDot != std::string::npos) {
            baseName = baseName.substr(0, lastDot);
        }
        
        // 出力フォーマットに応じた拡張子を追加
        if (options.outputFormat == "pdf") {
            options.outputFile = baseName + ".pdf";
        } else if (options.outputFormat == "epub") {
            options.outputFile = baseName + ".epub";
        } else if (options.outputFormat == "html") {
            options.outputFile = baseName + ".html";
        }
    }
    
    try {
        // 文書の読み込み
        if (options.verbose) {
            showInfo("文書を読み込んでいます: " + options.inputFile);
        }
        core::document::Document document = loadDocument(options.inputFile);
        
        // スタイルの読み込み
        core::style::Style style;
        if (!options.styleFile.empty()) {
            if (options.verbose) {
                showInfo("スタイルを読み込んでいます: " + options.styleFile);
            }
            style = loadStyle(options.styleFile);
        } else {
            // デフォルトスタイルの設定
            style.setFontFamily(options.fontFamily);
            style.setFontSize(options.fontSize);
            style.setLineHeight(options.lineHeight);
        }
        
        // 文書の組版
        if (options.verbose) {
            showInfo("文書を組版しています...");
        }
        std::vector<core::typesetting::TextBlock> blocks = typesetDocument(document, style, options);
        
        // 結果の出力
        if (options.verbose) {
            showInfo("結果を出力しています: " + options.outputFile);
        }
        if (!outputResult(blocks, options)) {
            showError("結果の出力に失敗しました");
            return 1;
        }
        
        if (options.verbose) {
            showInfo("処理が完了しました");
        }
        
        return 0;
    } catch (const std::exception& e) {
        showError("エラーが発生しました: " + std::string(e.what()));
        return 1;
    }
}

core::document::Document CommandLineInterface::loadDocument(const std::string& filePath) {
    core::document::Document document;
    
    // ファイルの読み込み
    if (!document.loadFromFile(filePath)) {
        throw std::runtime_error("文書の読み込みに失敗しました: " + filePath);
    }
    
    return document;
}

core::style::Style CommandLineInterface::loadStyle(const std::string& filePath) {
    core::style::Style style;
    
    // ファイルの読み込み
    if (!style.loadFromFile(filePath)) {
        throw std::runtime_error("スタイルの読み込みに失敗しました: " + filePath);
    }
    
    return style;
}

std::vector<core::typesetting::TextBlock> CommandLineInterface::typesetDocument(
    const core::document::Document& document,
    const core::style::Style& style,
    const CommandLineOptions& options) {
    
    // 組版エンジンの作成
    core::typesetting::TypesettingEngine engine;
    
    // 組版処理
    double contentWidth = options.pageWidth - options.marginLeft - options.marginRight;
    return engine.typesetDocument(document, style, contentWidth);
}

bool CommandLineInterface::outputResult(
    const std::vector<core::typesetting::TextBlock>& blocks,
    const CommandLineOptions& options) {
    
    // 出力フォーマットに応じた処理
    // 実際の実装では、各出力エンジンを呼び出す
    
    // 現時点では簡易的な実装として、テキスト出力のみを行う
    std::ofstream outFile(options.outputFile);
    if (!outFile.is_open()) {
        return false;
    }
    
    outFile << "Japanese Typesetting Output" << std::endl;
    outFile << "===========================" << std::endl;
    outFile << std::endl;
    
    for (const auto& block : blocks) {
        for (const auto& line : block.lines) {
            // UTF-32からUTF-8に変換して出力
            core::unicode::UnicodeHandler unicodeHandler;
            std::string utf8Text = unicodeHandler.utf32ToUtf8(line.text);
            outFile << utf8Text << std::endl;
        }
        outFile << std::endl;
    }
    
    outFile.close();
    return true;
}

void CommandLineInterface::showError(const std::string& message) const {
    std::cerr << "エラー: " << message << std::endl;
}

void CommandLineInterface::showInfo(const std::string& message) const {
    std::cout << "情報: " << message << std::endl;
}

} // namespace cli
} // namespace japanese_typesetting
