/**
 * @file plugin_manager.cpp
 * @brief プラグインマネージャーの詳細実装
 */

#include "japanese_typesetting/plugin/plugin_manager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <dlfcn.h>
#include <nlohmann/json.hpp>

namespace japanese_typesetting {
namespace plugin {

// シングルトンインスタンスの取得
PluginManager& PluginManager::getInstance() {
    static PluginManager instance;
    return instance;
}

PluginManager::PluginManager() {
    // 特に初期化処理はない
}

PluginManager::~PluginManager() {
    // プラグインの終了処理
    shutdownAllPlugins();
}

bool PluginManager::registerPlugin(std::shared_ptr<Plugin> plugin) {
    if (!plugin) {
        std::cerr << "無効なプラグインです" << std::endl;
        return false;
    }
    
    PluginInfo info = plugin->getInfo();
    
    // 既に登録されているか確認
    if (m_plugins.find(info.id) != m_plugins.end()) {
        std::cerr << "プラグイン '" << info.id << "' は既に登録されています" << std::endl;
        return false;
    }
    
    // 依存関係の解決
    if (!resolveDependencies(plugin)) {
        std::cerr << "プラグイン '" << info.id << "' の依存関係を解決できませんでした" << std::endl;
        return false;
    }
    
    // プラグインの初期化
    if (!plugin->initialize()) {
        std::cerr << "プラグイン '" << info.id << "' の初期化に失敗しました" << std::endl;
        return false;
    }
    
    // プラグインの登録
    m_plugins[info.id] = plugin;
    
    // プラグインの有効化（設定に基づく）
    if (info.enabled) {
        if (!plugin->enable()) {
            std::cerr << "プラグイン '" << info.id << "' の有効化に失敗しました" << std::endl;
            return false;
        }
    }
    
    std::cout << "プラグイン '" << info.id << "' を登録しました" << std::endl;
    return true;
}

bool PluginManager::unregisterPlugin(const std::string& pluginId) {
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        std::cerr << "プラグイン '" << pluginId << "' は登録されていません" << std::endl;
        return false;
    }
    
    // プラグインの無効化
    if (it->second->isEnabled()) {
        if (!it->second->disable()) {
            std::cerr << "プラグイン '" << pluginId << "' の無効化に失敗しました" << std::endl;
            return false;
        }
    }
    
    // プラグインの終了処理
    it->second->shutdown();
    
    // プラグインの登録解除
    m_plugins.erase(it);
    
    std::cout << "プラグイン '" << pluginId << "' の登録を解除しました" << std::endl;
    return true;
}

std::shared_ptr<Plugin> PluginManager::getPlugin(const std::string& pluginId) const {
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        return nullptr;
    }
    
    return it->second;
}

std::vector<std::shared_ptr<Plugin>> PluginManager::getPluginsByType(PluginType type) const {
    std::vector<std::shared_ptr<Plugin>> result;
    
    for (const auto& [id, plugin] : m_plugins) {
        if (plugin->getInfo().type == type) {
            result.push_back(plugin);
        }
    }
    
    return result;
}

std::vector<std::shared_ptr<Plugin>> PluginManager::getAllPlugins() const {
    std::vector<std::shared_ptr<Plugin>> result;
    
    for (const auto& [id, plugin] : m_plugins) {
        result.push_back(plugin);
    }
    
    return result;
}

bool PluginManager::enablePlugin(const std::string& pluginId) {
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        std::cerr << "プラグイン '" << pluginId << "' は登録されていません" << std::endl;
        return false;
    }
    
    if (it->second->isEnabled()) {
        // 既に有効
        return true;
    }
    
    if (!it->second->enable()) {
        std::cerr << "プラグイン '" << pluginId << "' の有効化に失敗しました" << std::endl;
        return false;
    }
    
    std::cout << "プラグイン '" << pluginId << "' を有効化しました" << std::endl;
    return true;
}

bool PluginManager::disablePlugin(const std::string& pluginId) {
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        std::cerr << "プラグイン '" << pluginId << "' は登録されていません" << std::endl;
        return false;
    }
    
    if (!it->second->isEnabled()) {
        // 既に無効
        return true;
    }
    
    if (!it->second->disable()) {
        std::cerr << "プラグイン '" << pluginId << "' の無効化に失敗しました" << std::endl;
        return false;
    }
    
    std::cout << "プラグイン '" << pluginId << "' を無効化しました" << std::endl;
    return true;
}

bool PluginManager::isPluginEnabled(const std::string& pluginId) const {
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        return false;
    }
    
    return it->second->isEnabled();
}

int PluginManager::loadPluginsFromDirectory(const std::string& directory) {
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        std::cerr << "ディレクトリ '" << directory << "' が存在しないか、ディレクトリではありません" << std::endl;
        return 0;
    }
    
    int count = 0;
    
    // ディレクトリ内のすべてのファイルを走査
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            
            // 共有ライブラリファイルのみを処理
            if (extension == ".so" || extension == ".dll" || extension == ".dylib") {
                if (loadPluginFromFile(entry.path().string())) {
                    count++;
                }
            }
        }
    }
    
    std::cout << directory << " から " << count << " 個のプラグインを読み込みました" << std::endl;
    return count;
}

bool PluginManager::loadPluginFromFile(const std::string& filePath) {
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath)) {
        std::cerr << "ファイル '" << filePath << "' が存在しないか、通常のファイルではありません" << std::endl;
        return false;
    }
    
    // 共有ライブラリを開く
    void* handle = dlopen(filePath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "共有ライブラリ '" << filePath << "' を開けませんでした: " << dlerror() << std::endl;
        return false;
    }
    
    // プラグイン作成関数を取得
    using CreatePluginFunc = Plugin* (*)();
    CreatePluginFunc createPlugin = reinterpret_cast<CreatePluginFunc>(dlsym(handle, "createPlugin"));
    
    if (!createPlugin) {
        std::cerr << "プラグイン作成関数が見つかりませんでした: " << dlerror() << std::endl;
        dlclose(handle);
        return false;
    }
    
    // プラグインを作成
    Plugin* plugin = createPlugin();
    if (!plugin) {
        std::cerr << "プラグインの作成に失敗しました" << std::endl;
        dlclose(handle);
        return false;
    }
    
    // プラグインを登録
    std::shared_ptr<Plugin> pluginPtr(plugin);
    if (!registerPlugin(pluginPtr)) {
        dlclose(handle);
        return false;
    }
    
    std::cout << "プラグイン '" << plugin->getInfo().id << "' をファイル '" << filePath << "' から読み込みました" << std::endl;
    return true;
}

bool PluginManager::initializeAllPlugins() {
    bool success = true;
    
    for (auto& [id, plugin] : m_plugins) {
        if (!plugin->initialize()) {
            std::cerr << "プラグイン '" << id << "' の初期化に失敗しました" << std::endl;
            success = false;
        }
    }
    
    return success;
}

void PluginManager::shutdownAllPlugins() {
    for (auto& [id, plugin] : m_plugins) {
        if (plugin->isEnabled()) {
            plugin->disable();
        }
        plugin->shutdown();
    }
    
    m_plugins.clear();
}

std::any PluginManager::getPluginConfig(const std::string& pluginId, const std::string& key) const {
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        throw std::runtime_error("プラグイン '" + pluginId + "' は登録されていません");
    }
    
    return it->second->getConfig(key);
}

bool PluginManager::setPluginConfig(const std::string& pluginId, const std::string& key, const std::any& value) {
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        std::cerr << "プラグイン '" << pluginId << "' は登録されていません" << std::endl;
        return false;
    }
    
    return it->second->setConfig(key, value);
}

bool PluginManager::loadPluginConfigFromFile(const std::string& filePath) {
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath)) {
        std::cerr << "ファイル '" << filePath << "' が存在しないか、通常のファイルではありません" << std::endl;
        return false;
    }
    
    try {
        // JSONファイルを読み込む
        std::ifstream file(filePath);
        nlohmann::json config;
        file >> config;
        
        // 各プラグインの設定を適用
        for (auto& [pluginId, pluginConfig] : config.items()) {
            auto it = m_plugins.find(pluginId);
            if (it != m_plugins.end()) {
                for (auto& [key, value] : pluginConfig.items()) {
                    if (value.is_string()) {
                        it->second->setConfig(key, value.get<std::string>());
                    } else if (value.is_number_integer()) {
                        it->second->setConfig(key, value.get<int>());
                    } else if (value.is_number_float()) {
                        it->second->setConfig(key, value.get<double>());
                    } else if (value.is_boolean()) {
                        it->second->setConfig(key, value.get<bool>());
                    }
                }
            }
        }
        
        std::cout << "プラグイン設定をファイル '" << filePath << "' から読み込みました" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "プラグイン設定の読み込みに失敗しました: " << e.what() << std::endl;
        return false;
    }
}

bool PluginManager::savePluginConfigToFile(const std::string& filePath) {
    try {
        // 各プラグインの設定を収集
        nlohmann::json config;
        
        for (const auto& [id, plugin] : m_plugins) {
            nlohmann::json pluginConfig;
            
            // プラグインの設定キーを取得（実装依存）
            // ここでは仮の実装
            std::vector<std::string> keys = {"enabled", "priority"};
            
            for (const auto& key : keys) {
                try {
                    std::any value = plugin->getConfig(key);
                    
                    // std::anyから適切な型に変換（実装依存）
                    // ここでは仮の実装
                    if (value.type() == typeid(std::string)) {
                        pluginConfig[key] = std::any_cast<std::string>(value);
                    } else if (value.type() == typeid(int)) {
                        pluginConfig[key] = std::any_cast<int>(value);
                    } else if (value.type() == typeid(double)) {
                        pluginConfig[key] = std::any_cast<double>(value);
                    } else if (value.type() == typeid(bool)) {
                        pluginConfig[key] = std::any_cast<bool>(value);
                    }
                } catch (const std::exception& e) {
                    // 設定の取得に失敗した場合はスキップ
                }
            }
            
            config[id] = pluginConfig;
        }
        
        // JSONファイルに書き込む
        std::ofstream file(filePath);
        file << config.dump(4); // インデント4でフォーマット
        
        std::cout << "プラグイン設定をファイル '" << filePath << "' に保存しました" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "プラグイン設定の保存に失敗しました: " << e.what() << std::endl;
        return false;
    }
}

bool PluginManager::resolveDependencies(std::shared_ptr<Plugin> plugin) {
    PluginInfo info = plugin->getInfo();
    
    for (const auto& dependencyId : info.dependencies) {
        auto it = m_plugins.find(dependencyId);
        if (it == m_plugins.end()) {
            std::cerr << "プラグイン '" << info.id << "' の依存プラグイン '" << dependencyId << "' が見つかりません" << std::endl;
            return false;
        }
        
        if (!it->second->isEnabled()) {
            std::cerr << "プラグイン '" << info.id << "' の依存プラグイン '" << dependencyId << "' が無効です" << std::endl;
            return false;
        }
    }
    
    return true;
}

} // namespace plugin
} // namespace japanese_typesetting
