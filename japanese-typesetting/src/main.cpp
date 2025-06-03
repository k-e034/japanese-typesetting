/**
 * @file main.cpp
 * @brief 日本語組版ソフトウェアのメインエントリーポイント
 */

#include <iostream>
#include <string>
#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting.h"
#include "japanese_typesetting/core/unicode/unicode.h"

int main(int argc, char* argv[]) {
    std::cout << "日本語組版特化ソフトウェア - 開発版" << std::endl;
    std::cout << "Copyright (c) 2025 Japanese Typesetting Project" << std::endl;
    std::cout << "Version 0.1.0" << std::endl;
    std::cout << std::endl;

    // 基本的なオブジェクトの作成テスト
    japanese_typesetting::core::document::Document doc("サンプル文書", "テスト著者", true);
    japanese_typesetting::core::style::Style style;
    japanese_typesetting::core::typesetting::TypesettingEngine engine;
    japanese_typesetting::core::unicode::UnicodeHandler unicode;

    std::cout << "文書タイトル: " << doc.getTitle() << std::endl;
    std::cout << "著者: " << doc.getAuthor() << std::endl;
    std::cout << "縦書き: " << (doc.isVertical() ? "はい" : "いいえ") << std::endl;
    std::cout << std::endl;

    // Unicodeテスト
    std::string testText = "日本語組版のテスト。Hello, World!";
    std::u32string utf32Text = unicode.utf8ToUtf32(testText);
    std::cout << "UTF-8 -> UTF-32 -> UTF-8 変換テスト: " << unicode.utf32ToUtf8(utf32Text) << std::endl;
    
    std::cout << "文字種別判定テスト:" << std::endl;
    for (char32_t c : utf32Text) {
        std::string charUtf8 = unicode.utf32ToUtf8(std::u32string(1, c));
        std::cout << "  文字 '" << charUtf8 << "': ";
        if (unicode.isJapaneseCharacter(c)) {
            std::cout << "日本語 ";
        }
        if (unicode.isFullWidthCharacter(c)) {
            std::cout << "全角 ";
        }
        if (unicode.isHalfWidthCharacter(c)) {
            std::cout << "半角 ";
        }
        if (unicode.isPunctuation(c)) {
            std::cout << "句読点 ";
        }
        if (unicode.isOpeningBracket(c)) {
            std::cout << "開き括弧 ";
        }
        if (unicode.isClosingBracket(c)) {
            std::cout << "閉じ括弧 ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "初期セットアップ完了" << std::endl;
    
    return 0;
}
