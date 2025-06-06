#include <gtest/gtest.h>
#include "japanese_typesetting/core/unicode/unicode.h"

using japanese_typesetting::core::unicode::UnicodeHandler;

TEST(UnicodeHandlerTest, Utf8ToUtf32HandlesSupplementaryCharacters) {
    UnicodeHandler handler;
    std::string emoji_utf8 = u8"\U0001F600"; // grinning face
    std::u32string utf32 = handler.utf8ToUtf32(emoji_utf8);
    ASSERT_EQ(utf32.size(), 1u);
    EXPECT_EQ(utf32[0], U'\U0001F600');
}

TEST(UnicodeHandlerTest, Utf32Utf8RoundTrip) {
    UnicodeHandler handler;
    std::u32string input = { U'\U0001F600', U'あ', U'\n' };
    std::string utf8 = handler.utf32ToUtf8(input);
    std::u32string round = handler.utf8ToUtf32(utf8);
    EXPECT_EQ(round, input);
}
