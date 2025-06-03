/**
 * @file plugin_loader.h
 * @brief プラグイン読み込み機能の実装
 */

#ifndef JAPANESE_TYPESETTING_PLUGIN_LOADER_H
#define JAPANESE_TYPESETTING_PLUGIN_LOADER_H

#include "japanese_typesetting/plugin/plugin_manager.h"
#include <string>
#include <vector>
#include <map>

namespace japanese_typesetting {
namespace plugin {

/**
 * @class PluginLoader
 * @brief プラグインの読み込みを管理するクラス
 */
class PluginLoader {
public:
    /**
     * @brief シングルトンインスタンスを取得する
     * @return PluginLoaderのインスタンス
     */
    static PluginLoader& getInstance();

    /**
     * @brief プラグインディレクトリを追加する
     * @param directory ディレクトリパス
     * @return 成功した場合はtrue
     */
    bool addPluginDirectory(const std::string& directory);

    /**
     * @brief プラグインディレクトリを削除する
     * @param directory ディレクトリパス
     * @return 成功した場合はtrue
     */
    bool removePluginDirectory(const std::string& directory);

    /**
     * @brief すべてのプラグインディレクトリを取得する
     * @return プラグインディレクトリのリスト
     */
    std::vector<std::string> getPluginDirectories() const;

    /**
     * @brief すべてのプラグインディレクトリからプラグインを読み込む
     * @return 読み込まれたプラグイン数
     */
    int loadAllPlugins();

    /**
     * @brief 指定したディレクトリからプラグインを読み込む
     * @param directory ディレクトリパス
     * @return 読み込まれたプラグイン数
     */
    int loadPluginsFromDirectory(const std::string& directory);

    /**
     * @brief 指定したファイルからプラグインを読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadPluginFromFile(const std::string& filePath);

    /**
     * @brief プラグイン設定ファイルを読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadPluginConfig(const std::string& filePath);

    /**
     * @brief プラグイン設定ファイルを保存する
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool savePluginConfig(const std::string& filePath);

    /**
     * @brief プラグインの自動読み込みを有効にする
     * @param enabled 有効にする場合はtrue
     */
    void setAutoLoadEnabled(bool enabled);

    /**
     * @brief プラグインの自動読み込みが有効かどうかを取得する
     * @return 有効な場合はtrue
     */
    bool isAutoLoadEnabled() const;

    /**
     * @brief プラグインの読み込み状態をリセットする
     */
    void reset();

private:
    /**
     * @brief コンストラクタ
     */
    PluginLoader();

    /**
     * @brief デストラクタ
     */
    ~PluginLoader();

    /**
     * @brief プラグインファイルかどうかを判定する
     * @param filePath ファイルパス
     * @return プラグインファイルの場合はtrue
     */
    bool isPluginFile(const std::string& filePath) const;

    std::vector<std::string> m_pluginDirectories; ///< プラグインディレクトリのリスト
    std::map<std::string, bool> m_loadedPlugins;  ///< 読み込み済みプラグインのマップ
    bool m_autoLoadEnabled;                       ///< 自動読み込みが有効かどうか
};

} // namespace plugin
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_PLUGIN_LOADER_H
