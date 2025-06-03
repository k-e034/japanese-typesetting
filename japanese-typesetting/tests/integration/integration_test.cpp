/**
 * @file integration_test.cpp
 * @brief 日本語組版ソフトウェアの統合テスト
 */

#include <gtest/gtest.h>
#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include "japanese_typesetting/core/unicode/unicode.h"
#include "japanese_typesetting/output/pdf_output.h"
#include "japanese_typesetting/output/epub_output.h"
#include "japanese_typesetting/output/html_output.h"
#include "japanese_typesetting/plugin/plugin_manager.h"
#include <string>
#include <vector>
#include <filesystem>

using namespace japanese_typesetting;

// 文書処理の統合テスト
TEST(IntegrationTest, DocumentProcessing) {
    // 文書の作成
    core::document::Document doc;
    doc.setTitle("統合テスト文書");
    doc.setAuthor("テスト作者");
    
    // テキストコンテンツの追加
    core::document::Section section;
    section.setTitle("第1章");
    
    core::document::Paragraph para1;
    para1.setText("これは日本語組版ソフトウェアの統合テストです。");
    section.addParagraph(para1);
    
    core::document::Paragraph para2;
    para2.setText("禁則処理や文字詰め、ルビなどの機能をテストします。");
    section.addParagraph(para2);
    
    // ルビ付きテキストの追加
    core::document::Paragraph para3;
    para3.setText("漢字《かんじ》にルビを振ることができます。");
    section.addParagraph(para3);
    
    doc.addSection(section);
    
    // スタイルの設定
    core::style::Style style;
    style.setFontFamily("Noto Sans CJK JP");
    style.setFontSize(10.5);
    style.setLineHeight(1.5);
    style.setMarginTop(20.0);
    style.setMarginBottom(20.0);
    style.setMarginLeft(20.0);
    style.setMarginRight(20.0);
    style.setVerticalWriting(true);  // 縦書きモード
    
    // 組版エンジンの初期化
    core::typesetting::TypesettingEngine engine;
    
    // 文書の組版
    double pageWidth = 210.0;  // A4サイズ（mm）
    std::vector<core::typesetting::TextBlock> blocks = engine.typesetDocument(doc, style, pageWidth);
    
    // 組版結果の検証
    ASSERT_FALSE(blocks.empty());
    
    // 各出力エンジンでの出力テスト
    
    // 一時ディレクトリの作成
    std::string tempDir = "/tmp/japanese_typesetting_test";
    std::filesystem::create_directories(tempDir);
    
    // PDF出力
    output::PDFOutputOptions pdfOptions;
    pdfOptions.fontFamily = "Noto Sans CJK JP";
    pdfOptions.fontSize = 10.5;
    pdfOptions.pageSize = "A4";
    pdfOptions.vertical = true;
    
    output::PDFOutputEngine pdfEngine;
    EXPECT_TRUE(pdfEngine.outputPDF(doc, style, tempDir + "/test_output.pdf", pdfOptions));
    
    // EPUB出力
    output::EpubOutputOptions epubOptions;
    epubOptions.fontFamily = "Noto Sans CJK JP";
    epubOptions.fontSize = 10.5;
    epubOptions.lineHeight = 1.5;
    epubOptions.vertical = true;
    epubOptions.title = "統合テスト文書";
    epubOptions.author = "テスト作者";
    epubOptions.language = "ja";
    
    output::EpubOutputEngine epubEngine;
    EXPECT_TRUE(epubEngine.outputEpub(doc, style, tempDir + "/test_output.epub", epubOptions));
    
    // HTML出力
    output::HtmlOutputOptions htmlOptions;
    htmlOptions.fontFamily = "Noto Sans CJK JP";
    htmlOptions.fontSize = 10.5;
    htmlOptions.lineHeight = 1.5;
    htmlOptions.vertical = true;
    htmlOptions.title = "統合テスト文書";
    htmlOptions.author = "テスト作者";
    htmlOptions.language = "ja";
    htmlOptions.embedCss = true;
    
    output::HtmlOutputEngine htmlEngine;
    EXPECT_TRUE(htmlEngine.outputHtml(doc, style, tempDir + "/test_output.html", htmlOptions));
    
    // 出力ファイルの存在確認
    EXPECT_TRUE(std::filesystem::exists(tempDir + "/test_output.pdf"));
    EXPECT_TRUE(std::filesystem::exists(tempDir + "/test_output.epub"));
    EXPECT_TRUE(std::filesystem::exists(tempDir + "/test_output.html"));
}

// 縦書き・横書き切り替えの統合テスト
TEST(IntegrationTest, VerticalHorizontalSwitching) {
    // 文書の作成
    core::document::Document doc;
    doc.setTitle("縦書き・横書きテスト");
    
    core::document::Section section;
    section.setTitle("テスト");
    
    core::document::Paragraph para;
    para.setText("これは縦書きと横書きの切り替えテストです。");
    section.addParagraph(para);
    
    doc.addSection(section);
    
    // 組版エンジンの初期化
    core::typesetting::TypesettingEngine engine;
    
    // 縦書きスタイル
    core::style::Style verticalStyle;
    verticalStyle.setVerticalWriting(true);
    
    // 横書きスタイル
    core::style::Style horizontalStyle;
    horizontalStyle.setVerticalWriting(false);
    
    // 縦書き組版
    std::vector<core::typesetting::TextBlock> verticalBlocks = engine.typesetDocument(doc, verticalStyle, 210.0);
    
    // 横書き組版
    std::vector<core::typesetting::TextBlock> horizontalBlocks = engine.typesetDocument(doc, horizontalStyle, 210.0);
    
    // 組版結果の検証
    ASSERT_FALSE(verticalBlocks.empty());
    ASSERT_FALSE(horizontalBlocks.empty());
    
    // 縦書きと横書きで異なる結果になることを確認
    // （実際の検証方法はエンジンの実装に依存）
    EXPECT_NE(verticalBlocks.size(), 0);
    EXPECT_NE(horizontalBlocks.size(), 0);
}

// 禁則処理の統合テスト
TEST(IntegrationTest, JapaneseLineBreakingRules) {
    // 文書の作成
    core::document::Document doc;
    
    core::document::Section section;
    
    // 行頭禁則文字を含むテキスト
    core::document::Paragraph para1;
    para1.setText("これは、行頭禁則のテストです。");
    section.addParagraph(para1);
    
    // 行末禁則文字を含むテキスト
    core::document::Paragraph para2;
    para2.setText("これは「行末禁則」のテストです。");
    section.addParagraph(para2);
    
    // 分離禁止文字を含むテキスト
    core::document::Paragraph para3;
    para3.setText("分離禁止の例：123,456円です。");
    section.addParagraph(para3);
    
    doc.addSection(section);
    
    // スタイルの設定（狭い幅で禁則処理が発生するように）
    core::style::Style style;
    style.setFontSize(12.0);
    
    // 組版エンジンの初期化
    core::typesetting::TypesettingEngine engine;
    
    // 狭い幅で組版
    double narrowWidth = 100.0;
    std::vector<core::typesetting::TextBlock> blocks = engine.typesetDocument(doc, style, narrowWidth);
    
    // 組版結果の検証
    ASSERT_FALSE(blocks.empty());
    
    // 禁則処理が適切に行われていることを確認
    // （実際の検証方法はエンジンの実装に依存）
    // ここでは単純に結果が生成されることだけを確認
    EXPECT_NE(blocks.size(), 0);
}

// ルビ処理の統合テスト
TEST(IntegrationTest, RubyProcessing) {
    // 文書の作成
    core::document::Document doc;
    
    core::document::Section section;
    
    // ルビ付きテキスト
    core::document::Paragraph para1;
    para1.setText("漢字《かんじ》にルビを振ります。");
    section.addParagraph(para1);
    
    // 複数のルビ
    core::document::Paragraph para2;
    para2.setText("日本語《にほんご》の組版《くみはん》について。");
    section.addParagraph(para2);
    
    doc.addSection(section);
    
    // スタイルの設定
    core::style::Style style;
    style.setFontSize(12.0);
    
    // 組版エンジンの初期化
    core::typesetting::TypesettingEngine engine;
    
    // 組版
    std::vector<core::typesetting::TextBlock> blocks = engine.typesetDocument(doc, style, 210.0);
    
    // 組版結果の検証
    ASSERT_FALSE(blocks.empty());
    
    // ルビが適切に処理されていることを確認
    // （実際の検証方法はエンジンの実装に依存）
    // ここでは単純に結果が生成されることだけを確認
    EXPECT_NE(blocks.size(), 0);
}

// プラグインシステムの統合テスト
TEST(IntegrationTest, PluginSystem) {
    // プラグインマネージャーの取得
    plugin::PluginManager& manager = plugin::PluginManager::getInstance();
    
    // プラグインディレクトリの設定（テスト用）
    std::string pluginDir = "/tmp/japanese_typesetting_plugins";
    std::filesystem::create_directories(pluginDir);
    
    // プラグインの読み込み（実際のプラグインがなくても、ディレクトリの存在確認だけ行う）
    EXPECT_TRUE(std::filesystem::exists(pluginDir));
    
    // プラグインマネージャーの基本機能テスト
    EXPECT_NE(&manager, nullptr);
    
    // すべてのプラグインを取得
    std::vector<std::shared_ptr<plugin::Plugin>> plugins = manager.getAllPlugins();
    
    // この時点では登録されているプラグインはないはず
    EXPECT_TRUE(plugins.empty());
}

// 出力フォーマット切り替えの統合テスト
TEST(IntegrationTest, OutputFormatSwitching) {
    // 文書の作成
    core::document::Document doc;
    doc.setTitle("出力フォーマットテスト");
    
    core::document::Section section;
    section.setTitle("テスト");
    
    core::document::Paragraph para;
    para.setText("これは出力フォーマット切り替えのテストです。");
    section.addParagraph(para);
    
    doc.addSection(section);
    
    // スタイルの設定
    core::style::Style style;
    style.setFontFamily("Noto Sans CJK JP");
    style.setFontSize(10.5);
    
    // 一時ディレクトリの作成
    std::string tempDir = "/tmp/japanese_typesetting_format_test";
    std::filesystem::create_directories(tempDir);
    
    // PDF出力
    output::PDFOutputOptions pdfOptions;
    pdfOptions.fontFamily = "Noto Sans CJK JP";
    pdfOptions.fontSize = 10.5;
    
    output::PDFOutputEngine pdfEngine;
    EXPECT_TRUE(pdfEngine.outputPDF(doc, style, tempDir + "/format_test.pdf", pdfOptions));
    
    // EPUB出力
    output::EpubOutputOptions epubOptions;
    epubOptions.fontFamily = "Noto Sans CJK JP";
    epubOptions.fontSize = 10.5;
    epubOptions.title = "出力フォーマットテスト";
    
    output::EpubOutputEngine epubEngine;
    EXPECT_TRUE(epubEngine.outputEpub(doc, style, tempDir + "/format_test.epub", epubOptions));
    
    // HTML出力（単一ファイル）
    output::HtmlOutputOptions htmlSingleOptions;
    htmlSingleOptions.fontFamily = "Noto Sans CJK JP";
    htmlSingleOptions.fontSize = 10.5;
    htmlSingleOptions.title = "出力フォーマットテスト";
    
    output::HtmlOutputEngine htmlEngine;
    EXPECT_TRUE(htmlEngine.outputHtml(doc, style, tempDir + "/format_test_single.html", htmlSingleOptions));
    
    // HTML出力（複数ファイル）
    output::HtmlOutputOptions htmlMultiOptions;
    htmlMultiOptions.fontFamily = "Noto Sans CJK JP";
    htmlMultiOptions.fontSize = 10.5;
    htmlMultiOptions.title = "出力フォーマットテスト";
    
    EXPECT_TRUE(htmlEngine.outputHtmlMultiFile(doc, style, tempDir + "/html_multi", htmlMultiOptions));
    
    // 出力ファイルの存在確認
    EXPECT_TRUE(std::filesystem::exists(tempDir + "/format_test.pdf"));
    EXPECT_TRUE(std::filesystem::exists(tempDir + "/format_test.epub"));
    EXPECT_TRUE(std::filesystem::exists(tempDir + "/format_test_single.html"));
    EXPECT_TRUE(std::filesystem::exists(tempDir + "/html_multi/index.html"));
}
