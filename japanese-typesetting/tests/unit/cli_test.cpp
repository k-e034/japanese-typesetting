/**
 * @file cli_test.cpp
 * @brief コマンドラインインターフェースのテスト
 */

#include "japanese_typesetting/cli/cli.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace japanese_typesetting {
namespace cli {
namespace test {

// コマンドライン引数の解析テスト
TEST(CommandLineInterfaceTest, ParseCommandLine) {
    CommandLineInterface cli;
    
    // テスト用の引数を作成
    const char* argv[] = {
        "japanese-typesetting",
        "-i", "input.txt",
        "-o", "output.pdf",
        "--font-size", "12",
        "--horizontal"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    // 引数の解析
    CommandLineOptions options = cli.parseCommandLine(argc, const_cast<char**>(argv));
    
    // 結果の検証
    EXPECT_EQ(options.inputFile, "input.txt");
    EXPECT_EQ(options.outputFile, "output.pdf");
    EXPECT_EQ(options.fontSize, 12.0);
    EXPECT_FALSE(options.vertical);
}

// ヘルプオプションのテスト
TEST(CommandLineInterfaceTest, HelpOption) {
    CommandLineInterface cli;
    
    // テスト用の引数を作成
    const char* argv[] = {
        "japanese-typesetting",
        "--help"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    // 引数の解析
    CommandLineOptions options = cli.parseCommandLine(argc, const_cast<char**>(argv));
    
    // 結果の検証
    EXPECT_TRUE(options.help);
}

// バージョンオプションのテスト
TEST(CommandLineInterfaceTest, VersionOption) {
    CommandLineInterface cli;
    
    // テスト用の引数を作成
    const char* argv[] = {
        "japanese-typesetting",
        "--version"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    // 引数の解析
    CommandLineOptions options = cli.parseCommandLine(argc, const_cast<char**>(argv));
    
    // 結果の検証
    EXPECT_TRUE(options.version);
}

// 出力フォーマットオプションのテスト
TEST(CommandLineInterfaceTest, OutputFormatOption) {
    CommandLineInterface cli;
    
    // テスト用の引数を作成
    const char* argv[] = {
        "japanese-typesetting",
        "input.txt",
        "-f", "html"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    // 引数の解析
    CommandLineOptions options = cli.parseCommandLine(argc, const_cast<char**>(argv));
    
    // 結果の検証
    EXPECT_EQ(options.inputFile, "input.txt");
    EXPECT_EQ(options.outputFormat, "html");
}

// 非オプション引数のテスト
TEST(CommandLineInterfaceTest, NonOptionArguments) {
    CommandLineInterface cli;
    
    // テスト用の引数を作成
    const char* argv[] = {
        "japanese-typesetting",
        "input.txt",
        "output.epub"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    // 引数の解析
    CommandLineOptions options = cli.parseCommandLine(argc, const_cast<char**>(argv));
    
    // 結果の検証
    EXPECT_EQ(options.inputFile, "input.txt");
    EXPECT_EQ(options.outputFile, "output.epub");
}

// カスタムオプションのテスト
TEST(CommandLineInterfaceTest, CustomOptions) {
    CommandLineInterface cli;
    
    // テスト用の引数を作成
    const char* argv[] = {
        "japanese-typesetting",
        "input.txt",
        "--custom-option", "value",
        "--flag-option"
    };
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    // 引数の解析
    CommandLineOptions options = cli.parseCommandLine(argc, const_cast<char**>(argv));
    
    // 結果の検証
    EXPECT_EQ(options.inputFile, "input.txt");
    EXPECT_EQ(options.extraOptions["custom-option"], "value");
    EXPECT_EQ(options.extraOptions["flag-option"], "true");
}

} // namespace test
} // namespace cli
} // namespace japanese_typesetting
