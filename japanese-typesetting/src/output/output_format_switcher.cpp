/**
 * @file output_format_switcher.cpp
 * @brief 出力フォーマット切り替え機能の実装
 */

#include "japanese_typesetting/output/output_format_switcher.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace japanese_typesetting {
namespace output {

// シングルトンインスタンスの取得
OutputFormatSwitcher& OutputFormatSwitcher::getInstance() {
    static OutputFormatSwitcher instance;
    return instance;
}

OutputFormatSwitcher::OutputFormatSwitcher() : m_nextCustomFormatId(OutputFormat::CUSTOM) {
    // 特に初期化処理はない
}

OutputFormatSwitcher::~OutputFormatSwitcher() {
    // 特に終了処理はない
}

bool OutputFormatSwitcher::outputDocument(
    const core::document::Document& document,
    const core::style::Style& style,
    const std::string& outputPath,
    OutputFormat format,
    const OutputOptions& options) {
    
    try {
        switch (format) {
            case OutputFormat::PDF: {
                PDFOutputOptions pdfOptions = convertToPDFOptions(options);
                return m_pdfEngine.outputPDF(document, style, outputPath, pdfOptions);
            }
            
            case OutputFormat::EPUB: {
                EpubOutputOptions epubOptions = convertToEpubOptions(options);
                return m_epubEngine.outputEpub(document, style, outputPath, epubOptions);
            }
            
            case OutputFormat::HTML: {
                HtmlOutputOptions htmlOptions = convertToHtmlOptions(options);
                return m_htmlEngine.outputHtml(document, style, outputPath, htmlOptions);
            }
            
            case OutputFormat::HTML_MULTI: {
                HtmlOutputOptions htmlOptions = convertToHtmlOptions(options);
                return m_htmlEngine.outputHtmlMultiFile(document, style, outputPath, htmlOptions);
            }
            
            default: {
                // カスタムフォーマットの場合
                auto it = m_customFormats.find(format);
                if (it != m_customFormats.end()) {
                    return it->second.outputFunc(document, style, outputPath, options);
                }
                
                throw std::runtime_error("未対応の出力フォーマットです");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "出力中にエラーが発生しました: " << e.what() << std::endl;
        return false;
    }
}

std::vector<OutputFormat> OutputFormatSwitcher::getAvailableFormats() const {
    std::vector<OutputFormat> formats = {
        OutputFormat::PDF,
        OutputFormat::EPUB,
        OutputFormat::HTML,
        OutputFormat::HTML_MULTI
    };
    
    // カスタムフォーマットを追加
    for (const auto& [format, _] : m_customFormats) {
        formats.push_back(format);
    }
    
    return formats;
}

std::string OutputFormatSwitcher::getFormatName(OutputFormat format) const {
    switch (format) {
        case OutputFormat::PDF:
            return "PDF";
        case OutputFormat::EPUB:
            return "EPUB";
        case OutputFormat::HTML:
            return "HTML (単一ファイル)";
        case OutputFormat::HTML_MULTI:
            return "HTML (複数ファイル)";
        default: {
            // カスタムフォーマットの場合
            auto it = m_customFormats.find(format);
            if (it != m_customFormats.end()) {
                return it->second.name;
            }
            return "不明なフォーマット";
        }
    }
}

std::string OutputFormatSwitcher::getFormatDescription(OutputFormat format) const {
    switch (format) {
        case OutputFormat::PDF:
            return "印刷用PDFドキュメント";
        case OutputFormat::EPUB:
            return "電子書籍リーダー用EPUBドキュメント";
        case OutputFormat::HTML:
            return "Webブラウザ用HTML（単一ファイル）";
        case OutputFormat::HTML_MULTI:
            return "Webブラウザ用HTML（複数ファイル）";
        default: {
            // カスタムフォーマットの場合
            auto it = m_customFormats.find(format);
            if (it != m_customFormats.end()) {
                return it->second.description;
            }
            return "説明なし";
        }
    }
}

std::string OutputFormatSwitcher::getFormatExtension(OutputFormat format) const {
    switch (format) {
        case OutputFormat::PDF:
            return ".pdf";
        case OutputFormat::EPUB:
            return ".epub";
        case OutputFormat::HTML:
            return ".html";
        case OutputFormat::HTML_MULTI:
            return ""; // ディレクトリ出力のため拡張子なし
        default: {
            // カスタムフォーマットの場合
            auto it = m_customFormats.find(format);
            if (it != m_customFormats.end()) {
                return it->second.extension;
            }
            return "";
        }
    }
}

OutputFormat OutputFormatSwitcher::registerCustomFormat(
    const std::string& name,
    const std::string& description,
    const std::string& extension,
    std::function<bool(const core::document::Document&, const core::style::Style&, const std::string&, const OutputOptions&)> outputFunc) {
    
    OutputFormat format = m_nextCustomFormatId;
    
    // 次のカスタムフォーマットIDを更新
    m_nextCustomFormatId = static_cast<OutputFormat>(static_cast<int>(m_nextCustomFormatId) + 1);
    
    // カスタムフォーマット情報を登録
    CustomFormatInfo info;
    info.name = name;
    info.description = description;
    info.extension = extension;
    info.outputFunc = outputFunc;
    
    m_customFormats[format] = info;
    
    return format;
}

bool OutputFormatSwitcher::unregisterCustomFormat(OutputFormat format) {
    // 標準フォーマットは登録解除できない
    if (format == OutputFormat::PDF || format == OutputFormat::EPUB || 
        format == OutputFormat::HTML || format == OutputFormat::HTML_MULTI) {
        return false;
    }
    
    auto it = m_customFormats.find(format);
    if (it == m_customFormats.end()) {
        return false;
    }
    
    m_customFormats.erase(it);
    return true;
}

PDFOutputOptions OutputFormatSwitcher::convertToPDFOptions(const OutputOptions& options) const {
    PDFOutputOptions pdfOptions;
    
    pdfOptions.fontFamily = options.fontFamily;
    pdfOptions.fontSize = options.fontSize;
    pdfOptions.lineHeight = options.lineHeight;
    pdfOptions.vertical = options.vertical;
    pdfOptions.title = options.title;
    pdfOptions.author = options.author;
    
    // その他のオプションの変換
    auto it = options.extraOptions.find("pageSize");
    if (it != options.extraOptions.end()) {
        pdfOptions.pageSize = it->second;
    }
    
    it = options.extraOptions.find("marginTop");
    if (it != options.extraOptions.end()) {
        pdfOptions.marginTop = std::stod(it->second);
    }
    
    it = options.extraOptions.find("marginBottom");
    if (it != options.extraOptions.end()) {
        pdfOptions.marginBottom = std::stod(it->second);
    }
    
    it = options.extraOptions.find("marginLeft");
    if (it != options.extraOptions.end()) {
        pdfOptions.marginLeft = std::stod(it->second);
    }
    
    it = options.extraOptions.find("marginRight");
    if (it != options.extraOptions.end()) {
        pdfOptions.marginRight = std::stod(it->second);
    }
    
    return pdfOptions;
}

EpubOutputOptions OutputFormatSwitcher::convertToEpubOptions(const OutputOptions& options) const {
    EpubOutputOptions epubOptions;
    
    epubOptions.fontFamily = options.fontFamily;
    epubOptions.fontSize = options.fontSize;
    epubOptions.lineHeight = options.lineHeight;
    epubOptions.vertical = options.vertical;
    epubOptions.title = options.title;
    epubOptions.author = options.author;
    
    // その他のオプションの変換
    auto it = options.extraOptions.find("publisher");
    if (it != options.extraOptions.end()) {
        epubOptions.publisher = it->second;
    }
    
    it = options.extraOptions.find("language");
    if (it != options.extraOptions.end()) {
        epubOptions.language = it->second;
    }
    
    it = options.extraOptions.find("identifier");
    if (it != options.extraOptions.end()) {
        epubOptions.identifier = it->second;
    }
    
    it = options.extraOptions.find("description");
    if (it != options.extraOptions.end()) {
        epubOptions.description = it->second;
    }
    
    it = options.extraOptions.find("rights");
    if (it != options.extraOptions.end()) {
        epubOptions.rights = it->second;
    }
    
    it = options.extraOptions.find("coverImagePath");
    if (it != options.extraOptions.end()) {
        epubOptions.coverImagePath = it->second;
    }
    
    return epubOptions;
}

HtmlOutputOptions OutputFormatSwitcher::convertToHtmlOptions(const OutputOptions& options) const {
    HtmlOutputOptions htmlOptions;
    
    htmlOptions.fontFamily = options.fontFamily;
    htmlOptions.fontSize = options.fontSize;
    htmlOptions.lineHeight = options.lineHeight;
    htmlOptions.vertical = options.vertical;
    htmlOptions.title = options.title;
    htmlOptions.author = options.author;
    
    // その他のオプションの変換
    auto it = options.extraOptions.find("language");
    if (it != options.extraOptions.end()) {
        htmlOptions.language = it->second;
    }
    
    it = options.extraOptions.find("description");
    if (it != options.extraOptions.end()) {
        htmlOptions.description = it->second;
    }
    
    it = options.extraOptions.find("embedCss");
    if (it != options.extraOptions.end()) {
        htmlOptions.embedCss = (it->second == "true");
    }
    
    it = options.extraOptions.find("embedFonts");
    if (it != options.extraOptions.end()) {
        htmlOptions.embedFonts = (it->second == "true");
    }
    
    it = options.extraOptions.find("responsive");
    if (it != options.extraOptions.end()) {
        htmlOptions.responsive = (it->second == "true");
    }
    
    it = options.extraOptions.find("generateToc");
    if (it != options.extraOptions.end()) {
        htmlOptions.generateToc = (it->second == "true");
    }
    
    it = options.extraOptions.find("cssPath");
    if (it != options.extraOptions.end()) {
        htmlOptions.cssPath = it->second;
    }
    
    return htmlOptions;
}

} // namespace output
} // namespace japanese_typesetting
