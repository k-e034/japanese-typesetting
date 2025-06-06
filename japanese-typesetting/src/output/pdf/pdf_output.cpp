/**
 * @file pdf_output.cpp
 * @brief PDF出力エンジンの詳細実装
 */

#include "japanese_typesetting/output/pdf_output.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <Python.h>
#include <iostream>

namespace japanese_typesetting {
namespace output {

PdfOutputEngine::PdfOutputEngine() {
    // 特に初期化処理はない
}

PdfOutputEngine::~PdfOutputEngine() {
    // 特に何もしない
}

bool PdfOutputEngine::outputPdf(
    const core::document::Document& document,
    const core::style::Style& style,
    const std::string& outputPath,
    const PdfOutputOptions& options) {
    
    try {
        // 文書の組版
        double contentWidth = options.pageWidth - options.marginLeft - options.marginRight;
        std::vector<core::typesetting::TextBlock> blocks = m_typesettingEngine.typesetDocument(document, style, contentWidth);
        
        // HTMLとCSSの生成
        std::string htmlContent = generateHtml(document, style, options);
        std::string cssContent = generateCss(style, options);
        
        // WeasyPrintを使用してPDFを生成
        return generatePdfWithWeasyPrint(htmlContent, cssContent, outputPath, options);
    } catch (const std::exception& e) {
        // エラー処理
        std::cerr << "PDF出力中にエラーが発生しました: " << e.what() << std::endl;
        return false;
    }
}

bool PdfOutputEngine::generatePdfWithWeasyPrint(
    const std::string& htmlContent,
    const std::string& cssContent,
    const std::string& outputPath,
    const PdfOutputOptions& options) {
    
    // 一時ファイルの作成
    std::string htmlFilePath = createTempFile(htmlContent, ".html");
    std::string cssFilePath = createTempFile(cssContent, ".css");
    
    try {
        // Pythonインタプリタの初期化
        Py_Initialize();
        
        // WeasyPrintを使用するPythonコード
        std::string pythonCode = R"(
import sys
from weasyprint import HTML, CSS

try:
    # HTMLとCSSファイルの読み込み
    html = HTML(filename=sys.argv[1])
    css = CSS(filename=sys.argv[2])
    
    # メタデータの設定
    metadata = {
        'title': sys.argv[4] if len(sys.argv) > 4 else '',
        'author': sys.argv[5] if len(sys.argv) > 5 else '',
        'subject': sys.argv[6] if len(sys.argv) > 6 else '',
        'keywords': sys.argv[7] if len(sys.argv) > 7 else '',
        'creator': sys.argv[8] if len(sys.argv) > 8 else '',
    }
    
    # PDFの生成
    html.write_pdf(sys.argv[3], stylesheets=[css], metadata=metadata)
    print('PDF生成が完了しました')
    sys.exit(0)
except Exception as e:
    print(f'エラーが発生しました: {e}', file=sys.stderr)
    sys.exit(1)
)";
        
        // 一時Pythonスクリプトファイルの作成
        std::string pythonFilePath = createTempFile(pythonCode, ".py");
        
        // コマンドの構築
        std::ostringstream cmdStream;
        cmdStream << "python3 " << pythonFilePath
                 << " " << htmlFilePath
                 << " " << cssFilePath
                 << " " << outputPath
                 << " \"" << options.title << "\""
                 << " \"" << options.author << "\""
                 << " \"" << options.subject << "\""
                 << " \"" << options.keywords << "\""
                 << " \"" << options.creator << "\"";
        
        // コマンドの実行
        int result = std::system(cmdStream.str().c_str());
        
        // 一時ファイルの削除
        removeTempFile(htmlFilePath);
        removeTempFile(cssFilePath);
        removeTempFile(pythonFilePath);
        
        // Pythonインタプリタの終了
        Py_Finalize();
        
        return result == 0;
    } catch (const std::exception& e) {
        // エラー処理
        std::cerr << "WeasyPrintによるPDF生成中にエラーが発生しました: " << e.what() << std::endl;
        
        // 一時ファイルの削除
        removeTempFile(htmlFilePath);
        removeTempFile(cssFilePath);
        
        // Pythonインタプリタの終了
        Py_Finalize();
        
        return false;
    }
}

std::string PdfOutputEngine::generateHtml(
    const core::document::Document& document,
    const core::style::Style& style,
    const PdfOutputOptions& options) {
    
    // 文書の組版
    double contentWidth = options.pageWidth - options.marginLeft - options.marginRight;
    std::vector<core::typesetting::TextBlock> blocks = m_typesettingEngine.typesetDocument(document, style, contentWidth);
    
    // HTMLの生成
    std::ostringstream html;
    
    html << "<!DOCTYPE html>\n"
         << "<html lang=\"ja\">\n"
         << "<head>\n"
         << "  <meta charset=\"UTF-8\">\n"
         << "  <title>" << options.title << "</title>\n"
         << "  <link rel=\"stylesheet\" href=\"style.css\">\n"
         << "</head>\n"
         << "<body class=\"" << (options.vertical ? "vertical" : "horizontal") << "\">\n";
    
    // 文書内容の変換
    html << blocksToHtml(blocks, options);
    
    html << "</body>\n"
         << "</html>\n";
    
    return html.str();
}

std::string PdfOutputEngine::generateCss(
    const core::style::Style& style,
    const PdfOutputOptions& options) {
    
    // CSSの生成
    std::ostringstream css;
    
    // ページ設定
    css << "@page {\n"
        << "  size: " << options.pageWidth << "mm " << options.pageHeight << "mm;\n"
        << "  margin: " << options.marginTop << "mm " << options.marginRight << "mm "
        << options.marginBottom << "mm " << options.marginLeft << "mm;\n"
        << "}\n\n";
    
    // 基本スタイル
    css << "body {\n"
        << "  font-family: \"" << options.fontFamily << "\", \"Noto Sans CJK JP\", \"WenQuanYi Zen Hei\", sans-serif;\n"
        << "  font-size: " << options.fontSize << "pt;\n"
        << "  line-height: " << options.lineHeight << ";\n"
        << "  margin: 0;\n"
        << "  padding: 0;\n"
        << "}\n\n";
    
    // 縦書きモード
    css << "body.vertical {\n"
        << "  writing-mode: vertical-rl;\n"
        << "  text-orientation: upright;\n"
        << "  -webkit-writing-mode: vertical-rl;\n"
        << "  -ms-writing-mode: tb-rl;\n"
        << "}\n\n";
    
    // 横書きモード
    css << "body.horizontal {\n"
        << "  writing-mode: horizontal-tb;\n"
        << "  -webkit-writing-mode: horizontal-tb;\n"
        << "  -ms-writing-mode: lr-tb;\n"
        << "}\n\n";
    
    // 段落スタイル
    css << "p {\n"
        << "  margin: 0;\n"
        << "  padding: 0;\n"
        << "  text-align: justify;\n"
        << "}\n\n";
    
    // 見出しスタイル
    css << "h1, h2, h3, h4, h5, h6 {\n"
        << "  font-weight: bold;\n"
        << "  margin: 1em 0 0.5em 0;\n"
        << "}\n\n";
    
    // ルビスタイル
    css << "ruby {\n"
        << "  ruby-align: center;\n"
        << "}\n\n";
    
    // 文字間隔
    css << "span.char {\n"
        << "  letter-spacing: " << style.getCharacterSpacing() << "em;\n"
        << "}\n\n";
    
    return css.str();
}

std::string PdfOutputEngine::blocksToHtml(
    const std::vector<core::typesetting::TextBlock>& blocks,
    const PdfOutputOptions& options) {
    
    std::ostringstream html;
    core::unicode::UnicodeHandler unicodeHandler;
    
    // 各ブロックをHTML要素に変換
    for (const auto& block : blocks) {
        html << "<div class=\"block\">\n";
        
        for (const auto& line : block.lines) {
            html << "  <p>";
            
            // UTF-32からUTF-8に変換
            std::string utf8Text = unicodeHandler.utf32ToUtf8(line.text);
            
            // 特殊文字のエスケープ
            for (char c : utf8Text) {
                switch (c) {
                    case '<': html << "&lt;"; break;
                    case '>': html << "&gt;"; break;
                    case '&': html << "&amp;"; break;
                    case '"': html << "&quot;"; break;
                    case '\'': html << "&#39;"; break;
                    default: html << c;
                }
            }
            
            html << "</p>\n";
        }
        
        html << "</div>\n";
    }
    
    return html.str();
}

std::string PdfOutputEngine::createTempFile(
    const std::string& content,
    const std::string& extension) {
    
    // 一時ファイル名の生成
    std::string tempDir = std::filesystem::temp_directory_path().string();
    std::string tempFileName = tempDir + "/japanese_typesetting_" + 
                              std::to_string(std::rand()) + extension;
    
    // ファイルの作成
    std::ofstream file(tempFileName);
    if (!file.is_open()) {
        throw std::runtime_error("一時ファイルの作成に失敗しました: " + tempFileName);
    }
    
    file << content;
    file.close();
    
    return tempFileName;
}

void PdfOutputEngine::removeTempFile(const std::string& filePath) {
    // ファイルの削除
    std::remove(filePath.c_str());
}

} // namespace output
} // namespace japanese_typesetting
