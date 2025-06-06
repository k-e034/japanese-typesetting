/**
 * @file html_output.cpp
 * @brief HTML出力エンジンの詳細実装
 */

#include "japanese_typesetting/output/html_output.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace japanese_typesetting {
namespace output {

HtmlOutputEngine::HtmlOutputEngine() {
    // 特に初期化処理はない
}

HtmlOutputEngine::~HtmlOutputEngine() {
    // 特に何もしない
}

bool HtmlOutputEngine::outputHtml(
    const core::document::Document& document,
    const core::style::Style& style,
    const std::string& outputPath,
    const HtmlOutputOptions& options) {
    
    try {
        // HTMLの生成
        std::string htmlContent = generateHtml(document, style, options);
        
        // ファイルへの書き込み
        std::ofstream file(outputPath);
        if (!file.is_open()) {
            throw std::runtime_error("HTMLファイルの作成に失敗しました: " + outputPath);
        }
        
        file << htmlContent;
        file.close();
        
        // CSSファイルの生成（埋め込みでない場合）
        if (!options.embedCss && options.cssPath.empty()) {
            std::string cssContent = generateCss(style, options);
            std::string cssPath = std::filesystem::path(outputPath).parent_path().string() + "/style.css";
            
            std::ofstream cssFile(cssPath);
            if (!cssFile.is_open()) {
                throw std::runtime_error("CSSファイルの作成に失敗しました: " + cssPath);
            }
            
            cssFile << cssContent;
            cssFile.close();
        }
        
        return true;
    } catch (const std::exception& e) {
        // エラー処理
        std::cerr << "HTML出力中にエラーが発生しました: " << e.what() << std::endl;
        return false;
    }
}

bool HtmlOutputEngine::outputHtmlMultiFile(
    const core::document::Document& document,
    const core::style::Style& style,
    const std::string& outputDir,
    const HtmlOutputOptions& options) {
    
    try {
        // 出力ディレクトリの作成
        std::filesystem::create_directories(outputDir);
        
        // HTMLファイルの生成
        std::map<std::string, std::string> htmlFiles = generateMultiFileHtml(document, style, options);
        
        // ファイルへの書き込み
        for (const auto& [path, content] : htmlFiles) {
            std::string fullPath = outputDir + "/" + path;
            
            // ディレクトリの作成（必要な場合）
            std::filesystem::path dirPath = std::filesystem::path(fullPath).parent_path();
            std::filesystem::create_directories(dirPath);
            
            // ファイルの書き込み
            std::ofstream file(fullPath);
            if (!file.is_open()) {
                throw std::runtime_error("HTMLファイルの作成に失敗しました: " + fullPath);
            }
            
            file << content;
            file.close();
        }
        
        // CSSファイルの生成
        if (!options.embedCss) {
            std::string cssContent = generateCss(style, options);
            std::string cssPath = outputDir + "/css/style.css";
            
            // CSSディレクトリの作成
            std::filesystem::create_directories(outputDir + "/css");
            
            // ファイルの書き込み
            std::ofstream cssFile(cssPath);
            if (!cssFile.is_open()) {
                throw std::runtime_error("CSSファイルの作成に失敗しました: " + cssPath);
            }
            
            cssFile << cssContent;
            cssFile.close();
        }
        
        // フォントファイルのコピー（埋め込みでない場合）
        if (!options.embedFonts && !options.fontPaths.empty()) {
            // フォントディレクトリの作成
            std::filesystem::create_directories(outputDir + "/fonts");
            
            // フォントファイルのコピー
            for (const auto& fontPath : options.fontPaths) {
                if (std::filesystem::exists(fontPath)) {
                    std::string destPath = outputDir + "/fonts/" + std::filesystem::path(fontPath).filename().string();
                    std::filesystem::copy_file(fontPath, destPath, std::filesystem::copy_options::overwrite_existing);
                }
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        // エラー処理
        std::cerr << "HTML出力中にエラーが発生しました: " << e.what() << std::endl;
        return false;
    }
}

std::string HtmlOutputEngine::generateHtml(
    const core::document::Document& document,
    const core::style::Style& style,
    const HtmlOutputOptions& options) {
    
    // 文書の組版
    double contentWidth = 800.0; // 仮の幅
    std::vector<core::typesetting::TextBlock> blocks = m_typesettingEngine.typesetDocument(document, style, contentWidth);
    
    // HTMLの生成
    std::ostringstream html;
    
    html << "<!DOCTYPE html>\n"
         << "<html lang=\"" << options.language << "\">\n"
         << "<head>\n"
         << "  <meta charset=\"UTF-8\">\n"
         << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
         << "  <title>" << options.title << "</title>\n";
    
    // メタ情報
    html << "  <meta name=\"author\" content=\"" << options.author << "\">\n"
         << "  <meta name=\"description\" content=\"" << options.description << "\">\n";
    
    // CSSの埋め込みまたは参照
    if (options.embedCss) {
        html << "  <style>\n"
             << generateCss(style, options)
             << "  </style>\n";
    } else if (!options.cssPath.empty()) {
        html << "  <link rel=\"stylesheet\" href=\"" << options.cssPath << "\">\n";
    } else {
        html << "  <link rel=\"stylesheet\" href=\"style.css\">\n";
    }
    
    // フォントの埋め込み
    if (options.embedFonts && !options.fontPaths.empty()) {
        html << "  <style>\n";
        
        auto ends_with = [](const std::string& s, const std::string& suf) {
            return s.size() >= suf.size() && std::equal(suf.rbegin(), suf.rend(), s.rbegin());
        };
        for (const auto& fontPath : options.fontPaths) {
            if (std::filesystem::exists(fontPath)) {
                std::string fontName = std::filesystem::path(fontPath).stem().string();
                std::string fontData = encodeFont(fontPath);
                std::string fontFormat;

                if (ends_with(fontPath, ".ttf")) {
                    fontFormat = "truetype";
                } else if (ends_with(fontPath, ".otf")) {
                    fontFormat = "opentype";
                } else if (ends_with(fontPath, ".woff")) {
                    fontFormat = "woff";
                } else if (ends_with(fontPath, ".woff2")) {
                    fontFormat = "woff2";
                } else {
                    continue; // 未対応のフォント形式
                }
                
                html << "    @font-face {\n"
                     << "      font-family: '" << fontName << "';\n"
                     << "      src: url('data:application/font-" << fontFormat << ";base64," << fontData << "') format('" << fontFormat << "');\n"
                     << "      font-weight: normal;\n"
                     << "      font-style: normal;\n"
                     << "    }\n";
            }
        }
        
        html << "  </style>\n";
    }
    
    html << "</head>\n"
         << "<body class=\"" << (options.vertical ? "vertical" : "horizontal") << "\">\n";
    
    // 目次の生成（オプション）
    if (options.generateToc) {
        html << generateToc(document, options);
    }
    
    // 文書内容の変換
    html << "<div class=\"content\">\n"
         << blocksToHtml(blocks, options)
         << "</div>\n";
    
    html << "</body>\n"
         << "</html>\n";
    
    return html.str();
}

std::string HtmlOutputEngine::generateCss(
    const core::style::Style& style,
    const HtmlOutputOptions& options) {
    
    // CSSの生成
    std::ostringstream css;
    
    css << "/* 日本語組版ソフトウェア生成CSS */\n\n"
        << "/* 基本スタイル */\n"
        << "html, body {\n"
        << "  margin: 0;\n"
        << "  padding: 0;\n"
        << "  font-family: " << options.fontFamily << ", sans-serif;\n"
        << "  font-size: " << options.fontSize << "pt;\n"
        << "  line-height: " << options.lineHeight << ";\n"
        << "  color: #333;\n"
        << "  background-color: #fff;\n"
        << "}\n\n";
    
    // レスポンシブデザイン
    if (options.responsive) {
        css << "/* レスポンシブデザイン */\n"
            << "@media screen and (max-width: 768px) {\n"
            << "  html, body {\n"
            << "    font-size: " << (options.fontSize * 0.9) << "pt;\n"
            << "  }\n"
            << "}\n\n"
            << "@media screen and (max-width: 480px) {\n"
            << "  html, body {\n"
            << "    font-size: " << (options.fontSize * 0.8) << "pt;\n"
            << "  }\n"
            << "}\n\n";
    }
    
    // 縦書きモード
    css << "/* 縦書きモード */\n"
        << "body.vertical {\n"
        << "  writing-mode: vertical-rl;\n"
        << "  -webkit-writing-mode: vertical-rl;\n"
        << "  -ms-writing-mode: tb-rl;\n"
        << "  text-orientation: upright;\n"
        << "  height: 100vh;\n"
        << "  overflow-x: auto;\n"
        << "  overflow-y: hidden;\n"
        << "}\n\n";
    
    // 横書きモード
    css << "/* 横書きモード */\n"
        << "body.horizontal {\n"
        << "  writing-mode: horizontal-tb;\n"
        << "  -webkit-writing-mode: horizontal-tb;\n"
        << "  -ms-writing-mode: lr-tb;\n"
        << "  max-width: 800px;\n"
        << "  margin: 0 auto;\n"
        << "  padding: 1em;\n"
        << "}\n\n";
    
    // コンテンツ領域
    css << "/* コンテンツ領域 */\n"
        << ".content {\n"
        << "  margin: 2em;\n"
        << "}\n\n";
    
    // 段落スタイル
    css << "/* 段落スタイル */\n"
        << "p {\n"
        << "  margin: 0;\n"
        << "  padding: 0;\n"
        << "  text-align: justify;\n"
        << "  text-indent: 1em;\n"
        << "}\n\n";
    
    // 見出しスタイル
    css << "/* 見出しスタイル */\n"
        << "h1, h2, h3, h4, h5, h6 {\n"
        << "  font-weight: bold;\n"
        << "  margin: 1em 0 0.5em 0;\n"
        << "  line-height: 1.2;\n"
        << "}\n\n"
        << "h1 {\n"
        << "  font-size: 1.8em;\n"
        << "}\n\n"
        << "h2 {\n"
        << "  font-size: 1.5em;\n"
        << "}\n\n"
        << "h3 {\n"
        << "  font-size: 1.3em;\n"
        << "}\n\n"
        << "h4 {\n"
        << "  font-size: 1.1em;\n"
        << "}\n\n";
    
    // ブロックスタイル
    css << "/* ブロックスタイル */\n"
        << ".block {\n"
        << "  margin: 1em 0;\n"
        << "}\n\n";
    
    // 目次スタイル
    css << "/* 目次スタイル */\n"
        << ".toc {\n"
        << "  margin: 2em 0;\n"
        << "  padding: 1em;\n"
        << "  border: 1px solid #ddd;\n"
        << "  background-color: #f9f9f9;\n"
        << "}\n\n"
        << ".toc h2 {\n"
        << "  margin-top: 0;\n"
        << "}\n\n"
        << ".toc ul {\n"
        << "  list-style-type: none;\n"
        << "  padding-left: 1em;\n"
        << "}\n\n"
        << ".toc li {\n"
        << "  margin: 0.5em 0;\n"
        << "}\n\n"
        << ".toc a {\n"
        << "  text-decoration: none;\n"
        << "  color: #0066cc;\n"
        << "}\n\n"
        << ".toc a:hover {\n"
        << "  text-decoration: underline;\n"
        << "}\n\n";
    
    // ルビスタイル
    css << "/* ルビスタイル */\n"
        << "ruby {\n"
        << "  ruby-align: center;\n"
        << "}\n\n"
        << "rt {\n"
        << "  font-size: 0.5em;\n"
        << "  line-height: 1;\n"
        << "}\n\n";
    
    // 文字間隔
    css << "/* 文字間隔 */\n"
        << ".char {\n"
        << "  letter-spacing: " << style.getCharacterSpacing() << "em;\n"
        << "}\n\n";
    
    // 縦中横
    css << "/* 縦中横 */\n"
        << ".tcy {\n"
        << "  text-combine-upright: all;\n"
        << "  -webkit-text-combine: horizontal;\n"
        << "  -ms-text-combine-horizontal: all;\n"
        << "}\n\n";
    
    // 圏点
    css << "/* 圏点 */\n"
        << ".emphasis {\n"
        << "  text-emphasis: filled circle;\n"
        << "  -webkit-text-emphasis: filled circle;\n"
        << "}\n\n";
    
    // 禁則処理
    css << "/* 禁則処理 */\n"
        << "body {\n"
        << "  word-break: normal;\n"
        << "  overflow-wrap: break-word;\n"
        << "  line-break: strict;\n"
        << "}\n\n";
    
    return css.str();
}

std::string HtmlOutputEngine::generateToc(
    const core::document::Document& document,
    const HtmlOutputOptions& options) {
    
    // 目次の生成
    std::ostringstream toc;
    
    toc << "<div class=\"toc\">\n"
        << "  <h2>目次</h2>\n"
        << "  <ul>\n";
    
    // 実際には文書構造から目次を生成する
    // ここでは仮の目次を生成
    for (int i = 1; i <= 5; i++) {
        toc << "    <li><a href=\"#chapter" << i << "\">第" << i << "章</a></li>\n";
    }
    
    toc << "  </ul>\n"
        << "</div>\n";
    
    return toc.str();
}

std::string HtmlOutputEngine::blocksToHtml(
    const std::vector<core::typesetting::TextBlock>& blocks,
    const HtmlOutputOptions& options) {
    
    std::ostringstream html;
    core::unicode::UnicodeHandler unicodeHandler;
    
    // 各ブロックをHTML要素に変換
    for (const auto& block : blocks) {
        html << "<div class=\"block\">\n";
        
        for (const auto& line : block.lines) {
            html << "  <p>";
            
            // UTF-32からUTF-8に変換
            std::string utf8Text = unicodeHandler.utf32ToUtf8(line.text);
            
            // 特殊文字のエスケープと変換
            for (size_t i = 0; i < utf8Text.length(); i++) {
                char c = utf8Text[i];
                
                // 特殊文字のエスケープ
                switch (c) {
                    case '<': html << "&lt;"; break;
                    case '>': html << "&gt;"; break;
                    case '&': html << "&amp;"; break;
                    case '"': html << "&quot;"; break;
                    case '\'': html << "&#39;"; break;
                    default:
                        // 数字の縦中横変換（縦書きモードの場合）
                        if (options.vertical && std::isdigit(c)) {
                            // 連続する数字を検出
                            size_t start = i;
                            while (i < utf8Text.length() && std::isdigit(utf8Text[i])) {
                                i++;
                            }
                            i--; // ループで再度インクリメントされるため
                            
                            // 縦中横として出力
                            html << "<span class=\"tcy\">" << utf8Text.substr(start, i - start + 1) << "</span>";
                        } else {
                            html << c;
                        }
                }
            }
            
            html << "</p>\n";
        }
        
        html << "</div>\n";
    }
    
    return html.str();
}

std::string HtmlOutputEngine::encodeFont(const std::string& fontPath) {
    // フォントファイルをBase64エンコード
    std::ifstream file(fontPath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("フォントファイルを開けませんでした: " + fontPath);
    }
    
    // ファイルサイズの取得
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // ファイル内容の読み込み
    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();
    
    // Base64エンコード
    static const std::string base64Chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    std::ostringstream result;
    int i = 0;
    int j = 0;
    unsigned char array3[3];
    unsigned char array4[4];
    
    for (size_t i = 0; i < fileSize; i++) {
        array3[j++] = buffer[i];
        if (j == 3) {
            array4[0] = (array3[0] & 0xfc) >> 2;
            array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
            array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
            array4[3] = array3[2] & 0x3f;
            
            for (int k = 0; k < 4; k++) {
                result << base64Chars[array4[k]];
            }
            j = 0;
        }
    }
    
    if (j) {
        for (int k = j; k < 3; k++) {
            array3[k] = '\0';
        }
        
        array4[0] = (array3[0] & 0xfc) >> 2;
        array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
        array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
        
        for (int k = 0; k < j + 1; k++) {
            result << base64Chars[array4[k]];
        }
        
        while (j++ < 3) {
            result << '=';
        }
    }
    
    return result.str();
}

std::map<std::string, std::string> HtmlOutputEngine::generateMultiFileHtml(
    const core::document::Document& document,
    const core::style::Style& style,
    const HtmlOutputOptions& options) {
    
    std::map<std::string, std::string> htmlFiles;
    
    // 文書の組版
    double contentWidth = 800.0; // 仮の幅
    std::vector<core::typesetting::TextBlock> blocks = m_typesettingEngine.typesetDocument(document, style, contentWidth);
    
    // チャプターごとにHTMLファイルを生成
    int chapterCount = 1;
    std::vector<core::typesetting::TextBlock> chapterBlocks;
    
    for (const auto& block : blocks) {
        chapterBlocks.push_back(block);
        
        // 一定数のブロックごとにチャプターを区切る（実際には文書構造に基づいて区切る）
        if (chapterBlocks.size() >= 10 || &block == &blocks.back()) {
            // HTMLの生成
            std::ostringstream html;
            
            html << "<!DOCTYPE html>\n"
                 << "<html lang=\"" << options.language << "\">\n"
                 << "<head>\n"
                 << "  <meta charset=\"UTF-8\">\n"
                 << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                 << "  <title>" << options.title << " - 第" << chapterCount << "章</title>\n";
            
            // メタ情報
            html << "  <meta name=\"author\" content=\"" << options.author << "\">\n"
                 << "  <meta name=\"description\" content=\"" << options.description << "\">\n";
            
            // CSSの埋め込みまたは参照
            if (options.embedCss) {
                html << "  <style>\n"
                     << generateCss(style, options)
                     << "  </style>\n";
            } else {
                html << "  <link rel=\"stylesheet\" href=\"css/style.css\">\n";
            }
            
            html << "</head>\n"
                 << "<body class=\"" << (options.vertical ? "vertical" : "horizontal") << "\">\n";
            
            // ナビゲーション
            html << "<div class=\"navigation\">\n";
            if (chapterCount > 1) {
                html << "  <a href=\"chapter" << (chapterCount - 1) << ".html\">前の章</a> | ";
            }
            html << "  <a href=\"index.html\">目次</a>";
            if (chapterCount < 5) { // 仮のチャプター数
                html << " | <a href=\"chapter" << (chapterCount + 1) << ".html\">次の章</a>";
            }
            html << "\n</div>\n";
            
            // チャプタータイトル
            html << "<h1 id=\"chapter" << chapterCount << "\">第" << chapterCount << "章</h1>\n";
            
            // 文書内容の変換
            html << "<div class=\"content\">\n"
                 << blocksToHtml(chapterBlocks, options)
                 << "</div>\n";
            
            html << "</body>\n"
                 << "</html>\n";
            
            // ファイル名の生成
            std::string fileName = "chapter" + std::to_string(chapterCount) + ".html";
            
            // コンテンツファイルに追加
            htmlFiles[fileName] = html.str();
            
            // チャプターカウントの更新
            chapterCount++;
            chapterBlocks.clear();
        }
    }
    
    // インデックスページの生成
    std::ostringstream indexHtml;
    indexHtml << "<!DOCTYPE html>\n"
              << "<html lang=\"" << options.language << "\">\n"
              << "<head>\n"
              << "  <meta charset=\"UTF-8\">\n"
              << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
              << "  <title>" << options.title << "</title>\n";
    
    // メタ情報
    indexHtml << "  <meta name=\"author\" content=\"" << options.author << "\">\n"
              << "  <meta name=\"description\" content=\"" << options.description << "\">\n";
    
    // CSSの埋め込みまたは参照
    if (options.embedCss) {
        indexHtml << "  <style>\n"
                  << generateCss(style, options)
                  << "  </style>\n";
    } else {
        indexHtml << "  <link rel=\"stylesheet\" href=\"css/style.css\">\n";
    }
    
    indexHtml << "</head>\n"
              << "<body class=\"" << (options.vertical ? "vertical" : "horizontal") << "\">\n";
    
    // タイトルと著者
    indexHtml << "<div class=\"title-page\">\n"
              << "  <h1>" << options.title << "</h1>\n"
              << "  <p class=\"author\">" << options.author << "</p>\n"
              << "</div>\n";
    
    // 目次
    indexHtml << "<div class=\"toc\">\n"
              << "  <h2>目次</h2>\n"
              << "  <ul>\n";
    
    for (int i = 1; i < chapterCount; i++) {
        indexHtml << "    <li><a href=\"chapter" << i << ".html\">第" << i << "章</a></li>\n";
    }
    
    indexHtml << "  </ul>\n"
              << "</div>\n";
    
    indexHtml << "</body>\n"
              << "</html>\n";
    
    htmlFiles["index.html"] = indexHtml.str();
    
    return htmlFiles;
}

} // namespace output
} // namespace japanese_typesetting
