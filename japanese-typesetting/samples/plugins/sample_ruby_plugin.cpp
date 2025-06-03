/**
 * @file sample_ruby_plugin.cpp
 * @brief ルビ処理拡張サンプルプラグイン
 */

#include "sample_ruby_plugin.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <nlohmann/json.hpp>

namespace japanese_typesetting {
namespace plugin {
namespace sample {

SampleRubyPlugin::SampleRubyPlugin() : m_enabled(false) {
    // デフォルトの漢字とルビのマッピングを追加
    m_rubyMap["日本語"] = "にほんご";
    m_rubyMap["漢字"] = "かんじ";
    m_rubyMap["仮名"] = "かな";
    m_rubyMap["組版"] = "くみはん";
    m_rubyMap["禁則"] = "きんそく";
    m_rubyMap["文字詰め"] = "もじづめ";
    m_rubyMap["行分割"] = "ぎょうぶんかつ";
    m_rubyMap["縦書き"] = "たてがき";
    m_rubyMap["横書き"] = "よこがき";
    
    // デフォルト設定
    m_config["autoApply"] = true;
    m_config["rubyFormat"] = "《》";
    m_config["minKanjiLength"] = 2;
}

SampleRubyPlugin::~SampleRubyPlugin() {
    // 特に終了処理はない
}

PluginInfo SampleRubyPlugin::getInfo() const {
    PluginInfo info;
    info.id = "jp.typesetting.sample.ruby";
    info.name = "サンプルルビプラグイン";
    info.version = "1.0.0";
    info.author = "日本語組版プロジェクト";
    info.description = "特定の漢字に対して自動的にルビを振るサンプルプラグイン";
    info.type = PluginType::TYPESETTING_RULE;
    info.apiVersion = "1.0";
    info.enabled = m_enabled;
    return info;
}

bool SampleRubyPlugin::initialize() {
    std::cout << "サンプルルビプラグインを初期化しています..." << std::endl;
    return true;
}

void SampleRubyPlugin::shutdown() {
    std::cout << "サンプルルビプラグインを終了しています..." << std::endl;
    m_enabled = false;
}

bool SampleRubyPlugin::enable() {
    std::cout << "サンプルルビプラグインを有効化しています..." << std::endl;
    m_enabled = true;
    return true;
}

bool SampleRubyPlugin::disable() {
    std::cout << "サンプルルビプラグインを無効化しています..." << std::endl;
    m_enabled = false;
    return true;
}

bool SampleRubyPlugin::isEnabled() const {
    return m_enabled;
}

std::any SampleRubyPlugin::getConfig(const std::string& key) const {
    auto it = m_config.find(key);
    if (it != m_config.end()) {
        return it->second;
    }
    return std::any();
}

bool SampleRubyPlugin::setConfig(const std::string& key, const std::any& value) {
    m_config[key] = value;
    return true;
}

std::string SampleRubyPlugin::applyRuby(const std::string& text) {
    if (!m_enabled) {
        return text;
    }
    
    std::string result = text;
    
    // 自動適用が無効な場合は何もしない
    bool autoApply = std::any_cast<bool>(getConfig("autoApply"));
    if (!autoApply) {
        return result;
    }
    
    // ルビフォーマットを取得
    std::string rubyFormat = std::any_cast<std::string>(getConfig("rubyFormat"));
    std::string rubyOpen = rubyFormat.substr(0, 1);
    std::string rubyClose = rubyFormat.substr(1, 1);
    
    // 最小漢字長を取得
    int minKanjiLength = std::any_cast<int>(getConfig("minKanjiLength"));
    
    // すべての漢字とルビのマッピングを適用
    for (const auto& [kanji, ruby] : m_rubyMap) {
        // 最小漢字長以上の場合のみ適用
        if (kanji.length() >= minKanjiLength) {
            // 正規表現を使用して置換
            std::regex pattern(kanji);
            std::string replacement = kanji + rubyOpen + ruby + rubyClose;
            result = std::regex_replace(result, pattern, replacement);
        }
    }
    
    return result;
}

void SampleRubyPlugin::addRubyMapping(const std::string& kanji, const std::string& ruby) {
    m_rubyMap[kanji] = ruby;
}

bool SampleRubyPlugin::removeRubyMapping(const std::string& kanji) {
    auto it = m_rubyMap.find(kanji);
    if (it != m_rubyMap.end()) {
        m_rubyMap.erase(it);
        return true;
    }
    return false;
}

std::string SampleRubyPlugin::getRuby(const std::string& kanji) const {
    auto it = m_rubyMap.find(kanji);
    if (it != m_rubyMap.end()) {
        return it->second;
    }
    return "";
}

std::map<std::string, std::string> SampleRubyPlugin::getAllRubyMappings() const {
    return m_rubyMap;
}

bool SampleRubyPlugin::loadRubyMappingsFromFile(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "ファイル '" << filePath << "' を開けませんでした" << std::endl;
            return false;
        }
        
        nlohmann::json jsonData;
        file >> jsonData;
        
        // 既存のマッピングをクリア
        m_rubyMap.clear();
        
        // JSONからマッピングを読み込む
        for (auto& [kanji, ruby] : jsonData.items()) {
            m_rubyMap[kanji] = ruby.get<std::string>();
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "ルビマッピングの読み込み中にエラーが発生しました: " << e.what() << std::endl;
        return false;
    }
}

bool SampleRubyPlugin::saveRubyMappingsToFile(const std::string& filePath) {
    try {
        nlohmann::json jsonData;
        
        // マッピングをJSONに変換
        for (const auto& [kanji, ruby] : m_rubyMap) {
            jsonData[kanji] = ruby;
        }
        
        // JSONをファイルに書き込む
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "ファイル '" << filePath << "' を開けませんでした" << std::endl;
            return false;
        }
        
        file << jsonData.dump(4); // インデント4でフォーマット
        return true;
    } catch (const std::exception& e) {
        std::cerr << "ルビマッピングの保存中にエラーが発生しました: " << e.what() << std::endl;
        return false;
    }
}

} // namespace sample
} // namespace plugin
} // namespace japanese_typesetting
