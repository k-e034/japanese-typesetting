cmake_minimum_required(VERSION 3.14)

project(japanese_typesetting VERSION 0.1.0 LANGUAGES CXX)

# C++17を要求
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 依存ライブラリの検索
find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(Freetype REQUIRED)
find_package(harfbuzz CONFIG QUIET)
if(NOT harfbuzz_FOUND)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(HARFBUZZ harfbuzz)
        if(HARFBUZZ_FOUND)
            add_library(harfbuzz::harfbuzz ALIAS PkgConfig::HARFBUZZ)
        endif()
    endif()
endif()
if(NOT TARGET harfbuzz::harfbuzz)
    message(WARNING "harfbuzz not found; building without it")
endif()

# テストビルドのオプション
option(BUILD_TESTING "Build the testing tree." OFF)

# サブディレクトリの追加
add_subdirectory(src)

# テストビルドが有効な場合のみテストを追加
if(BUILD_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()

# ビルド設定情報の表示
message(STATUS "Japanese Typesetting Software build configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
