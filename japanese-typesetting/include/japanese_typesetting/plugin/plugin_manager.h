/**
 * @file plugin_manager.h
 * @brief プラグインマネージャーの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_PLUGIN_PLUGIN_MANAGER_H
#define JAPANESE_TYPESETTING_PLUGIN_PLUGIN_MANAGER_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <any>

namespace japanese_typesetting {
namespace plugin {

/**
 * @enum PluginType
 * @brief プラグインの種類を表す列挙型
 */
enum class PluginType {
    INPUT_FILTER,      ///< 入力フィルタ
    OUTPUT_FILTER,     ///< 出力フィルタ
    TYPESETTING_RULE,  ///< 組版ルール
    STYLE_EXTENSION,   ///< スタイル拡張
    UI_EXTENSION,      ///< UI拡張
    COMMAND_EXTENSION, ///< コマンド拡張
    OTHER              ///< その他
};

/**
 * @struct PluginInfo
 * @brief プラグイン情報を表す構造体
 */
struct PluginInfo {
    std::string id;           ///< プラグインID
    std::string name;         ///< プラグイン名
    std::string version;      ///< バージョン
    std::string author;       ///< 作者
    std::string description;  ///< 説明
    PluginType type;          ///< プラグインの種類
    std::string apiVersion;   ///< 対応APIバージョン
    std::vector<std::string> dependencies; ///< 依存プラグイン
    bool enabled;             ///< 有効かどうか
};

/**
 * @class Plugin
 * @brief プラグインの基底クラス
 */
class Plugin {
public:
    /**
     * @brief コンストラクタ
     */
    Plugin() = default;

    /**
     * @brief デストラクタ
     */
    virtual ~Plugin() = default;

    /**
     * @brief プラグイン情報を取得する
     * @return プラグイン情報
     */
    virtual PluginInfo getInfo() const = 0;

    /**
     * @brief プラグインを初期化する
     * @return 成功した場合はtrue
     */
    virtual bool initialize() = 0;

    /**
     * @brief プラグインを終了する
     */
    virtual void shutdown() = 0;

    /**
     * @brief プラグインを有効にする
     * @return 成功した場合はtrue
     */
    virtual bool enable() = 0;

    /**
     * @brief プラグインを無効にする
     * @return 成功した場合はtrue
     */
    virtual bool disable() = 0;

    /**
     * @brief プラグインが有効かどうかを取得する
     * @return 有効な場合はtrue
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief プラグインの設定を取得する
     * @param key 設定キー
     * @return 設定値
     */
    virtual std::any getConfig(const std::string& key) const = 0;

    /**
     * @brief プラグインの設定を設定する
     * @param key 設定キー
     * @param value 設定値
     * @return 成功した場合はtrue
     */
    virtual bool setConfig(const std::string& key, const std::any& value) = 0;
};

/**
 * @class InputFilterPlugin
 * @brief 入力フィルタプラグインの基底クラス
 */
class InputFilterPlugin : public Plugin {
public:
    /**
     * @brief 入力を処理する
     * @param input 入力テキスト
     * @return 処理後のテキスト
     */
    virtual std::string processInput(const std::string& input) = 0;
};

/**
 * @class OutputFilterPlugin
 * @brief 出力フィルタプラグインの基底クラス
 */
class OutputFilterPlugin : public Plugin {
public:
    /**
     * @brief 出力を処理する
     * @param output 出力テキスト
     * @return 処理後のテキスト
     */
    virtual std::string processOutput(const std::string& output) = 0;
};

/**
 * @class TypesettingRulePlugin
 * @brief 組版ルールプラグインの基底クラス
 */
class TypesettingRulePlugin : public Plugin {
public:
    /**
     * @brief 組版ルールを適用する
     * @param document 文書
     * @param style スタイル
     * @param blocks 組版されたテキストブロックのリスト
     * @return 処理後のテキストブロックのリスト
     */
    virtual std::vector<core::typesetting::TextBlock> applyRule(
        const core::document::Document& document,
        const core::style::Style& style,
        const std::vector<core::typesetting::TextBlock>& blocks) = 0;
};

/**
 * @class StyleExtensionPlugin
 * @brief スタイル拡張プラグインの基底クラス
 */
class StyleExtensionPlugin : public Plugin {
public:
    /**
     * @brief スタイルを拡張する
     * @param style スタイル
     * @return 拡張後のスタイル
     */
    virtual core::style::Style extendStyle(const core::style::Style& style) = 0;
};

/**
 * @class PluginManager
 * @brief プラグインマネージャークラス
 */
class PluginManager {
public:
    /**
     * @brief シングルトンインスタンスを取得する
     * @return プラグインマネージャーのインスタンス
     */
    static PluginManager& getInstance();

    /**
     * @brief プラグインを登録する
     * @param plugin プラグイン
     * @return 成功した場合はtrue
     */
    bool registerPlugin(std::shared_ptr<Plugin> plugin);

    /**
     * @brief プラグインを登録解除する
     * @param pluginId プラグインID
     * @return 成功した場合はtrue
     */
    bool unregisterPlugin(const std::string& pluginId);

    /**
     * @brief プラグインを取得する
     * @param pluginId プラグインID
     * @return プラグイン
     */
    std::shared_ptr<Plugin> getPlugin(const std::string& pluginId) const;

    /**
     * @brief 指定した種類のプラグインをすべて取得する
     * @param type プラグインの種類
     * @return プラグインのリスト
     */
    std::vector<std::shared_ptr<Plugin>> getPluginsByType(PluginType type) const;

    /**
     * @brief すべてのプラグインを取得する
     * @return プラグインのリスト
     */
    std::vector<std::shared_ptr<Plugin>> getAllPlugins() const;

    /**
     * @brief プラグインを有効にする
     * @param pluginId プラグインID
     * @return 成功した場合はtrue
     */
    bool enablePlugin(const std::string& pluginId);

    /**
     * @brief プラグインを無効にする
     * @param pluginId プラグインID
     * @return 成功した場合はtrue
     */
    bool disablePlugin(const std::string& pluginId);

    /**
     * @brief プラグインが有効かどうかを取得する
     * @param pluginId プラグインID
     * @return 有効な場合はtrue
     */
    bool isPluginEnabled(const std::string& pluginId) const;

    /**
     * @brief プラグインディレクトリからプラグインを読み込む
     * @param directory ディレクトリパス
     * @return 読み込まれたプラグイン数
     */
    int loadPluginsFromDirectory(const std::string& directory);

    /**
     * @brief プラグインファイルからプラグインを読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadPluginFromFile(const std::string& filePath);

    /**
     * @brief すべてのプラグインを初期化する
     * @return 成功した場合はtrue
     */
    bool initializeAllPlugins();

    /**
     * @brief すべてのプラグインを終了する
     */
    void shutdownAllPlugins();

    /**
     * @brief プラグインの設定を取得する
     * @param pluginId プラグインID
     * @param key 設定キー
     * @return 設定値
     */
    std::any getPluginConfig(const std::string& pluginId, const std::string& key) const;

    /**
     * @brief プラグインの設定を設定する
     * @param pluginId プラグインID
     * @param key 設定キー
     * @param value 設定値
     * @return 成功した場合はtrue
     */
    bool setPluginConfig(const std::string& pluginId, const std::string& key, const std::any& value);

    /**
     * @brief プラグインの設定をファイルから読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadPluginConfigFromFile(const std::string& filePath);

    /**
     * @brief プラグインの設定をファイルに保存する
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool savePluginConfigToFile(const std::string& filePath);

private:
    /**
     * @brief コンストラクタ
     */
    PluginManager();

    /**
     * @brief デストラクタ
     */
    ~PluginManager();

    /**
     * @brief プラグインの依存関係を解決する
     * @param plugin プラグイン
     * @return 成功した場合はtrue
     */
    bool resolveDependencies(std::shared_ptr<Plugin> plugin);

    std::map<std::string, std::shared_ptr<Plugin>> m_plugins; ///< プラグインマップ
};

} // namespace plugin
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_PLUGIN_PLUGIN_MANAGER_H
