# コアライブラリのCMakeリスト

# コアモジュールのソースファイル
set(CORE_SOURCES
    core/document/document.cpp
    core/style/style.cpp
    core/typesetting/typesetting.cpp
    core/unicode/unicode.cpp
)

# コアライブラリの作成
add_library(japanese_typesetting_core ${CORE_SOURCES})

# インクルードディレクトリの設定
target_include_directories(japanese_typesetting_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 依存ライブラリのリンク
target_link_libraries(japanese_typesetting_core
    PUBLIC
        ICU::uc
        ICU::i18n
        Freetype::Freetype
        harfbuzz::harfbuzz
)

# CLIモジュールのソースファイル
set(CLI_SOURCES
    cli/cli.cpp
)

# CLIライブラリの作成
add_library(japanese_typesetting_cli ${CLI_SOURCES})

# CLIライブラリの依存関係
target_link_libraries(japanese_typesetting_cli
    PUBLIC
        japanese_typesetting_core
)

# GUIモジュールのソースファイル
set(GUI_SOURCES
    gui/gui.cpp
)

# GUIライブラリの作成
add_library(japanese_typesetting_gui ${GUI_SOURCES})

# GUIライブラリの依存関係
target_link_libraries(japanese_typesetting_gui
    PUBLIC
        japanese_typesetting_core
)

# 出力モジュールのソースファイル
set(OUTPUT_SOURCES
    output/pdf/pdf_output.cpp
    output/epub/epub_output.cpp
    output/html/html_output.cpp
)

# 出力ライブラリの作成
add_library(japanese_typesetting_output ${OUTPUT_SOURCES})

# 出力ライブラリの依存関係
target_link_libraries(japanese_typesetting_output
    PUBLIC
        japanese_typesetting_core
)

# プラグインモジュールのソースファイル
set(PLUGIN_SOURCES
    plugin/plugin_manager.cpp
)

# プラグインライブラリの作成
add_library(japanese_typesetting_plugin ${PLUGIN_SOURCES})

# プラグインライブラリの依存関係
target_link_libraries(japanese_typesetting_plugin
    PUBLIC
        japanese_typesetting_core
)

# メイン実行ファイルの作成
add_executable(japanese_typesetting main.cpp)

# メイン実行ファイルの依存関係
target_link_libraries(japanese_typesetting
    PRIVATE
        japanese_typesetting_core
        japanese_typesetting_cli
        japanese_typesetting_gui
        japanese_typesetting_output
        japanese_typesetting_plugin
)
