/**
 * @file sample_ruby_plugin.h
 * @brief ルビ処理拡張サンプルプラグイン
 */

#ifndef JAPANESE_TYPESETTING_SAMPLE_RUBY_PLUGIN_H
#define JAPANESE_TYPESETTING_SAMPLE_RUBY_PLUGIN_H

#include "japanese_typesetting/plugin/plugin_manager.h"
#include <string>
#include <map>
#include <any>

namespace japanese_typesetting {
namespace plugin {
namespace sample {

/**
 * @class SampleRubyPlugin
 * @brief ルビ処理を拡張するサンプルプラグイン
 * 
 * このプラグインは、特定のパターンのルビ処理を拡張するサンプルです。
 * 例えば、特定の漢字に対して自動的にルビを振る機能を提供します。
 */
class SampleRubyPlugin : public Plugin {
public:
    /**
     * @brief コンストラクタ
     */
    SampleRubyPlugin();
    
    /**
     * @brief デストラクタ
     */
    ~SampleRubyPlugin();
    
    /**
     * @brief プラグイン情報を取得する
     * @return プラグイン情報
     */
    PluginInfo getInfo() const override;
    
    /**
     * @brief プラグインを初期化する
     * @return 成功した場合はtrue
     */
    bool initialize() override;
    
    /**
     * @brief プラグインを終了する
     */
    void shutdown() override;
    
    /**
     * @brief プラグインを有効にする
     * @return 成功した場合はtrue
     */
    bool enable() override;
    
    /**
     * @brief プラグインを無効にする
     * @return 成功した場合はtrue
     */
    bool disable() override;
    
    /**
     * @brief プラグインが有効かどうかを取得する
     * @return 有効な場合はtrue
     */
    bool isEnabled() const override;
    
    /**
     * @brief プラグインの設定を取得する
     * @param key 設定キー
     * @return 設定値
     */
    std::any getConfig(const std::string& key) const override;
    
    /**
     * @brief プラグインの設定を設定する
     * @param key 設定キー
     * @param value 設定値
     * @return 成功した場合はtrue
     */
    bool setConfig(const std::string& key, const std::any& value) override;
    
    /**
     * @brief 漢字にルビを適用する
     * @param text 対象テキスト
     * @return ルビ適用後のテキスト
     */
    std::string applyRuby(const std::string& text);
    
    /**
     * @brief 漢字とルビのマッピングを追加する
     * @param kanji 漢字
     * @param ruby ルビ
     */
    void addRubyMapping(const std::string& kanji, const std::string& ruby);
    
    /**
     * @brief 漢字とルビのマッピングを削除する
     * @param kanji 漢字
     * @return 成功した場合はtrue
     */
    bool removeRubyMapping(const std::string& kanji);
    
    /**
     * @brief 漢字のルビを取得する
     * @param kanji 漢字
     * @return ルビ（マッピングがない場合は空文字列）
     */
    std::string getRuby(const std::string& kanji) const;
    
    /**
     * @brief すべての漢字とルビのマッピングを取得する
     * @return 漢字とルビのマッピング
     */
    std::map<std::string, std::string> getAllRubyMappings() const;
    
    /**
     * @brief 漢字とルビのマッピングをファイルから読み込む
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool loadRubyMappingsFromFile(const std::string& filePath);
    
    /**
     * @brief 漢字とルビのマッピングをファイルに保存する
     * @param filePath ファイルパス
     * @return 成功した場合はtrue
     */
    bool saveRubyMappingsToFile(const std::string& filePath);
    
private:
    bool m_enabled;                                 ///< プラグインが有効かどうか
    std::map<std::string, std::string> m_rubyMap;   ///< 漢字とルビのマッピング
    std::map<std::string, std::any> m_config;       ///< プラグインの設定
};

} // namespace sample
} // namespace plugin
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_SAMPLE_RUBY_PLUGIN_H
