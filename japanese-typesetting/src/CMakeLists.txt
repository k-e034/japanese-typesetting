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
)
if(TARGET harfbuzz::harfbuzz)
    target_link_libraries(japanese_typesetting_core PUBLIC harfbuzz::harfbuzz)
endif()

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
find_package(Qt5 COMPONENTS Widgets QUIET)
if(Qt5_FOUND)
    set(GUI_SOURCES gui/gui.cpp)
    add_library(japanese_typesetting_gui ${GUI_SOURCES})
    target_link_libraries(japanese_typesetting_gui
        PUBLIC
            japanese_typesetting_core
            Qt5::Widgets
    )
else()
    message(WARNING "Qt5 not found; GUI module will not be built")
endif()

# 出力モジュールのソースファイル
set(OUTPUT_SOURCES
    output/html/html_output.cpp
)

find_package(Python3 COMPONENTS Development QUIET)
if(Python3_Development_FOUND)
    list(APPEND OUTPUT_SOURCES output/pdf/pdf_output.cpp)
else()
    message(WARNING "Python3 development package not found; PDF output will be disabled")
endif()

include(CheckIncludeFileCXX)
check_include_file_cxx(zip.h HAVE_ZIP_H)
if(HAVE_ZIP_H)
    list(APPEND OUTPUT_SOURCES output/epub/epub_output.cpp)
else()
    message(WARNING "libzip headers not found; EPUB output will be disabled")
endif()

# 出力ライブラリの作成
add_library(japanese_typesetting_output ${OUTPUT_SOURCES})

# 出力ライブラリの依存関係
target_link_libraries(japanese_typesetting_output
    PUBLIC
        japanese_typesetting_core
)

if(Python3_Development_FOUND)
    target_include_directories(japanese_typesetting_output PUBLIC ${Python3_INCLUDE_DIRS})
    target_link_libraries(japanese_typesetting_output PUBLIC ${Python3_LIBRARIES})
endif()

# プラグインモジュールのソースファイル
include(CheckIncludeFileCXX)
check_include_file_cxx(nlohmann/json.hpp HAVE_NLOHMANN_JSON)
if(HAVE_NLOHMANN_JSON)
    set(PLUGIN_SOURCES plugin/plugin_manager.cpp)
    add_library(japanese_typesetting_plugin ${PLUGIN_SOURCES})
    target_link_libraries(japanese_typesetting_plugin PUBLIC japanese_typesetting_core)
else()
    message(WARNING "nlohmann/json.hpp not found; plugin system will be disabled")
endif()

# メイン実行ファイルの作成
add_executable(japanese_typesetting main.cpp)

# メイン実行ファイルの依存関係
target_link_libraries(japanese_typesetting
    PRIVATE
        japanese_typesetting_core
        japanese_typesetting_cli
        japanese_typesetting_output
)
if(TARGET japanese_typesetting_gui)
    target_link_libraries(japanese_typesetting PRIVATE japanese_typesetting_gui)
endif()
if(TARGET japanese_typesetting_plugin)
    target_link_libraries(japanese_typesetting PRIVATE japanese_typesetting_plugin)
endif()
