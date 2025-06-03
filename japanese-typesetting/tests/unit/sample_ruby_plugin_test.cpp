/**
 * @file sample_ruby_plugin_test.cpp
 * @brief サンプルルビプラグインのテスト
 */

#include <gtest/gtest.h>
#include "sample_ruby_plugin.h"
#include <string>
#include <map>

using namespace japanese_typesetting::plugin::sample;

// サンプルルビプラグインの基本機能テスト
TEST(SampleRubyPluginTest, BasicFunctionality) {
    // プラグインのインスタンス作成
    SampleRubyPlugin plugin;
    
    // 初期状態は無効
    EXPECT_FALSE(plugin.isEnabled());
    
    // プラグイン情報の確認
    auto info = plugin.getInfo();
    EXPECT_EQ(info.id, "jp.typesetting.sample.ruby");
    EXPECT_EQ(info.name, "サンプルルビプラグイン");
    EXPECT_EQ(info.type, japanese_typesetting::plugin::PluginType::TYPESETTING_RULE);
    
    // プラグインの有効化
    EXPECT_TRUE(plugin.enable());
    EXPECT_TRUE(plugin.isEnabled());
    
    // プラグインの無効化
    EXPECT_TRUE(plugin.disable());
    EXPECT_FALSE(plugin.isEnabled());
}

// ルビマッピングのテスト
TEST(SampleRubyPluginTest, RubyMapping) {
    SampleRubyPlugin plugin;
    
    // デフォルトのマッピングの確認
    EXPECT_EQ(plugin.getRuby("漢字"), "かんじ");
    EXPECT_EQ(plugin.getRuby("日本語"), "にほんご");
    
    // 新しいマッピングの追加
    plugin.addRubyMapping("新規", "しんき");
    EXPECT_EQ(plugin.getRuby("新規"), "しんき");
    
    // マッピングの削除
    EXPECT_TRUE(plugin.removeRubyMapping("新規"));
    EXPECT_EQ(plugin.getRuby("新規"), "");
    
    // 存在しないマッピングの削除
    EXPECT_FALSE(plugin.removeRubyMapping("存在しない"));
    
    // すべてのマッピングの取得
    auto allMappings = plugin.getAllRubyMappings();
    EXPECT_GT(allMappings.size(), 0);
    EXPECT_EQ(allMappings["漢字"], "かんじ");
}

// ルビ適用のテスト
TEST(SampleRubyPluginTest, ApplyRuby) {
    SampleRubyPlugin plugin;
    
    // プラグインを有効化
    plugin.enable();
    
    // 設定の確認と変更
    EXPECT_TRUE(std::any_cast<bool>(plugin.getConfig("autoApply")));
    EXPECT_EQ(std::any_cast<std::string>(plugin.getConfig("rubyFormat")), "《》");
    
    // ルビの適用
    std::string text = "日本語の漢字について";
    std::string expected = "日本語《にほんご》の漢字《かんじ》について";
    EXPECT_EQ(plugin.applyRuby(text), expected);
    
    // ルビフォーマットの変更
    plugin.setConfig("rubyFormat", std::string("()"));
    EXPECT_EQ(plugin.applyRuby(text), "日本語(にほんご)の漢字(かんじ)について");
    
    // 自動適用の無効化
    plugin.setConfig("autoApply", false);
    EXPECT_EQ(plugin.applyRuby(text), text);
    
    // プラグインの無効化
    plugin.disable();
    plugin.setConfig("autoApply", true);
    EXPECT_EQ(plugin.applyRuby(text), text);
}

// 最小漢字長のテスト
TEST(SampleRubyPluginTest, MinKanjiLength) {
    SampleRubyPlugin plugin;
    plugin.enable();
    
    // 最小漢字長の設定
    plugin.setConfig("minKanjiLength", 2);
    
    // 新しいマッピングの追加
    plugin.addRubyMapping("字", "じ");
    plugin.addRubyMapping("漢字", "かんじ");
    
    // 最小漢字長以上のみ適用される
    std::string text = "漢字と字について";
    std::string expected = "漢字《かんじ》と字について";
    EXPECT_EQ(plugin.applyRuby(text), expected);
    
    // 最小漢字長の変更
    plugin.setConfig("minKanjiLength", 1);
    expected = "漢字《かんじ》と字《じ》について";
    EXPECT_EQ(plugin.applyRuby(text), expected);
}

// ファイル入出力のテスト
TEST(SampleRubyPluginTest, FileIO) {
    SampleRubyPlugin plugin;
    
    // テスト用のマッピングを追加
    plugin.addRubyMapping("テスト", "てすと");
    plugin.addRubyMapping("ファイル", "ふぁいる");
    
    // 一時ファイルパス
    std::string tempFile = "/tmp/ruby_mappings_test.json";
    
    // ファイルに保存
    EXPECT_TRUE(plugin.saveRubyMappingsToFile(tempFile));
    
    // 新しいプラグインインスタンスを作成
    SampleRubyPlugin newPlugin;
    
    // ファイルから読み込み
    EXPECT_TRUE(newPlugin.loadRubyMappingsFromFile(tempFile));
    
    // マッピングが正しく読み込まれたか確認
    EXPECT_EQ(newPlugin.getRuby("テスト"), "てすと");
    EXPECT_EQ(newPlugin.getRuby("ファイル"), "ふぁいる");
}
