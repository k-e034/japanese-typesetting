# ローカル環境セットアップガイド

このドキュメントでは、日本語組版特化ソフトウェアをローカル環境で利用するための手順を説明します。動作環境の準備からビルド、基本的な使い方までをまとめました。

## 1. 必要環境

- **対応OS**: Windows 10/11、macOS 10.15以降、Linux (Ubuntu 20.04以降)
- **必須ツール**:
  - C++17 対応コンパイラ (GCC 7+/Clang 5+/MSVC 2017+)
  - CMake 3.10 以上
  - Git
  - (Linux) build-essential 等の開発ツール
- **必須ライブラリ**: ICU、FreeType、HarfBuzz、Qt5 など

## 2. リポジトリの取得

```bash
git clone https://github.com/japanese-typesetting/japanese-typesetting.git
cd japanese-typesetting/japanese-typesetting
```

## 3. 依存パッケージのインストール

### Ubuntu での例

```bash
sudo apt-get update
sudo apt-get install build-essential cmake git libicu-dev libfreetype-dev libharfbuzz-dev libqt5core5a qtbase5-dev libpodofo-dev libpugixml-dev libxml2-dev
```

### macOS での例 (Homebrew)

```bash
brew install cmake icu4c freetype harfbuzz qt podofo pugixml
```

### Windows での例

1. [vcpkg](https://github.com/microsoft/vcpkg) をインストール
2. `vcpkg install icu freetype harfbuzz qt5-base podofo pugixml`

## 4. ビルド手順

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)    # Windows の場合は nmake などを使用
```

完了すると `bin/` ディレクトリに実行ファイルが生成されます。

## 5. インストール (任意)

```bash
sudo make install
```

## 6. 基本的な使い方

### 6.1 コマンドラインからの実行

```bash
./bin/japanese-typesetting-cli input.txt -o output.pdf
```

主なオプションについては `--help` を参照してください。

### 6.2 GUI の起動

```bash
./bin/japanese-typesetting-gui
```

GUI 上で文書の編集、プレビュー、エクスポートが行えます。

## 7. 設定ファイル

ユーザー設定は以下の場所に保存されます。

- Windows: `%APPDATA%\japanese-typesetting\config.json`
- macOS: `~/Library/Application Support/japanese-typesetting/config.json`
- Linux: `~/.config/japanese-typesetting/config.json`

初期設定を変更する場合はこのファイルを編集してください。

## 8. トラブルシューティング

ビルドや実行時に問題が発生した場合は、`logs/` ディレクトリのログファイルを確認してください。詳細なエラーは GitHub Issues でも報告できます。

