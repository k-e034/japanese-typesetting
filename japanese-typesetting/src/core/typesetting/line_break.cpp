/**
 * @file line_break.cpp
 * @brief 行分割アルゴリズムの詳細実装
 */

#include "japanese_typesetting/core/typesetting/line_break.h"
#include <algorithm>
#include <limits>
#include <cmath>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

LineBreaker::LineBreaker(const TypesettingRules& rules, const unicode::UnicodeHandler& unicodeHandler)
    : m_rules(rules)
    , m_unicodeHandler(unicodeHandler) {
}

LineBreaker::~LineBreaker() {
    // 特に何もしない
}

std::vector<std::u32string> LineBreaker::breakLines(const std::u32string& text, const style::Style& style, double maxWidth, bool vertical) {
    std::vector<std::u32string> lines;
    
    // 分割可能な位置を検出
    std::vector<BreakPoint> breakPoints = findBreakPoints(text);
    
    // 最適な分割位置を計算
    std::vector<size_t> optimalBreaks = calculateOptimalBreaks(text, breakPoints, style, maxWidth, vertical);
    
    // 分割位置に基づいて行を生成
    size_t startPos = 0;
    for (size_t breakPos : optimalBreaks) {
        lines.push_back(text.substr(startPos, breakPos - startPos));
        startPos = breakPos;
    }
    
    // 最後の行を追加
    if (startPos < text.length()) {
        lines.push_back(text.substr(startPos));
    }
    
    return lines;
}

std::vector<BreakPoint> LineBreaker::findBreakPoints(const std::u32string& text) {
    std::vector<BreakPoint> breakPoints;
    
    // 先頭位置を追加（ペナルティは高く設定）
    BreakPoint startPoint;
    startPoint.position = 0;
    startPoint.penalty = 1000.0;
    startPoint.mandatory = false;
    breakPoints.push_back(startPoint);
    
    for (size_t i = 0; i < text.length(); ++i) {
        char32_t ch = text[i];
        
        // 改行文字は強制的な分割点
        if (ch == U'\n') {
            BreakPoint bp;
            bp.position = i + 1; // 改行文字の次の位置
            bp.penalty = 0.0;    // 最低ペナルティ
            bp.mandatory = true;
            breakPoints.push_back(bp);
            continue;
        }
        
        // 空白文字は分割可能
        if (ch == U' ' || ch == U'\t') {
            BreakPoint bp;
            bp.position = i + 1; // 空白文字の次の位置
            bp.penalty = 50.0;   // 中程度のペナルティ
            bp.mandatory = false;
            breakPoints.push_back(bp);
            continue;
        }
        
        // 日本語の文字間は基本的に分割可能
        if (i > 0 && m_unicodeHandler.isJapaneseCharacter(text[i-1]) && m_unicodeHandler.isJapaneseCharacter(ch)) {
            // 行頭禁則文字の場合は分割不可
            if (m_rules.isLineStartProhibited(ch)) {
                continue;
            }
            
            // 行末禁則文字の場合は分割不可
            if (m_rules.isLineEndProhibited(text[i-1])) {
                continue;
            }
            
            // 分離禁止文字の場合は分割不可
            if (m_rules.isInseparable(ch) || m_rules.isInseparable(text[i-1])) {
                continue;
            }
            
            BreakPoint bp;
            bp.position = i;
            bp.penalty = 100.0;  // 日本語の文字間は空白よりも高いペナルティ
            bp.mandatory = false;
            breakPoints.push_back(bp);
        }
    }
    
    // 末尾位置を追加
    BreakPoint endPoint;
    endPoint.position = text.length();
    endPoint.penalty = 0.0;
    endPoint.mandatory = true;
    breakPoints.push_back(endPoint);
    
    return breakPoints;
}

std::vector<size_t> LineBreaker::calculateOptimalBreaks(const std::u32string& text, const std::vector<BreakPoint>& breakPoints, const style::Style& style, double maxWidth, bool vertical) {
    // 動的計画法による最適な分割位置の計算
    std::vector<size_t> result;
    
    // 分割点がない場合は空のリストを返す
    if (breakPoints.size() <= 1) {
        return result;
    }
    
    // 各分割点までの最適なペナルティと前の分割点を記録する配列
    std::vector<double> minPenalty(breakPoints.size(), std::numeric_limits<double>::infinity());
    std::vector<size_t> prev(breakPoints.size(), 0);
    
    // 初期値の設定
    minPenalty[0] = 0.0;
    
    // 各分割点について最適な前の分割点を計算
    for (size_t j = 1; j < breakPoints.size(); ++j) {
        size_t endPos = breakPoints[j].position;
        
        for (size_t i = 0; i < j; ++i) {
            size_t startPos = breakPoints[i].position;
            
            // この区間のテキストを取得
            std::u32string segment = text.substr(startPos, endPos - startPos);
            
            // 区間の幅を計算
            double width = 0.0;
            for (char32_t ch : segment) {
                width += calculateCharacterWidth(ch, style, vertical);
            }
            
            // 最大幅を超える場合はスキップ（強制分割点を除く）
            if (width > maxWidth && !breakPoints[j].mandatory) {
                continue;
            }
            
            // 行の余白に基づくペナルティ
            double linePenalty = 0.0;
            if (width < maxWidth) {
                // 行が短すぎる場合のペナルティ
                double ratio = width / maxWidth;
                linePenalty = 100.0 * (1.0 - ratio) * (1.0 - ratio);
            }
            
            // 分割点自体のペナルティ
            double breakPenalty = breakPoints[j].penalty;
            
            // 総合ペナルティ
            double totalPenalty = minPenalty[i] + linePenalty + breakPenalty;
            
            // より良い分割が見つかった場合は更新
            if (totalPenalty < minPenalty[j]) {
                minPenalty[j] = totalPenalty;
                prev[j] = i;
            }
        }
    }
    
    // 最適な分割位置を逆順に取得
    std::vector<size_t> breaks;
    size_t j = breakPoints.size() - 1;
    while (j > 0) {
        j = prev[j];
        if (j > 0) {
            breaks.push_back(breakPoints[j].position);
        }
    }
    
    // 分割位置を正順にする
    std::reverse(breaks.begin(), breaks.end());
    
    return breaks;
}

double LineBreaker::calculateCharacterWidth(char32_t character, const style::Style& style, bool vertical) {
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

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting
