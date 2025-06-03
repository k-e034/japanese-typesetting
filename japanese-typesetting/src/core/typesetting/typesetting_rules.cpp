/**
 * @file typesetting_rules.cpp
 * @brief 日本語組版ルールの実装
 */

#include "japanese_typesetting/core/typesetting/typesetting_rules.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace japanese_typesetting {
namespace core {
namespace typesetting {

TypesettingRules::TypesettingRules() {
    // デフォルトのJIS X 4051準拠ルールを設定
    setDefaultJisX4051Rules();
}

TypesettingRules::~TypesettingRules() {
    // 特に何もしない
}

void TypesettingRules::addLineStartProhibitedCharacter(char32_t character) {
    m_lineStartProhibitedChars.insert(character);
}

bool TypesettingRules::isLineStartProhibited(char32_t character) const {
    return m_lineStartProhibitedChars.find(character) != m_lineStartProhibitedChars.end();
}

const std::set<char32_t>& TypesettingRules::getLineStartProhibitedCharacters() const {
    return m_lineStartProhibitedChars;
}

void TypesettingRules::addLineEndProhibitedCharacter(char32_t character) {
    m_lineEndProhibitedChars.insert(character);
}

bool TypesettingRules::isLineEndProhibited(char32_t character) const {
    return m_lineEndProhibitedChars.find(character) != m_lineEndProhibitedChars.end();
}

const std::set<char32_t>& TypesettingRules::getLineEndProhibitedCharacters() const {
    return m_lineEndProhibitedChars;
}

void TypesettingRules::addInseparableCharacter(char32_t character) {
    m_inseparableChars.insert(character);
}

bool TypesettingRules::isInseparable(char32_t character) const {
    return m_inseparableChars.find(character) != m_inseparableChars.end();
}

const std::set<char32_t>& TypesettingRules::getInseparableCharacters() const {
    return m_inseparableChars;
}

void TypesettingRules::addHangingCharacter(char32_t character) {
    m_hangingChars.insert(character);
}

bool TypesettingRules::isHangingCharacter(char32_t character) const {
    return m_hangingChars.find(character) != m_hangingChars.end();
}

const std::set<char32_t>& TypesettingRules::getHangingCharacters() const {
    return m_hangingChars;
}

void TypesettingRules::setDefaultJisX4051Rules() {
    // 行頭禁則文字（JIS X 4051準拠）
    // 句読点
    addLineStartProhibitedCharacter(U'、');
    addLineStartProhibitedCharacter(U'。');
    addLineStartProhibitedCharacter(U'，');
    addLineStartProhibitedCharacter(U'．');
    addLineStartProhibitedCharacter(U'・');
    addLineStartProhibitedCharacter(U'：');
    addLineStartProhibitedCharacter(U'；');
    addLineStartProhibitedCharacter(U'？');
    addLineStartProhibitedCharacter(U'！');
    addLineStartProhibitedCharacter(U'‥');
    addLineStartProhibitedCharacter(U'…');
    addLineStartProhibitedCharacter(U'—');
    addLineStartProhibitedCharacter(U'―');
    
    // 閉じ括弧類
    addLineStartProhibitedCharacter(U'）');
    addLineStartProhibitedCharacter(U'］');
    addLineStartProhibitedCharacter(U'｝');
    addLineStartProhibitedCharacter(U'」');
    addLineStartProhibitedCharacter(U'』');
    addLineStartProhibitedCharacter(U'】');
    addLineStartProhibitedCharacter(U'〕');
    addLineStartProhibitedCharacter(U'〉');
    addLineStartProhibitedCharacter(U'》');
    addLineStartProhibitedCharacter(U'）');
    addLineStartProhibitedCharacter(U'〗');
    addLineStartProhibitedCharacter(U'〙');
    addLineStartProhibitedCharacter(U'〟');
    
    // 中点類
    addLineStartProhibitedCharacter(U'・');
    addLineStartProhibitedCharacter(U'：');
    addLineStartProhibitedCharacter(U'；');
    
    // その他の記号
    addLineStartProhibitedCharacter(U'ゝ');
    addLineStartProhibitedCharacter(U'ゞ');
    addLineStartProhibitedCharacter(U'ー');
    addLineStartProhibitedCharacter(U'ァ');
    addLineStartProhibitedCharacter(U'ィ');
    addLineStartProhibitedCharacter(U'ゥ');
    addLineStartProhibitedCharacter(U'ェ');
    addLineStartProhibitedCharacter(U'ォ');
    addLineStartProhibitedCharacter(U'ッ');
    addLineStartProhibitedCharacter(U'ャ');
    addLineStartProhibitedCharacter(U'ュ');
    addLineStartProhibitedCharacter(U'ョ');
    addLineStartProhibitedCharacter(U'ヮ');
    addLineStartProhibitedCharacter(U'ヵ');
    addLineStartProhibitedCharacter(U'ヶ');
    addLineStartProhibitedCharacter(U'ぁ');
    addLineStartProhibitedCharacter(U'ぃ');
    addLineStartProhibitedCharacter(U'ぅ');
    addLineStartProhibitedCharacter(U'ぇ');
    addLineStartProhibitedCharacter(U'ぉ');
    addLineStartProhibitedCharacter(U'っ');
    addLineStartProhibitedCharacter(U'ゃ');
    addLineStartProhibitedCharacter(U'ゅ');
    addLineStartProhibitedCharacter(U'ょ');
    addLineStartProhibitedCharacter(U'ゎ');
    addLineStartProhibitedCharacter(U'々');
    addLineStartProhibitedCharacter(U'〻');
    addLineStartProhibitedCharacter(U'‐');
    addLineStartProhibitedCharacter(U'゠');
    addLineStartProhibitedCharacter(U'–');
    addLineStartProhibitedCharacter(U'〜');
    addLineStartProhibitedCharacter(U'?');
    addLineStartProhibitedCharacter(U'!');
    addLineStartProhibitedCharacter(U'‼');
    addLineStartProhibitedCharacter(U'⁇');
    addLineStartProhibitedCharacter(U'⁈');
    addLineStartProhibitedCharacter(U'⁉');
    addLineStartProhibitedCharacter(U'℃');
    addLineStartProhibitedCharacter(U'％');
    addLineStartProhibitedCharacter(U'‰');
    addLineStartProhibitedCharacter(U'‱');
    addLineStartProhibitedCharacter(U'°');
    
    // 行末禁則文字（JIS X 4051準拠）
    // 開き括弧類
    addLineEndProhibitedCharacter(U'（');
    addLineEndProhibitedCharacter(U'［');
    addLineEndProhibitedCharacter(U'｛');
    addLineEndProhibitedCharacter(U'「');
    addLineEndProhibitedCharacter(U'『');
    addLineEndProhibitedCharacter(U'【');
    addLineEndProhibitedCharacter(U'〔');
    addLineEndProhibitedCharacter(U'〈');
    addLineEndProhibitedCharacter(U'《');
    addLineEndProhibitedCharacter(U'〖');
    addLineEndProhibitedCharacter(U'〘');
    addLineEndProhibitedCharacter(U'〝');
    
    // 分離禁止文字（JIS X 4051準拠）
    // 単位記号
    addInseparableCharacter(U'$');
    addInseparableCharacter(U'￥');
    addInseparableCharacter(U'￡');
    addInseparableCharacter(U'℃');
    addInseparableCharacter(U'°');
    
    // ぶら下げ対象文字（JIS X 4051準拠）
    // 句読点
    addHangingCharacter(U'、');
    addHangingCharacter(U'。');
    addHangingCharacter(U'，');
    addHangingCharacter(U'．');
    
    // 閉じ括弧類
    addHangingCharacter(U'）');
    addHangingCharacter(U'］');
    addHangingCharacter(U'｝');
    addHangingCharacter(U'」');
    addHangingCharacter(U'』');
    addHangingCharacter(U'】');
    addHangingCharacter(U'〕');
    addHangingCharacter(U'〉');
    addHangingCharacter(U'》');
}

bool TypesettingRules::loadFromFile(const std::string& filePath) {
    // 簡易的なファイル読み込み実装
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open typesetting rules file: " << filePath << std::endl;
        return false;
    }

    // ファイルフォーマットの例：
    // [LineStartProhibited]
    // U+3001,U+3002,U+FF0C,U+FF0E
    // [LineEndProhibited]
    // U+FF08,U+3010,U+FF5B
    // [Inseparable]
    // U+0024,U+FFE5,U+FFE1
    // [Hanging]
    // U+3001,U+3002,U+FF0C,U+FF0E

    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        // 空行をスキップ
        if (line.empty()) {
            continue;
        }

        // セクションヘッダーの処理
        if (line[0] == '[' && line[line.size() - 1] == ']') {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        // セクション内容の処理
        if (!currentSection.empty()) {
            std::istringstream iss(line);
            std::string token;
            
            while (std::getline(iss, token, ',')) {
                // U+XXXX形式の文字コードを解析
                if (token.substr(0, 2) == "U+") {
                    std::string hexCode = token.substr(2);
                    char32_t character = static_cast<char32_t>(std::stoul(hexCode, nullptr, 16));
                    
                    if (currentSection == "LineStartProhibited") {
                        addLineStartProhibitedCharacter(character);
                    } else if (currentSection == "LineEndProhibited") {
                        addLineEndProhibitedCharacter(character);
                    } else if (currentSection == "Inseparable") {
                        addInseparableCharacter(character);
                    } else if (currentSection == "Hanging") {
                        addHangingCharacter(character);
                    }
                }
            }
        }
    }

    file.close();
    return true;
}

bool TypesettingRules::saveToFile(const std::string& filePath) const {
    // 簡易的なファイル保存実装
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    // 行頭禁則文字の書き込み
    file << "[LineStartProhibited]\n";
    for (auto it = m_lineStartProhibitedChars.begin(); it != m_lineStartProhibitedChars.end(); ++it) {
        file << "U+" << std::hex << std::uppercase << static_cast<uint32_t>(*it);
        if (std::next(it) != m_lineStartProhibitedChars.end()) {
            file << ",";
        }
    }
    file << "\n\n";

    // 行末禁則文字の書き込み
    file << "[LineEndProhibited]\n";
    for (auto it = m_lineEndProhibitedChars.begin(); it != m_lineEndProhibitedChars.end(); ++it) {
        file << "U+" << std::hex << std::uppercase << static_cast<uint32_t>(*it);
        if (std::next(it) != m_lineEndProhibitedChars.end()) {
            file << ",";
        }
    }
    file << "\n\n";

    // 分離禁止文字の書き込み
    file << "[Inseparable]\n";
    for (auto it = m_inseparableChars.begin(); it != m_inseparableChars.end(); ++it) {
        file << "U+" << std::hex << std::uppercase << static_cast<uint32_t>(*it);
        if (std::next(it) != m_inseparableChars.end()) {
            file << ",";
        }
    }
    file << "\n\n";

    // ぶら下げ対象文字の書き込み
    file << "[Hanging]\n";
    for (auto it = m_hangingChars.begin(); it != m_hangingChars.end(); ++it) {
        file << "U+" << std::hex << std::uppercase << static_cast<uint32_t>(*it);
        if (std::next(it) != m_hangingChars.end()) {
            file << ",";
        }
    }
    file << "\n";

    file.close();
    return true;
}

} // namespace typesetting
} // namespace core
} // namespace japanese_typesetting
