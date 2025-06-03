/**
 * @file plugin_test.cpp
 * @brief プラグインシステムのテストコード
 */

#include <gtest/gtest.h>
#include "japanese_typesetting/plugin/plugin_manager.h"
#include <memory>

using namespace japanese_typesetting::plugin;

// テスト用のモックプラグインクラス
class MockPlugin : public Plugin {
public:
    MockPlugin(const std::string& id, PluginType type) : m_id(id), m_type(type), m_enabled(false) {}
    
    PluginInfo getInfo() const override {
        PluginInfo info;
        info.id = m_id;
        info.name = "Mock Plugin " + m_id;
        info.version = "1.0.0";
        info.author = "Test Author";
        info.description = "Test plugin for unit testing";
        info.type = m_type;
        info.apiVersion = "1.0";
        info.dependencies = m_dependencies;
        info.enabled = m_enabled;
        return info;
    }
    
    bool initialize() override {
        m_initialized = true;
        return true;
    }
    
    void shutdown() override {
        m_initialized = false;
    }
    
    bool enable() override {
        m_enabled = true;
        return true;
    }
    
    bool disable() override {
        m_enabled = false;
        return true;
    }
    
    bool isEnabled() const override {
        return m_enabled;
    }
    
    std::any getConfig(const std::string& key) const override {
        auto it = m_config.find(key);
        if (it != m_config.end()) {
            return it->second;
        }
        return std::any();
    }
    
    bool setConfig(const std::string& key, const std::any& value) override {
        m_config[key] = value;
        return true;
    }
    
    void addDependency(const std::string& dependencyId) {
        m_dependencies.push_back(dependencyId);
    }
    
    bool isInitialized() const {
        return m_initialized;
    }
    
private:
    std::string m_id;
    PluginType m_type;
    bool m_enabled;
    bool m_initialized = false;
    std::vector<std::string> m_dependencies;
    std::map<std::string, std::any> m_config;
};

// プラグインマネージャーのテスト
TEST(PluginManagerTest, RegisterPlugin) {
    // プラグインマネージャーのインスタンスを取得
    PluginManager& manager = PluginManager::getInstance();
    
    // テスト用プラグインを作成
    auto plugin = std::make_shared<MockPlugin>("test.plugin", PluginType::INPUT_FILTER);
    
    // プラグインを登録
    EXPECT_TRUE(manager.registerPlugin(plugin));
    
    // 登録されたプラグインを取得
    auto retrievedPlugin = manager.getPlugin("test.plugin");
    EXPECT_NE(retrievedPlugin, nullptr);
    
    // 登録解除
    EXPECT_TRUE(manager.unregisterPlugin("test.plugin"));
    
    // 登録解除後は取得できないことを確認
    retrievedPlugin = manager.getPlugin("test.plugin");
    EXPECT_EQ(retrievedPlugin, nullptr);
}

// プラグインの有効化・無効化のテスト
TEST(PluginManagerTest, EnableDisablePlugin) {
    PluginManager& manager = PluginManager::getInstance();
    
    auto plugin = std::make_shared<MockPlugin>("test.plugin", PluginType::OUTPUT_FILTER);
    EXPECT_TRUE(manager.registerPlugin(plugin));
    
    // 初期状態は無効
    EXPECT_FALSE(manager.isPluginEnabled("test.plugin"));
    
    // 有効化
    EXPECT_TRUE(manager.enablePlugin("test.plugin"));
    EXPECT_TRUE(manager.isPluginEnabled("test.plugin"));
    
    // 無効化
    EXPECT_TRUE(manager.disablePlugin("test.plugin"));
    EXPECT_FALSE(manager.isPluginEnabled("test.plugin"));
    
    // 登録解除
    EXPECT_TRUE(manager.unregisterPlugin("test.plugin"));
}

// プラグインの種類によるフィルタリングのテスト
TEST(PluginManagerTest, GetPluginsByType) {
    PluginManager& manager = PluginManager::getInstance();
    
    // 異なる種類のプラグインを登録
    auto plugin1 = std::make_shared<MockPlugin>("test.plugin1", PluginType::INPUT_FILTER);
    auto plugin2 = std::make_shared<MockPlugin>("test.plugin2", PluginType::OUTPUT_FILTER);
    auto plugin3 = std::make_shared<MockPlugin>("test.plugin3", PluginType::INPUT_FILTER);
    
    EXPECT_TRUE(manager.registerPlugin(plugin1));
    EXPECT_TRUE(manager.registerPlugin(plugin2));
    EXPECT_TRUE(manager.registerPlugin(plugin3));
    
    // 種類によるフィルタリング
    auto inputPlugins = manager.getPluginsByType(PluginType::INPUT_FILTER);
    EXPECT_EQ(inputPlugins.size(), 2);
    
    auto outputPlugins = manager.getPluginsByType(PluginType::OUTPUT_FILTER);
    EXPECT_EQ(outputPlugins.size(), 1);
    
    // すべてのプラグインを取得
    auto allPlugins = manager.getAllPlugins();
    EXPECT_EQ(allPlugins.size(), 3);
    
    // 登録解除
    EXPECT_TRUE(manager.unregisterPlugin("test.plugin1"));
    EXPECT_TRUE(manager.unregisterPlugin("test.plugin2"));
    EXPECT_TRUE(manager.unregisterPlugin("test.plugin3"));
}

// プラグインの設定のテスト
TEST(PluginManagerTest, PluginConfig) {
    PluginManager& manager = PluginManager::getInstance();
    
    auto plugin = std::make_shared<MockPlugin>("test.plugin", PluginType::TYPESETTING_RULE);
    EXPECT_TRUE(manager.registerPlugin(plugin));
    
    // 設定の設定
    EXPECT_TRUE(manager.setPluginConfig("test.plugin", "string_value", std::string("test")));
    EXPECT_TRUE(manager.setPluginConfig("test.plugin", "int_value", 42));
    EXPECT_TRUE(manager.setPluginConfig("test.plugin", "bool_value", true));
    
    // 設定の取得
    EXPECT_EQ(std::any_cast<std::string>(manager.getPluginConfig("test.plugin", "string_value")), "test");
    EXPECT_EQ(std::any_cast<int>(manager.getPluginConfig("test.plugin", "int_value")), 42);
    EXPECT_EQ(std::any_cast<bool>(manager.getPluginConfig("test.plugin", "bool_value")), true);
    
    // 登録解除
    EXPECT_TRUE(manager.unregisterPlugin("test.plugin"));
}

// 依存関係のテスト
TEST(PluginManagerTest, Dependencies) {
    PluginManager& manager = PluginManager::getInstance();
    
    // 依存されるプラグイン
    auto dependency = std::make_shared<MockPlugin>("test.dependency", PluginType::STYLE_EXTENSION);
    EXPECT_TRUE(manager.registerPlugin(dependency));
    EXPECT_TRUE(manager.enablePlugin("test.dependency"));
    
    // 依存するプラグイン
    auto dependent = std::make_shared<MockPlugin>("test.dependent", PluginType::UI_EXTENSION);
    static_cast<MockPlugin*>(dependent.get())->addDependency("test.dependency");
    
    // 依存関係が解決されることを確認
    EXPECT_TRUE(manager.registerPlugin(dependent));
    
    // 依存関係の登録解除
    EXPECT_TRUE(manager.unregisterPlugin("test.dependent"));
    EXPECT_TRUE(manager.unregisterPlugin("test.dependency"));
}

// 初期化と終了のテスト
TEST(PluginManagerTest, InitializeShutdown) {
    PluginManager& manager = PluginManager::getInstance();
    
    auto plugin1 = std::make_shared<MockPlugin>("test.plugin1", PluginType::COMMAND_EXTENSION);
    auto plugin2 = std::make_shared<MockPlugin>("test.plugin2", PluginType::OTHER);
    
    EXPECT_TRUE(manager.registerPlugin(plugin1));
    EXPECT_TRUE(manager.registerPlugin(plugin2));
    
    // 初期化の確認
    EXPECT_TRUE(static_cast<MockPlugin*>(plugin1.get())->isInitialized());
    EXPECT_TRUE(static_cast<MockPlugin*>(plugin2.get())->isInitialized());
    
    // すべてのプラグインを終了
    manager.shutdownAllPlugins();
    
    // 終了後は取得できないことを確認
    EXPECT_EQ(manager.getPlugin("test.plugin1"), nullptr);
    EXPECT_EQ(manager.getPlugin("test.plugin2"), nullptr);
}
