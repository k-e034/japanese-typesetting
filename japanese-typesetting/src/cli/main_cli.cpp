/**
 * @file main_cli.cpp
 * @brief コマンドラインアプリケーションのエントリーポイント
 */

#include "japanese_typesetting/cli/cli.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // コマンドラインインターフェースの作成
        japanese_typesetting::cli::CommandLineInterface cli;
        
        // コマンドライン引数の解析
        japanese_typesetting::cli::CommandLineOptions options = cli.parseCommandLine(argc, argv);
        
        // メイン処理の実行
        return cli.run(options);
    } catch (const std::exception& e) {
        std::cerr << "致命的なエラーが発生しました: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "不明なエラーが発生しました" << std::endl;
        return 1;
    }
}
