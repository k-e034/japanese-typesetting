# サンプルプラグインのCMakeリスト

cmake_minimum_required(VERSION 3.10)

# プラグイン名
set(PLUGIN_NAME "sample_ruby_plugin")

# ソースファイル
set(PLUGIN_SOURCES
    sample_ruby_plugin.cpp
)

# ヘッダーファイル
set(PLUGIN_HEADERS
    sample_ruby_plugin.h
)

# 共有ライブラリとしてプラグインをビルド
add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

# インクルードディレクトリの設定
target_include_directories(${PLUGIN_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 依存ライブラリのリンク
target_link_libraries(${PLUGIN_NAME} PRIVATE
    japanese_typesetting_core
    japanese_typesetting_plugin
)

# コンパイルオプションの設定
target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_17)

# インストール設定
install(TARGETS ${PLUGIN_NAME}
    LIBRARY DESTINATION lib/japanese_typesetting/plugins
    RUNTIME DESTINATION lib/japanese_typesetting/plugins
)
