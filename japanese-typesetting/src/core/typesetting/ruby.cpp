/**
 * @file ruby.cpp
 * @brief ルビ処理の詳細実装
 */

#include "japanese_typesetting/core/typesetting/ruby.h"
#include <algorithm>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

RubyProcessor::RubyProcessor() {
    // 特に初期化処理はない
}

RubyProcessor::~RubyProcessor() {
    // 特に何もしない
}

std::vector<RubyText> RubyProcessor::parseRuby(const std::u32string& text) {
    std::vector<RubyText> result;
    
    size_t pos = 0;
    while (pos < text.length()) {
        // ルビ開始タグを検索
        size_t rubyStartPos = findRubyStartTag(text, pos);
        if (rubyStartPos == static_cast<size_t>(-1)) {
            break; // ルビタグが見つからない場合は終了
        }
        
        // ルビテキストタグを検索
        size_t rubyTextTagPos = findRubyTextTag(text, rubyStartPos + 1);
        if (rubyTextTagPos == static_cast<size_t>(-1)) {
            pos = rubyStartPos + 1;
            continue; // ルビテキストタグが見つからない場合は次の検索へ
        }
        
        // ルビ終了タグを検索
        size_t rubyEndPos = findRubyEndTag(text, rubyTextTagPos + 1);
        if (rubyEndPos == static_cast<size_t>(-1)) {
            pos = rubyTextTagPos + 1;
            continue; // ルビ終了タグが見つからない場合は次の検索へ
        }
        
        // ベーステキストとルビテキストを抽出
        std::u32string baseText = text.substr(rubyStartPos + 1, rubyTextTagPos - rubyStartPos - 1);
        std::u32string rubyText = text.substr(rubyTextTagPos + 1, rubyEndPos - rubyTextTagPos - 1);
        
        // ルビ情報を追加
        RubyText ruby;
        ruby.base = baseText;
        ruby.ruby = rubyText;
        ruby.startPos = rubyStartPos;
        ruby.endPos = rubyEndPos + 1;
        
        result.push_back(ruby);
        
        // 次の検索位置を更新
        pos = rubyEndPos + 1;
    }
    
    return result;
}

void RubyProcessor::calculateRubyLayout(RubyText& rubyText, const style::Style& baseStyle, const style::Style& rubyStyle, bool vertical) {
    // ルビの配置計算の実装
    // 実際の実装では、ベーステキストとルビテキストの幅や高さを計算し、
    // 適切な配置方法（モノルビ、グループルビなど）を決定する
    
    // この簡易実装では特に何もしない
    // 将来的には、ルビの配置情報を計算して返す
}

size_t RubyProcessor::findRubyStartTag(const std::u32string& text, size_t startPos) {
    // ルビ開始タグ「｜」または「《」の検索
    for (size_t i = startPos; i < text.length(); ++i) {
        if (text[i] == U'｜' || text[i] == U'《') {
            return i;
        }
    }
    return static_cast<size_t>(-1);
}

size_t RubyProcessor::findRubyTextTag(const std::u32string& text, size_t startPos) {
    // ルビテキストタグ「《」の検索
    for (size_t i = startPos; i < text.length(); ++i) {
        if (text[i] == U'《') {
            return i;
        }
    }
    return static_cast<size_t>(-1);
}

size_t RubyProcessor::findRubyEndTag(const std::u32string& text, size_t startPos) {
    // ルビ終了タグ「》」の検索
    for (size_t i = startPos; i < text.length(); ++i) {
        if (text[i] == U'》') {
            return i;
        }
    }
    return static_cast<size_t>(-1);
}

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting
