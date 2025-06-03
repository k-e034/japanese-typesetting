/**
 * @file plugin_loader.cpp
 * @brief プラグイン読み込み機能の実装
 */

#include "japanese_typesetting/plugin/plugin_loader.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

namespace japanese_typesetting {
namespace plugin {

// シングルトンインスタンスの取得
PluginLoader& PluginLoader::getInstance() {
    static PluginLoader instance;
    return instance;
}

PluginLoader::PluginLoader() : m_autoLoadEnabled(false) {
    // デフォルトのプラグインディレクトリを追加
    std::string defaultPluginDir = "/usr/local/share/japanese_typesetting/plugins";
    m_pluginDirectories.push_back(defaultPluginDir);
    
    // ユーザーのホームディレクトリにあるプラグインディレクトリを追加
    const char* homeDir = std::getenv("HOME");
    if (homeDir) {
        std::string userPluginDir = std::string(homeDir) + "/.japanese_typesetting/plugins";
        m_pluginDirectories.push_back(userPluginDir);
    }
}

PluginLoader::~PluginLoader() {
    // 特に終了処理はない
}

bool PluginLoader::addPluginDirectory(const std::string& directory) {
    // ディレクトリが既に登録されているか確認
    auto it = std::find(m_pluginDirectories.begin(), m_pluginDirectories.end(), directory);
    if (it != m_pluginDirectories.end()) {
        return false; // 既に登録済み
    }
    
    // ディレクトリを追加
    m_pluginDirectories.push_back(directory);
    
    // 自動読み込みが有効な場合は、ディレクトリからプラグインを読み込む
    if (m_autoLoadEnabled) {
        loadPluginsFromDirectory(directory);
    }
    
    return true;
}

bool PluginLoader::removePluginDirectory(const std::string& directory) {
    // ディレクトリが登録されているか確認
    auto it = std::find(m_pluginDirectories.begin(), m_pluginDirectories.end(), directory);
    if (it == m_pluginDirectories.end()) {
        return false; // 登録されていない
    }
    
    // ディレクトリを削除
    m_pluginDirectories.erase(it);
    return true;
}

std::vector<std::string> PluginLoader::getPluginDirectories() const {
    return m_pluginDirectories;
}

int PluginLoader::loadAllPlugins() {
    int totalCount = 0;
    
    // すべてのプラグインディレクトリからプラグインを読み込む
    for (const auto& directory : m_pluginDirectories) {
        totalCount += loadPluginsFromDirectory(directory);
    }
    
    return totalCount;
}

int PluginLoader::loadPluginsFromDirectory(const std::string& directory) {
    // プラグインマネージャーの取得
    PluginManager& manager = PluginManager::getInstance();
    
    // ディレクトリの存在確認
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        std::cerr << "ディレクトリ '" << directory << "' が存在しないか、ディレクトリではありません" << std::endl;
        return 0;
    }
    
    int count = 0;
    
    // ディレクトリ内のすべてのファイルを走査
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && isPluginFile(entry.path().string())) {
            // プラグインファイルの場合は読み込む
            if (loadPluginFromFile(entry.path().string())) {
                count++;
            }
        }
    }
    
    std::cout << directory << " から " << count << " 個のプラグインを読み込みました" << std::endl;
    return count;
}

bool PluginLoader::loadPluginFromFile(const std::string& filePath) {
    // ファイルが既に読み込まれているか確認
    if (m_loadedPlugins.find(filePath) != m_loadedPlugins.end()) {
        return false; // 既に読み込み済み
    }
    
    // プラグインマネージャーの取得
    PluginManager& manager = PluginManager::getInstance();
    
    // プラグインの読み込み
    if (manager.loadPluginFromFile(filePath)) {
        // 読み込み成功
        m_loadedPlugins[filePath] = true;
        return true;
    }
    
    return false;
}

bool PluginLoader::loadPluginConfig(const std::string& filePath) {
    // プラグインマネージャーの取得
    PluginManager& manager = PluginManager::getInstance();
    
    // プラグイン設定の読み込み
    return manager.loadPluginConfigFromFile(filePath);
}

bool PluginLoader::savePluginConfig(const std::string& filePath) {
    // プラグインマネージャーの取得
    PluginManager& manager = PluginManager::getInstance();
    
    // プラグイン設定の保存
    return manager.savePluginConfigToFile(filePath);
}

void PluginLoader::setAutoLoadEnabled(bool enabled) {
    m_autoLoadEnabled = enabled;
    
    // 自動読み込みが有効になった場合は、すべてのプラグインを読み込む
    if (m_autoLoadEnabled) {
        loadAllPlugins();
    }
}

bool PluginLoader::isAutoLoadEnabled() const {
    return m_autoLoadEnabled;
}

void PluginLoader::reset() {
    // プラグインマネージャーの取得
    PluginManager& manager = PluginManager::getInstance();
    
    // すべてのプラグインを終了
    manager.shutdownAllPlugins();
    
    // 読み込み状態をリセット
    m_loadedPlugins.clear();
}

bool PluginLoader::isPluginFile(const std::string& filePath) const {
    // ファイル拡張子の確認
    std::string extension = std::filesystem::path(filePath).extension().string();
    
    // 共有ライブラリファイルのみを対象とする
    return (extension == ".so" || extension == ".dll" || extension == ".dylib");
}

} // namespace plugin
} // namespace japanese_typesetting
