/**
 * @file style.cpp
 * @brief スタイル定義の詳細実装
 */

#include "japanese_typesetting/core/style/style.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace japanese_typesetting {
namespace core {
namespace style {

Style::Style()
    : m_fontFamily("Mincho")
    , m_fontSize(10.5)
    , m_lineHeight(1.5)
    , m_textAlignment(TextAlignment::Justify)
    , m_lineBreakMode(LineBreakMode::Normal)
    , m_characterSpacing(0.0)
    , m_wordSpacing(0.0)
    , m_paragraphSpacingBefore(0.0)
    , m_paragraphSpacingAfter(0.5)
    , m_firstLineIndent(1.0)
    , m_bold(false)
    , m_italic(false)
    , m_underline(false) {
}

Style::~Style() {
    // 特に何もしない
}

void Style::setFontFamily(const std::string& fontFamily) {
    m_fontFamily = fontFamily;
}

std::string Style::getFontFamily() const {
    return m_fontFamily;
}

void Style::setFontSize(double fontSize) {
    m_fontSize = fontSize;
}

double Style::getFontSize() const {
    return m_fontSize;
}

void Style::setLineHeight(double lineHeight) {
    m_lineHeight = lineHeight;
}

double Style::getLineHeight() const {
    return m_lineHeight;
}

void Style::setTextAlignment(TextAlignment alignment) {
    m_textAlignment = alignment;
}

TextAlignment Style::getTextAlignment() const {
    return m_textAlignment;
}

void Style::setLineBreakMode(LineBreakMode mode) {
    m_lineBreakMode = mode;
}

LineBreakMode Style::getLineBreakMode() const {
    return m_lineBreakMode;
}

void Style::setCharacterSpacing(double spacing) {
    m_characterSpacing = spacing;
}

double Style::getCharacterSpacing() const {
    return m_characterSpacing;
}

void Style::setWordSpacing(double spacing) {
    m_wordSpacing = spacing;
}

double Style::getWordSpacing() const {
    return m_wordSpacing;
}

void Style::setParagraphSpacingBefore(double margin) {
    m_paragraphSpacingBefore = margin;
}

double Style::getParagraphSpacingBefore() const {
    return m_paragraphSpacingBefore;
}

void Style::setParagraphSpacingAfter(double margin) {
    m_paragraphSpacingAfter = margin;
}

double Style::getParagraphSpacingAfter() const {
    return m_paragraphSpacingAfter;
}

void Style::setFirstLineIndent(double indent) {
    m_firstLineIndent = indent;
}

double Style::getFirstLineIndent() const {
    return m_firstLineIndent;
}

void Style::setBold(bool bold) {
    m_bold = bold;
}

bool Style::isBold() const {
    return m_bold;
}

void Style::setItalic(bool italic) {
    m_italic = italic;
}

bool Style::isItalic() const {
    return m_italic;
}

void Style::setUnderline(bool underline) {
    m_underline = underline;
}

bool Style::isUnderline() const {
    return m_underline;
}

void Style::setProperty(const std::string& key, const std::string& value) {
    m_properties[key] = value;
}

std::string Style::getProperty(const std::string& key) const {
    auto it = m_properties.find(key);
    if (it != m_properties.end()) {
        return it->second;
    }
    return "";
}

bool Style::loadFromFile(const std::string& filePath) {
    // 簡易的なファイル読み込み実装
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open style file: " << filePath << std::endl;
        return false;
    }

    // ファイルフォーマットの例：
    // FontFamily: Mincho
    // FontSize: 10.5
    // LineHeight: 1.5
    // TextAlignment: Justify
    // LineBreakMode: Normal
    // CharacterSpacing: 0.0
    // WordSpacing: 0.0
    // ParagraphSpacingBefore: 0.0
    // ParagraphSpacingAfter: 0.5
    // FirstLineIndent: 1.0
    // Bold: false
    // Italic: false
    // Underline: false
    // Property-Key1: Value1
    // Property-Key2: Value2

    std::string line;
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            
            // 先頭と末尾の空白を削除
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (key == "FontFamily") {
                setFontFamily(value);
            } else if (key == "FontSize") {
                setFontSize(std::stod(value));
            } else if (key == "LineHeight") {
                setLineHeight(std::stod(value));
            } else if (key == "TextAlignment") {
                if (value == "Left") {
                    setTextAlignment(TextAlignment::Left);
                } else if (value == "Right") {
                    setTextAlignment(TextAlignment::Right);
                } else if (value == "Center") {
                    setTextAlignment(TextAlignment::Center);
                } else if (value == "Justify") {
                    setTextAlignment(TextAlignment::Justify);
                }
            } else if (key == "LineBreakMode") {
                if (value == "Normal") {
                    setLineBreakMode(LineBreakMode::Normal);
                } else if (value == "Strict") {
                    setLineBreakMode(LineBreakMode::Strict);
                } else if (value == "Loose") {
                    setLineBreakMode(LineBreakMode::Loose);
                }
            } else if (key == "CharacterSpacing") {
                setCharacterSpacing(std::stod(value));
            } else if (key == "WordSpacing") {
                setWordSpacing(std::stod(value));
            } else if (key == "ParagraphSpacingBefore") {
                setParagraphSpacingBefore(std::stod(value));
            } else if (key == "ParagraphSpacingAfter") {
                setParagraphSpacingAfter(std::stod(value));
            } else if (key == "FirstLineIndent") {
                setFirstLineIndent(std::stod(value));
            } else if (key == "Bold") {
                setBold(value == "true");
            } else if (key == "Italic") {
                setItalic(value == "true");
            } else if (key == "Underline") {
                setUnderline(value == "true");
            } else if (key.substr(0, 9) == "Property-") {
                setProperty(key.substr(9), value);
            }
        }
    }

    file.close();
    return true;
}

bool Style::saveToFile(const std::string& filePath) const {
    // 簡易的なファイル保存実装
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    file << "FontFamily: " << m_fontFamily << "\n";
    file << "FontSize: " << m_fontSize << "\n";
    file << "LineHeight: " << m_lineHeight << "\n";
    
    // TextAlignment
    switch (m_textAlignment) {
        case TextAlignment::Left:
            file << "TextAlignment: Left\n";
            break;
        case TextAlignment::Right:
            file << "TextAlignment: Right\n";
            break;
        case TextAlignment::Center:
            file << "TextAlignment: Center\n";
            break;
        case TextAlignment::Justify:
            file << "TextAlignment: Justify\n";
            break;
    }
    
    // LineBreakMode
    switch (m_lineBreakMode) {
        case LineBreakMode::Normal:
            file << "LineBreakMode: Normal\n";
            break;
        case LineBreakMode::Strict:
            file << "LineBreakMode: Strict\n";
            break;
        case LineBreakMode::Loose:
            file << "LineBreakMode: Loose\n";
            break;
    }
    
    file << "CharacterSpacing: " << m_characterSpacing << "\n";
    file << "WordSpacing: " << m_wordSpacing << "\n";
    file << "ParagraphSpacingBefore: " << m_paragraphSpacingBefore << "\n";
    file << "ParagraphSpacingAfter: " << m_paragraphSpacingAfter << "\n";
    file << "FirstLineIndent: " << m_firstLineIndent << "\n";
    file << "Bold: " << (m_bold ? "true" : "false") << "\n";
    file << "Italic: " << (m_italic ? "true" : "false") << "\n";
    file << "Underline: " << (m_underline ? "true" : "false") << "\n";
    
    // カスタムプロパティの書き込み
    for (const auto& prop : m_properties) {
        file << "Property-" << prop.first << ": " << prop.second << "\n";
    }

    file.close();
    return true;
}

} // namespace style
} // namespace core
} // namespace japanese_typesetting
