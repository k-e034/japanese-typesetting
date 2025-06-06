/**
 * @file style_test.cpp
 * @brief Styleクラスの実際のテストを含むファイル
 */

#include <gtest/gtest.h>
#include "japanese_typesetting/core/style/style.h"

using japanese_typesetting::core::style::Style;
using japanese_typesetting::core::style::TextAlignment;
using japanese_typesetting::core::style::LineBreakMode;


// Setter/Getters の検証
TEST(StyleTest, SettersAndGetters) {
    Style style;

    style.setFontFamily("TestFont");
    EXPECT_EQ(style.getFontFamily(), "TestFont");

    style.setFontSize(12.0);
    EXPECT_DOUBLE_EQ(style.getFontSize(), 12.0);

    style.setLineHeight(2.0);
    EXPECT_DOUBLE_EQ(style.getLineHeight(), 2.0);

    style.setTextAlignment(TextAlignment::Center);
    EXPECT_EQ(style.getTextAlignment(), TextAlignment::Center);

    style.setLineBreakMode(LineBreakMode::Strict);
    EXPECT_EQ(style.getLineBreakMode(), LineBreakMode::Strict);

    style.setCharacterSpacing(0.1);
    EXPECT_DOUBLE_EQ(style.getCharacterSpacing(), 0.1);

    style.setWordSpacing(0.2);
    EXPECT_DOUBLE_EQ(style.getWordSpacing(), 0.2);

    style.setParagraphSpacingBefore(0.3);
    EXPECT_DOUBLE_EQ(style.getParagraphSpacingBefore(), 0.3);

    style.setParagraphSpacingAfter(0.4);
    EXPECT_DOUBLE_EQ(style.getParagraphSpacingAfter(), 0.4);

    style.setFirstLineIndent(1.5);
    EXPECT_DOUBLE_EQ(style.getFirstLineIndent(), 1.5);

    style.setBold(true);
    EXPECT_TRUE(style.isBold());

    style.setItalic(true);
    EXPECT_TRUE(style.isItalic());

    style.setUnderline(true);
    EXPECT_TRUE(style.isUnderline());

    style.setProperty("custom-key", "custom-value");
    EXPECT_EQ(style.getProperty("custom-key"), "custom-value");
}
