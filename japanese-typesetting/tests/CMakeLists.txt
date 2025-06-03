enable_testing()

# Google Testの検索
find_package(GTest REQUIRED)

# テスト用の共通設定
function(add_japanese_typesetting_test test_name test_file)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name}
        PRIVATE
            japanese_typesetting_core
            GTest::GTest
            GTest::Main
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# 単体テスト
add_subdirectory(unit)

# 統合テスト
add_subdirectory(integration)
