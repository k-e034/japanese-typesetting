# 日本語組版特化ソフトウェア プラグイン開発ガイド

## 1. はじめに

### 1.1 概要

本ドキュメントは、日本語組版特化ソフトウェアのプラグインAPIを使用して、独自の機能拡張を開発するためのガイドです。プラグインシステムの概要、APIの使用方法、サンプルコードなどを提供します。

### 1.2 プラグインシステムの特徴

- 動的ロード・アンロード
- 複数種類のプラグイン対応
- 依存関係の自動解決
- 設定の保存・読み込み
- イベント通知システム

### 1.3 プラグインの種類

- **入力フィルタ**: 入力文書の前処理
- **出力フィルタ**: 出力文書の後処理
- **組版ルール拡張**: 組版ルールのカスタマイズ
- **スタイル拡張**: スタイル定義の拡張
- **UIコンポーネント**: GUI拡張
- **コマンド拡張**: コマンドラインインターフェース拡張
- **その他**: 上記に該当しない拡張

## 2. プラグイン開発の準備

### 2.1 開発環境の構築

#### 必要なツールとライブラリ

- C++17対応コンパイラ (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10以上
- 日本語組版特化ソフトウェア開発キット (SDK)

#### SDKのインストール

```bash
# Ubuntuの場合
sudo apt-get install japanese-typesetting-dev

# 手動インストール
git clone https://github.com/japanese-typesetting/japanese-typesetting.git
cd japanese-typesetting
mkdir build && cd build
cmake .. -DBUILD_SDK=ON
make
sudo make install
```

### 2.2 プラグインプロジェクトの作成

#### プロジェクト構造

```
my_plugin/
├── CMakeLists.txt
├── include/
│   └── my_plugin.h
├── src/
│   └── my_plugin.cpp
└── test/
    └── my_plugin_test.cpp
```

#### CMakeLists.txtの例

```cmake
cmake_minimum_required(VERSION 3.10)
project(my_plugin)

# 日本語組版特化ソフトウェアのパッケージを検索
find_package(JapaneseTypesetting REQUIRED)

# プラグインのソースファイル
set(SOURCES
    src/my_plugin.cpp
)

# プラグインのヘッダーファイル
set(HEADERS
    include/my_plugin.h
)

# 共有ライブラリとしてプラグインをビルド
add_library(my_plugin SHARED ${SOURCES} ${HEADERS})

# インクルードディレクトリの設定
target_include_directories(my_plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${JAPANESE_TYPESETTING_INCLUDE_DIRS}
)

# 依存ライブラリのリンク
target_link_libraries(my_plugin PRIVATE
    ${JAPANESE_TYPESETTING_LIBRARIES}
)

# コンパイルオプションの設定
target_compile_features(my_plugin PRIVATE cxx_std_17)

# インストール設定
install(TARGETS my_plugin
    LIBRARY DESTINATION lib/japanese_typesetting/plugins
    RUNTIME DESTINATION lib/japanese_typesetting/plugins
)
```

## 3. プラグインAPIの基本

### 3.1 プラグインクラスの実装

すべてのプラグインは、`Plugin`基底クラスを継承する必要があります。

```cpp
#include "japanese_typesetting/plugin/plugin_manager.h"

namespace my_namespace {

class MyPlugin : public japanese_typesetting::plugin::Plugin {
public:
    // コンストラクタ
    MyPlugin();
    
    // デストラクタ
    ~MyPlugin();
    
    // プラグイン情報を取得する
    japanese_typesetting::plugin::PluginInfo getInfo() const override;
    
    // プラグインを初期化する
    bool initialize() override;
    
    // プラグインを終了する
    void shutdown() override;
    
    // プラグインを有効にする
    bool enable() override;
    
    // プラグインを無効にする
    bool disable() override;
    
    // プラグインが有効かどうかを取得する
    bool isEnabled() const override;
    
    // プラグインの設定を取得する
    std::any getConfig(const std::string& key) const override;
    
    // プラグインの設定を設定する
    bool setConfig(const std::string& key, const std::any& value) override;
    
    // プラグイン固有の機能
    void myCustomFunction();
    
private:
    bool m_enabled;
    std::map<std::string, std::any> m_config;
};

} // namespace my_namespace
```

### 3.2 プラグイン情報の定義

プラグイン情報は、`getInfo()`メソッドで返す必要があります。

```cpp
japanese_typesetting::plugin::PluginInfo MyPlugin::getInfo() const {
    japanese_typesetting::plugin::PluginInfo info;
    info.id = "com.example.my_plugin";
    info.name = "マイプラグイン";
    info.version = "1.0.0";
    info.author = "開発者名";
    info.description = "プラグインの説明";
    info.type = japanese_typesetting::plugin::PluginType::TYPESETTING_RULE;
    info.apiVersion = "1.0";
    info.dependencies = {"com.example.another_plugin"};
    info.enabled = m_enabled;
    return info;
}
```

### 3.3 プラグインのライフサイクル

1. **ロード**: プラグインファイルが読み込まれる
2. **初期化**: `initialize()`メソッドが呼ばれる
3. **有効化**: `enable()`メソッドが呼ばれる
4. **無効化**: `disable()`メソッドが呼ばれる
5. **終了**: `shutdown()`メソッドが呼ばれる
6. **アンロード**: プラグインファイルがアンロードされる

```cpp
bool MyPlugin::initialize() {
    // 初期化処理
    return true;
}

void MyPlugin::shutdown() {
    // 終了処理
}

bool MyPlugin::enable() {
    m_enabled = true;
    return true;
}

bool MyPlugin::disable() {
    m_enabled = false;
    return true;
}

bool MyPlugin::isEnabled() const {
    return m_enabled;
}
```

### 3.4 設定の管理

プラグインは、`getConfig()`と`setConfig()`メソッドを使用して設定を管理します。

```cpp
std::any MyPlugin::getConfig(const std::string& key) const {
    auto it = m_config.find(key);
    if (it != m_config.end()) {
        return it->second;
    }
    return std::any();
}

bool MyPlugin::setConfig(const std::string& key, const std::any& value) {
    m_config[key] = value;
    return true;
}
```

## 4. プラグイン種別ごとの実装

### 4.1 入力フィルタプラグイン

入力文書の前処理を行うプラグインです。

```cpp
class MyInputFilterPlugin : public japanese_typesetting::plugin::Plugin {
public:
    // 基本的なプラグインメソッド（省略）
    
    // 入力フィルタ処理
    std::string processInput(const std::string& input) {
        // 入力テキストの処理
        return processedInput;
    }
};
```

### 4.2 組版ルール拡張プラグイン

組版ルールをカスタマイズするプラグインです。

```cpp
class MyTypesettingRulePlugin : public japanese_typesetting::plugin::Plugin {
public:
    // 基本的なプラグインメソッド（省略）
    
    // 禁則処理のカスタマイズ
    bool isProhibitedLineStart(char32_t ch) {
        // 行頭禁則文字の判定
        return isProhibited;
    }
    
    // 文字詰め処理のカスタマイズ
    float getCharacterSpacing(char32_t prev, char32_t next) {
        // 文字間隔の計算
        return spacing;
    }
};
```

### 4.3 UIコンポーネントプラグイン

GUIを拡張するプラグインです。

```cpp
class MyUIPlugin : public japanese_typesetting::plugin::Plugin {
public:
    // 基本的なプラグインメソッド（省略）
    
    // UIコンポーネントの作成
    QWidget* createWidget(QWidget* parent) {
        // ウィジェットの作成
        return new MyCustomWidget(parent);
    }
    
    // メニュー項目の追加
    void addMenuItems(QMenu* menu) {
        // メニュー項目の追加
        QAction* action = menu->addAction("カスタム機能");
        QObject::connect(action, &QAction::triggered, []() {
            // アクション実行時の処理
        });
    }
};
```

## 5. プラグインの登録とエクスポート

### 5.1 プラグインのエクスポート

プラグインをエクスポートするには、以下のマクロを使用します。

```cpp
#include "japanese_typesetting/plugin/plugin_export.h"

// プラグインクラスの実装（省略）

// プラグインのエクスポート
JAPANESE_TYPESETTING_EXPORT_PLUGIN(my_namespace::MyPlugin)
```

### 5.2 複数プラグインのエクスポート

複数のプラグインをエクスポートする場合は、以下のようにします。

```cpp
JAPANESE_TYPESETTING_EXPORT_PLUGINS(
    my_namespace::MyPlugin1,
    my_namespace::MyPlugin2,
    my_namespace::MyPlugin3
)
```

## 6. プラグインのテスト

### 6.1 単体テスト

Google Testを使用した単体テストの例：

```cpp
#include <gtest/gtest.h>
#include "my_plugin.h"

// プラグインの基本機能テスト
TEST(MyPluginTest, BasicFunctionality) {
    my_namespace::MyPlugin plugin;
    
    // 初期状態は無効
    EXPECT_FALSE(plugin.isEnabled());
    
    // プラグイン情報の確認
    auto info = plugin.getInfo();
    EXPECT_EQ(info.id, "com.example.my_plugin");
    EXPECT_EQ(info.name, "マイプラグイン");
    
    // プラグインの有効化
    EXPECT_TRUE(plugin.enable());
    EXPECT_TRUE(plugin.isEnabled());
    
    // プラグインの無効化
    EXPECT_TRUE(plugin.disable());
    EXPECT_FALSE(plugin.isEnabled());
}

// プラグイン固有機能のテスト
TEST(MyPluginTest, CustomFunctionality) {
    my_namespace::MyPlugin plugin;
    plugin.enable();
    
    // プラグイン固有機能のテスト
    // ...
}
```

### 6.2 統合テスト

プラグインマネージャーと連携した統合テストの例：

```cpp
#include <gtest/gtest.h>
#include "japanese_typesetting/plugin/plugin_manager.h"
#include "my_plugin.h"

// プラグインマネージャーとの統合テスト
TEST(MyPluginIntegrationTest, PluginManager) {
    // プラグインマネージャーの取得
    auto& manager = japanese_typesetting::plugin::PluginManager::getInstance();
    
    // プラグインのインスタンス作成
    auto plugin = std::make_shared<my_namespace::MyPlugin>();
    
    // プラグインの登録
    EXPECT_TRUE(manager.registerPlugin(plugin));
    
    // プラグインの取得
    auto retrievedPlugin = manager.getPlugin("com.example.my_plugin");
    EXPECT_NE(retrievedPlugin, nullptr);
    
    // プラグインの有効化
    EXPECT_TRUE(manager.enablePlugin("com.example.my_plugin"));
    EXPECT_TRUE(manager.isPluginEnabled("com.example.my_plugin"));
    
    // プラグインの無効化
    EXPECT_TRUE(manager.disablePlugin("com.example.my_plugin"));
    EXPECT_FALSE(manager.isPluginEnabled("com.example.my_plugin"));
    
    // プラグインの登録解除
    EXPECT_TRUE(manager.unregisterPlugin("com.example.my_plugin"));
}
```

## 7. プラグインの配布

### 7.1 パッケージング

プラグインは、共有ライブラリ（.so/.dll/.dylib）として配布します。必要に応じて、設定ファイルやリソースファイルも含めます。

```
my_plugin/
├── libmy_plugin.so (または my_plugin.dll, libmy_plugin.dylib)
├── my_plugin.json (設定ファイル)
└── resources/ (リソースファイル)
```

### 7.2 インストール

プラグインは、以下のディレクトリにインストールします：

- システム全体: `/usr/local/lib/japanese_typesetting/plugins/`
- ユーザー個人: `~/.japanese_typesetting/plugins/`

### 7.3 メタデータ

プラグインのメタデータは、JSONファイルで提供することもできます：

```json
{
  "id": "com.example.my_plugin",
  "name": "マイプラグイン",
  "version": "1.0.0",
  "author": "開発者名",
  "description": "プラグインの説明",
  "type": "TYPESETTING_RULE",
  "apiVersion": "1.0",
  "dependencies": ["com.example.another_plugin"],
  "settings": {
    "option1": true,
    "option2": "デフォルト値",
    "option3": 42
  }
}
```

## 8. サンプルプラグイン

### 8.1 ルビ処理拡張プラグイン

特定の漢字に自動的にルビを振るプラグインの例：

```cpp
#include "japanese_typesetting/plugin/plugin_manager.h"
#include <string>
#include <map>
#include <regex>

namespace japanese_typesetting {
namespace plugin {
namespace sample {

class SampleRubyPlugin : public Plugin {
public:
    SampleRubyPlugin() : m_enabled(false) {
        // デフォルトの漢字とルビのマッピング
        m_rubyMap["日本語"] = "にほんご";
        m_rubyMap["漢字"] = "かんじ";
        
        // デフォルト設定
        m_config["autoApply"] = true;
        m_config["rubyFormat"] = "《》";
    }
    
    PluginInfo getInfo() const override {
        PluginInfo info;
        info.id = "jp.typesetting.sample.ruby";
        info.name = "サンプルルビプラグイン";
        info.version = "1.0.0";
        info.author = "日本語組版プロジェクト";
        info.description = "特定の漢字に自動的にルビを振るサンプルプラグイン";
        info.type = PluginType::TYPESETTING_RULE;
        info.apiVersion = "1.0";
        info.enabled = m_enabled;
        return info;
    }
    
    bool initialize() override { m_enabled = false; return true; }
    void shutdown() override { m_enabled = false; }
    bool enable() override { m_enabled = true; return true; }
    bool disable() override { m_enabled = false; return true; }
    bool isEnabled() const override { return m_enabled; }
    
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
    
    // ルビ適用処理
    std::string applyRuby(const std::string& text) {
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
        
        // すべての漢字とルビのマッピングを適用
        for (const auto& [kanji, ruby] : m_rubyMap) {
            // 正規表現を使用して置換
            std::regex pattern(kanji);
            std::string replacement = kanji + rubyOpen + ruby + rubyClose;
            result = std::regex_replace(result, pattern, replacement);
        }
        
        return result;
    }
    
    // 漢字とルビのマッピングを追加
    void addRubyMapping(const std::string& kanji, const std::string& ruby) {
        m_rubyMap[kanji] = ruby;
    }
    
private:
    bool m_enabled;
    std::map<std::string, std::string> m_rubyMap;
    std::map<std::string, std::any> m_config;
};

} // namespace sample
} // namespace plugin
} // namespace japanese_typesetting

// プラグインのエクスポート
JAPANESE_TYPESETTING_EXPORT_PLUGIN(japanese_typesetting::plugin::sample::SampleRubyPlugin)
```

### 8.2 文字変換プラグイン

旧字体・新字体の相互変換を行うプラグインの例：

```cpp
#include "japanese_typesetting/plugin/plugin_manager.h"
#include <string>
#include <map>

namespace japanese_typesetting {
namespace plugin {
namespace sample {

class CharacterConversionPlugin : public Plugin {
public:
    CharacterConversionPlugin() : m_enabled(false) {
        // 新字体→旧字体の変換マップ
        m_newToOldMap["国"] = "國";
        m_newToOldMap["学"] = "學";
        m_newToOldMap["広"] = "廣";
        // ... 他の文字も追加
        
        // 旧字体→新字体の変換マップ
        for (const auto& [newChar, oldChar] : m_newToOldMap) {
            m_oldToNewMap[oldChar] = newChar;
        }
        
        // デフォルト設定
        m_config["conversionMode"] = "newToOld"; // または "oldToNew"
    }
    
    // 基本的なプラグインメソッド（省略）
    
    // 文字変換処理
    std::string convertText(const std::string& text) {
        if (!m_enabled) {
            return text;
        }
        
        std::string result = text;
        std::string mode = std::any_cast<std::string>(getConfig("conversionMode"));
        
        if (mode == "newToOld") {
            // 新字体→旧字体の変換
            for (const auto& [newChar, oldChar] : m_newToOldMap) {
                size_t pos = 0;
                while ((pos = result.find(newChar, pos)) != std::string::npos) {
                    result.replace(pos, newChar.length(), oldChar);
                    pos += oldChar.length();
                }
            }
        } else if (mode == "oldToNew") {
            // 旧字体→新字体の変換
            for (const auto& [oldChar, newChar] : m_oldToNewMap) {
                size_t pos = 0;
                while ((pos = result.find(oldChar, pos)) != std::string::npos) {
                    result.replace(pos, oldChar.length(), newChar);
                    pos += newChar.length();
                }
            }
        }
        
        return result;
    }
    
private:
    bool m_enabled;
    std::map<std::string, std::string> m_newToOldMap;
    std::map<std::string, std::string> m_oldToNewMap;
    std::map<std::string, std::any> m_config;
};

} // namespace sample
} // namespace plugin
} // namespace japanese_typesetting

// プラグインのエクスポート
JAPANESE_TYPESETTING_EXPORT_PLUGIN(japanese_typesetting::plugin::sample::CharacterConversionPlugin)
```

## 9. よくある質問

### 9.1 プラグインが読み込まれない

- プラグインファイルが正しいディレクトリにあるか確認
- 依存ライブラリが正しくインストールされているか確認
- プラグインのAPIバージョンがホストアプリケーションと互換性があるか確認
- ログファイルでエラーメッセージを確認

### 9.2 プラグインの依存関係

- 依存するプラグインが先に読み込まれるようにする
- 循環依存を避ける
- 依存関係は`getInfo()`メソッドで正しく宣言する

### 9.3 プラグインの設定保存

- 設定は`getConfig()`と`setConfig()`メソッドで管理
- 複雑な設定はJSONなどのフォーマットで保存
- ユーザー設定は`~/.japanese_typesetting/plugins/settings/`に保存される

### 9.4 プラグインのデバッグ

- デバッグログを有効にする
- デバッグビルドでプラグインをビルドする
- デバッガをアタッチしてプラグインをデバッグする

## 10. 参考資料

### 10.1 APIリファレンス

詳細なAPIリファレンスは、以下のURLで参照できます：
https://docs.japanese-typesetting.org/api/plugin/

### 10.2 サンプルコード

サンプルプラグインのコードは、以下のリポジトリで参照できます：
https://github.com/japanese-typesetting/sample-plugins

### 10.3 コミュニティとサポート

- **フォーラム**: https://forum.japanese-typesetting.org/plugins
- **GitHub**: https://github.com/japanese-typesetting/japanese-typesetting/issues
- **メールサポート**: plugins@japanese-typesetting.org
