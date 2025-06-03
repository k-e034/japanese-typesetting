/**
 * @file typesetting_engine.cpp
 * @brief 組版エンジンの詳細実装
 */

#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <algorithm>
#include <cmath>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

TypesettingEngine::TypesettingEngine() {
    // デフォルトの組版ルールを設定
    m_rules.setDefaultJisX4051Rules();
}

TypesettingEngine::~TypesettingEngine() {
    // 特に何もしない
}

void TypesettingEngine::setTypesettingRules(const TypesettingRules& rules) {
    m_rules = rules;
}

const TypesettingRules& TypesettingEngine::getTypesettingRules() const {
    return m_rules;
}

void TypesettingEngine::setUnicodeHandler(const unicode::UnicodeHandler& handler) {
    m_unicodeHandler = handler;
}

const unicode::UnicodeHandler& TypesettingEngine::getUnicodeHandler() const {
    return m_unicodeHandler;
}

TextBlock TypesettingEngine::typeset(const std::string& text, const style::Style& style, double width, bool vertical) {
    // UTF-8からUTF-32に変換
    std::u32string utf32Text = m_unicodeHandler.utf8ToUtf32(text);
    
    // 行分割を行う
    std::vector<TextLine> lines = breakLines(utf32Text, style, width, vertical);
    
    // 禁則処理を適用
    applyProhibitionRules(lines, style, vertical);
    
    // 文字詰め処理を適用
    applyJustification(lines, style, width, vertical);
    
    // ぶら下げ処理を適用
    applyHanging(lines, style, vertical);
    
    // テキストブロックを作成
    TextBlock block;
    block.lines = lines;
    
    // ブロックの幅と高さを計算
    block.width = width;
    block.height = 0.0;
    for (const auto& line : lines) {
        block.height += line.height;
    }
    
    return block;
}

std::vector<TextBlock> TypesettingEngine::typesetDocument(const document::Document& document, const style::Style& style, double width) {
    std::vector<TextBlock> blocks;
    
    // 各セクションを組版
    for (size_t i = 0; i < document.getSectionCount(); ++i) {
        document::Section* section = document.getSection(i);
        if (section) {
            // セクションのタイトルを組版
            if (!section->getTitle().empty()) {
                style::Style titleStyle = style;
                titleStyle.setBold(true);
                titleStyle.setFontSize(style.getFontSize() * 1.2); // タイトルは少し大きく
                
                TextBlock titleBlock = typeset(section->getTitle(), titleStyle, width, document.isVertical());
                blocks.push_back(titleBlock);
            }
            
            // セクションの内容を組版
            TextBlock contentBlock = typeset(section->getContent(), style, width, document.isVertical());
            blocks.push_back(contentBlock);
            
            // 子セクションを再帰的に組版（将来的な拡張）
        }
    }
    
    return blocks;
}

std::vector<TextLine> TypesettingEngine::breakLines(const std::u32string& text, const style::Style& style, double maxWidth, bool vertical) {
    std::vector<TextLine> lines;
    
    // 現在の行
    TextLine currentLine;
    currentLine.text.clear();
    currentLine.width = 0.0;
    currentLine.height = style.getFontSize() * style.getLineHeight();
    currentLine.baseline = style.getFontSize() * 0.8; // 仮のベースライン位置
    currentLine.hasLineBreak = false;
    
    // 文字ごとに処理
    for (size_t i = 0; i < text.length(); ++i) {
        char32_t ch = text[i];
        
        // 改行文字の処理
        if (ch == U'\n') {
            currentLine.hasLineBreak = true;
            lines.push_back(currentLine);
            
            // 新しい行を開始
            currentLine.text.clear();
            currentLine.width = 0.0;
            currentLine.hasLineBreak = false;
            continue;
        }
        
        // 文字の幅を計算
        double charWidth = calculateCharacterWidth(ch, style, vertical);
        
        // 行の最大幅を超える場合は改行
        if (currentLine.width + charWidth > maxWidth && !currentLine.text.empty()) {
            lines.push_back(currentLine);
            
            // 新しい行を開始
            currentLine.text.clear();
            currentLine.width = 0.0;
            currentLine.hasLineBreak = false;
        }
        
        // 文字を追加
        currentLine.text.push_back(ch);
        currentLine.width += charWidth;
    }
    
    // 最後の行を追加
    if (!currentLine.text.empty()) {
        lines.push_back(currentLine);
    }
    
    return lines;
}

void TypesettingEngine::applyProhibitionRules(std::vector<TextLine>& lines, const style::Style& style, bool vertical) {
    // 行が2行未満の場合は処理不要
    if (lines.size() < 2) {
        return;
    }
    
    for (size_t i = 0; i < lines.size() - 1; ++i) {
        TextLine& currentLine = lines[i];
        TextLine& nextLine = lines[i + 1];
        
        // 明示的な改行がある場合はスキップ
        if (currentLine.hasLineBreak) {
            continue;
        }
        
        // 次の行が空の場合はスキップ
        if (nextLine.text.empty()) {
            continue;
        }
        
        // 行末禁則処理
        if (!currentLine.text.empty()) {
            char32_t lastChar = currentLine.text.back();
            
            // 行末禁則文字の場合
            if (m_rules.isLineEndProhibited(lastChar)) {
                // 次の行の先頭に移動
                currentLine.text.pop_back();
                currentLine.width -= calculateCharacterWidth(lastChar, style, vertical);
                
                nextLine.text.insert(0, 1, lastChar);
                nextLine.width += calculateCharacterWidth(lastChar, style, vertical);
            }
        }
        
        // 行頭禁則処理
        if (!nextLine.text.empty()) {
            char32_t firstChar = nextLine.text.front();
            
            // 行頭禁則文字の場合
            if (m_rules.isLineStartProhibited(firstChar)) {
                // 前の行の末尾に移動
                nextLine.text.erase(0, 1);
                nextLine.width -= calculateCharacterWidth(firstChar, style, vertical);
                
                currentLine.text.push_back(firstChar);
                currentLine.width += calculateCharacterWidth(firstChar, style, vertical);
            }
        }
    }
}

void TypesettingEngine::applyJustification(std::vector<TextLine>& lines, const style::Style& style, double maxWidth, bool vertical) {
    // 両端揃えの場合のみ処理
    if (style.getTextAlignment() != style::TextAlignment::Justify) {
        return;
    }
    
    for (auto& line : lines) {
        // 明示的な改行がある場合や、行が最大幅に近い場合はスキップ
        if (line.hasLineBreak || line.width >= maxWidth * 0.95) {
            continue;
        }
        
        // 行内の文字数
        size_t charCount = line.text.length();
        if (charCount <= 1) {
            continue;
        }
        
        // 追加する文字間隔を計算
        double additionalSpacing = (maxWidth - line.width) / (charCount - 1);
        
        // 文字間隔を調整した新しい幅を設定
        line.width = maxWidth;
        
        // 実際の描画時に文字間隔を適用する（この実装では単純に幅を調整するのみ）
    }
}

void TypesettingEngine::applyHanging(std::vector<TextLine>& lines, const style::Style& style, bool vertical) {
    for (auto& line : lines) {
        // 行が空の場合はスキップ
        if (line.text.empty()) {
            continue;
        }
        
        // 行末の文字がぶら下げ対象かチェック
        char32_t lastChar = line.text.back();
        if (m_rules.isHangingCharacter(lastChar)) {
            // ぶら下げ対象の場合、行の幅を調整（実際の描画時に位置を調整）
            double charWidth = calculateCharacterWidth(lastChar, style, vertical);
            line.width -= charWidth * 0.5; // 半分だけぶら下げる
        }
    }
}

double TypesettingEngine::calculateCharacterWidth(char32_t character, const style::Style& style, bool vertical) {
    // 簡易的な実装：フォントサイズに基づいて幅を計算
    // 実際の実装では、フォントメトリクスを使用して正確な幅を計算する
    
    double baseWidth = style.getFontSize();
    
    // 全角文字と半角文字で幅を調整
    if (m_unicodeHandler.isFullWidthCharacter(character)) {
        return baseWidth;
    } else if (m_unicodeHandler.isHalfWidthCharacter(character)) {
        return baseWidth * 0.5;
    }
    
    // デフォルトは全角として扱う
    return baseWidth;
}

double TypesettingEngine::calculateCharacterHeight(char32_t character, const style::Style& style, bool vertical) {
    // 簡易的な実装：フォントサイズに基づいて高さを計算
    // 実際の実装では、フォントメトリクスを使用して正確な高さを計算する
    
    return style.getFontSize();
}

double TypesettingEngine::calculateTextWidth(const std::u32string& text, const style::Style& style, bool vertical) {
    double width = 0.0;
    
    for (char32_t ch : text) {
        width += calculateCharacterWidth(ch, style, vertical);
    }
    
    // 文字間隔を考慮
    if (text.length() > 1) {
        width += style.getCharacterSpacing() * style.getFontSize() * (text.length() - 1);
    }
    
    return width;
}

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting
