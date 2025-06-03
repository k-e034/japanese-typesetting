# 単体テストのCMakeリスト

# ドキュメント関連のテスト
add_japanese_typesetting_test(document_test document_test.cpp)

# スタイル関連のテスト
add_japanese_typesetting_test(style_test style_test.cpp)

# 組版関連のテスト
add_japanese_typesetting_test(typesetting_test typesetting_test.cpp)

# Unicode処理関連のテスト
add_japanese_typesetting_test(unicode_test unicode_test.cpp)
