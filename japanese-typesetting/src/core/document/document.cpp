/**
 * @file document.cpp
 * @brief 文書構造の詳細実装
 */

#include "japanese_typesetting/core/document/document.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace japanese_typesetting {
namespace core {
namespace document {

// Section クラスの実装

Section::Section(const std::string& title)
    : m_title(title) {
}

Section::~Section() {
    // 子セクションの解放
    for (auto* section : m_childSections) {
        delete section;
    }
    m_childSections.clear();
}

std::string Section::getTitle() const {
    return m_title;
}

void Section::setTitle(const std::string& title) {
    m_title = title;
}

std::string Section::getContent() const {
    return m_content;
}

void Section::setContent(const std::string& content) {
    m_content = content;
}

void Section::addChildSection(Section* section) {
    if (section) {
        m_childSections.push_back(section);
    }
}

Section* Section::getChildSection(size_t index) const {
    if (index < m_childSections.size()) {
        return m_childSections[index];
    }
    return nullptr;
}

size_t Section::getChildSectionCount() const {
    return m_childSections.size();
}

void Section::setMetadata(const std::string& key, const std::string& value) {
    m_metadata[key] = value;
}

std::string Section::getMetadata(const std::string& key) const {
    auto it = m_metadata.find(key);
    if (it != m_metadata.end()) {
        return it->second;
    }
    return "";
}

// Document クラスの実装

Document::Document()
    : m_title("")
    , m_author("")
    , m_vertical(true) {
}

Document::Document(const std::string& title, const std::string& author, bool vertical)
    : m_title(title)
    , m_author(author)
    , m_vertical(vertical) {
}

Document::~Document() {
    // セクションの解放
    for (auto* section : m_sections) {
        delete section;
    }
    m_sections.clear();
}

std::string Document::getTitle() const {
    return m_title;
}

void Document::setTitle(const std::string& title) {
    m_title = title;
}

std::string Document::getAuthor() const {
    return m_author;
}

void Document::setAuthor(const std::string& author) {
    m_author = author;
}

bool Document::isVertical() const {
    return m_vertical;
}

void Document::setVertical(bool vertical) {
    m_vertical = vertical;
}

void Document::addSection(Section* section) {
    if (section) {
        m_sections.push_back(section);
    }
}

Section* Document::getSection(size_t index) const {
    if (index < m_sections.size()) {
        return m_sections[index];
    }
    return nullptr;
}

size_t Document::getSectionCount() const {
    return m_sections.size();
}

void Document::setMetadata(const std::string& key, const std::string& value) {
    m_metadata[key] = value;
}

std::string Document::getMetadata(const std::string& key) const {
    auto it = m_metadata.find(key);
    if (it != m_metadata.end()) {
        return it->second;
    }
    return "";
}

bool Document::loadFromFile(const std::string& filePath) {
    // 簡易的なファイル読み込み実装
    // 実際の実装では、XML、JSON、独自フォーマットなどを使用する
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    // ファイルフォーマットの例：
    // Title: ドキュメントタイトル
    // Author: 著者名
    // Vertical: true
    // Metadata-Key1: Value1
    // Metadata-Key2: Value2
    // ---
    // # セクション1タイトル
    // セクション1の内容
    // ---
    // # セクション2タイトル
    // セクション2の内容
    // ...

    std::string line;
    bool inHeader = true;
    Section* currentSection = nullptr;
    std::stringstream sectionContent;

    while (std::getline(file, line)) {
        if (inHeader) {
            if (line == "---") {
                inHeader = false;
                continue;
            }

            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                
                // 先頭と末尾の空白を削除
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                if (key == "Title") {
                    setTitle(value);
                } else if (key == "Author") {
                    setAuthor(value);
                } else if (key == "Vertical") {
                    setVertical(value == "true");
                } else if (key.substr(0, 9) == "Metadata-") {
                    setMetadata(key.substr(9), value);
                }
            }
        } else {
            if (line == "---") {
                // セクションの区切り
                if (currentSection) {
                    currentSection->setContent(sectionContent.str());
                    addSection(currentSection);
                    sectionContent.str("");
                    sectionContent.clear();
                }
                currentSection = new Section();
                continue;
            }

            if (currentSection) {
                if (line.size() > 0 && line[0] == '#') {
                    // セクションタイトル
                    currentSection->setTitle(line.substr(1));
                } else {
                    // セクション内容
                    sectionContent << line << "\n";
                }
            }
        }
    }

    // 最後のセクションを追加
    if (currentSection) {
        currentSection->setContent(sectionContent.str());
        addSection(currentSection);
    }

    file.close();
    return true;
}

bool Document::saveToFile(const std::string& filePath) const {
    // 簡易的なファイル保存実装
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return false;
    }

    // ヘッダー情報の書き込み
    file << "Title: " << m_title << "\n";
    file << "Author: " << m_author << "\n";
    file << "Vertical: " << (m_vertical ? "true" : "false") << "\n";
    
    // メタデータの書き込み
    for (const auto& meta : m_metadata) {
        file << "Metadata-" << meta.first << ": " << meta.second << "\n";
    }
    
    file << "---\n";

    // セクションの書き込み
    for (const auto* section : m_sections) {
        file << "---\n";
        file << "# " << section->getTitle() << "\n";
        file << section->getContent();
        
        // 子セクションは現在サポートしていない
    }

    file.close();
    return true;
}

} // namespace document
} // namespace core
} // namespace japanese_typesetting
