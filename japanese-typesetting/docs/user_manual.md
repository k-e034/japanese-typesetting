# 日本語組版特化ソフトウェア ユーザーマニュアル

## 1. はじめに

### 1.1 概要

本ソフトウェアは、JIS X 4051に完全準拠した高度な日本語組版機能を持つソフトウェアです。コマンドラインとGUIのハイブリッド構成、プラグイン拡張性、多様な出力形式対応など、プロフェッショナルな日本語文書制作のための機能を提供します。

### 1.2 主要機能

- JIS X 4051準拠の日本語組版
- 禁則処理（行頭禁則、行末禁則、分離禁止）
- 文字詰め処理
- ルビ処理
- 縦書き・横書き切り替え
- コマンドラインインターフェース
- グラフィカルユーザーインターフェース
- リアルタイムプレビュー
- PDF/EPUB/HTML出力
- プラグインシステム

### 1.3 動作環境

- **対応OS**: Windows 10/11, macOS 10.15以降, Linux (Ubuntu 20.04以降)
- **必要メモリ**: 4GB以上
- **ディスク容量**: 500MB以上
- **必須ライブラリ**: ICU, FreeType, HarfBuzz, Qt5

## 2. インストール方法

### 2.1 パッケージからのインストール

#### Windows
1. インストーラーをダウンロード
2. インストーラーを実行し、画面の指示に従う
3. デスクトップショートカットが作成される

#### macOS
1. DMGファイルをダウンロード
2. DMGファイルを開き、アプリケーションフォルダにドラッグ＆ドロップ

#### Linux
```bash
# Ubuntuの場合
sudo apt-get update
sudo apt-get install japanese-typesetting

# Fedoraの場合
sudo dnf install japanese-typesetting
```

### 2.2 ソースからのビルド

1. 必要な依存パッケージをインストール
```bash
# Ubuntuの場合
sudo apt-get install build-essential cmake libicu-dev libfreetype-dev libharfbuzz-dev qtbase5-dev libpodofo-dev libpugixml-dev libxml2-dev
```

2. リポジトリをクローン
```bash
git clone https://github.com/japanese-typesetting/japanese-typesetting.git
cd japanese-typesetting
```

3. ビルド
```bash
mkdir build && cd build
cmake ..
make
```

4. インストール
```bash
sudo make install
```

## 3. 基本的な使い方

### 3.1 コマンドラインインターフェース

#### 基本的なコマンド

```bash
# 基本的な使い方
japanese-typesetting-cli input.txt -o output.pdf

# 縦書きモード
japanese-typesetting-cli input.txt -o output.pdf --vertical

# 出力フォーマットの指定
japanese-typesetting-cli input.txt -o output.epub --format epub
japanese-typesetting-cli input.txt -o output.html --format html

# フォント指定
japanese-typesetting-cli input.txt -o output.pdf --font "Noto Sans CJK JP"

# ヘルプの表示
japanese-typesetting-cli --help
```

#### 詳細オプション

| オプション | 説明 | デフォルト値 |
|------------|------|------------|
| `-o, --output` | 出力ファイルパス | - |
| `--format` | 出力フォーマット (pdf/epub/html) | pdf |
| `--vertical` | 縦書きモード | 無効 |
| `--font` | フォント名 | "Noto Sans CJK JP" |
| `--font-size` | フォントサイズ (pt) | 10.5 |
| `--line-height` | 行の高さ (倍率) | 1.5 |
| `--page-size` | ページサイズ (PDFのみ) | A4 |
| `--margin-top` | 上マージン (mm) | 20 |
| `--margin-bottom` | 下マージン (mm) | 20 |
| `--margin-left` | 左マージン (mm) | 20 |
| `--margin-right` | 右マージン (mm) | 20 |

### 3.2 グラフィカルユーザーインターフェース

1. アプリケーションを起動
2. ファイル > 開く から文書を開く、または新規作成
3. 編集画面で文書を編集
4. プレビューパネルでリアルタイムプレビューを確認
5. スタイル設定パネルで組版設定を調整
6. ファイル > 保存 で文書を保存
7. ファイル > エクスポート で各種形式に出力

#### メインウィンドウの構成

- **メニューバー**: ファイル操作、編集、表示、設定、ヘルプ
- **ツールバー**: よく使う機能へのクイックアクセス
- **編集パネル**: 文書の編集
- **プレビューパネル**: リアルタイムプレビュー
- **スタイル設定パネル**: 組版設定の調整
- **アウトラインパネル**: 文書構造の表示と操作
- **ステータスバー**: 現在の状態表示

## 4. 高度な機能

### 4.1 禁則処理

JIS X 4051に準拠した禁則処理を提供します。

- **行頭禁則**: 句読点などが行頭に来ないようにする
- **行末禁則**: 開き括弧などが行末に来ないようにする
- **分離禁止**: 単位記号などが前の文字と分離しないようにする

設定パネルから禁則処理のレベルを調整できます。

### 4.2 ルビ処理

文字にルビを振る機能を提供します。

#### ルビの記法

```
漢字《かんじ》
```

#### ルビのスタイル設定

- ルビのフォントサイズ
- ルビの位置
- モノルビ/グループルビの切り替え

### 4.3 縦書き・横書き切り替え

文書全体または選択範囲の縦書き・横書きを切り替えることができます。

- ツールバーの縦書き/横書きボタン
- 設定パネルの文書方向設定
- コンテキストメニューからの切り替え

### 4.4 出力フォーマット

#### PDF出力

- 印刷用高品質PDF
- フォント埋め込み
- しおり・目次機能
- ヘッダー・フッター設定

#### EPUB出力

- 電子書籍リーダー対応
- 縦書き・横書き対応
- メタデータ設定
- カバー画像設定

#### HTML出力

- Webブラウザ表示用
- レスポンシブデザイン対応
- CSSカスタマイズ
- 単一ファイル/複数ファイル出力

## 5. プラグインシステム

### 5.1 プラグインの管理

- **設定 > プラグイン** からプラグイン管理画面を開く
- プラグインの有効化/無効化
- プラグインの設定
- 新規プラグインのインストール

### 5.2 標準プラグイン

- **ルビ自動変換**: 特定の漢字に自動的にルビを振る
- **文字変換**: 旧字体・新字体の相互変換
- **スタイルテンプレート**: 定型スタイルの適用

### 5.3 カスタムプラグイン開発

プラグインAPIを使用して独自の機能を追加できます。詳細は「プラグイン開発ガイド」を参照してください。

## 6. トラブルシューティング

### 6.1 よくある問題と解決方法

#### 起動しない場合
- 依存ライブラリが正しくインストールされているか確認
- ログファイルを確認 (`~/.japanese-typesetting/logs/`)
- 設定ファイルを初期化 (`--reset-config` オプションで起動)

#### フォントが正しく表示されない
- フォントがシステムにインストールされているか確認
- 別のフォントを試す
- ICUライブラリが最新版かチェック

#### 出力ファイルに問題がある
- 一時ファイルを削除
- 別の出力形式を試す
- 文書を小さな部分に分割して試す

### 6.2 ログファイル

ログファイルは以下の場所に保存されます：

- Windows: `%APPDATA%\japanese-typesetting\logs\`
- macOS: `~/Library/Logs/japanese-typesetting/`
- Linux: `~/.japanese-typesetting/logs/`

### 6.3 サポート

- **公式フォーラム**: https://forum.japanese-typesetting.org/
- **GitHub Issues**: https://github.com/japanese-typesetting/japanese-typesetting/issues
- **メールサポート**: support@japanese-typesetting.org

## 7. 付録

### 7.1 キーボードショートカット

| 機能 | Windows/Linux | macOS |
|------|--------------|-------|
| 新規作成 | Ctrl+N | Command+N |
| 開く | Ctrl+O | Command+O |
| 保存 | Ctrl+S | Command+S |
| 別名で保存 | Ctrl+Shift+S | Command+Shift+S |
| エクスポート | Ctrl+E | Command+E |
| 印刷 | Ctrl+P | Command+P |
| 元に戻す | Ctrl+Z | Command+Z |
| やり直し | Ctrl+Y | Command+Shift+Z |
| 切り取り | Ctrl+X | Command+X |
| コピー | Ctrl+C | Command+C |
| 貼り付け | Ctrl+V | Command+V |
| 検索 | Ctrl+F | Command+F |
| 置換 | Ctrl+H | Command+H |
| 縦書き/横書き切替 | Ctrl+Alt+V | Command+Option+V |
| プレビュー更新 | F5 | F5 |
| ヘルプ | F1 | F1 |

### 7.2 設定ファイル

設定ファイルは以下の場所に保存されます：

- Windows: `%APPDATA%\japanese-typesetting\config.json`
- macOS: `~/Library/Application Support/japanese-typesetting/config.json`
- Linux: `~/.config/japanese-typesetting/config.json`

### 7.3 コマンドライン設定ファイル

コマンドライン設定ファイルの例：

```json
{
  "default_font": "Noto Sans CJK JP",
  "default_font_size": 10.5,
  "default_line_height": 1.5,
  "default_vertical": true,
  "default_format": "pdf",
  "pdf_options": {
    "page_size": "A4",
    "margin_top": 20,
    "margin_bottom": 20,
    "margin_left": 20,
    "margin_right": 20
  },
  "epub_options": {
    "language": "ja",
    "publisher": "日本語組版プロジェクト"
  },
  "html_options": {
    "embed_css": true,
    "responsive": true
  }
}
```

### 7.4 JIS X 4051参照

JIS X 4051の主要な規定と本ソフトウェアでの対応については、「JIS X 4051実装ガイド」を参照してください。
