/**
 * @file gui.h
 * @brief GUIインターフェースの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_GUI_GUI_H
#define JAPANESE_TYPESETTING_GUI_GUI_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QGraphicsView>
#include <QDockWidget>
#include <QTabWidget>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QString>
#include <memory>

namespace japanese_typesetting {
namespace gui {

/**
 * @class MainWindow
 * @brief メインウィンドウクラス
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief コンストラクタ
     * @param parent 親ウィジェット
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief デストラクタ
     */
    ~MainWindow();

protected:
    /**
     * @brief ウィンドウを閉じるイベント
     * @param event イベント
     */
    void closeEvent(QCloseEvent* event) override;

private slots:
    /**
     * @brief 新規ファイルを作成
     */
    void newFile();

    /**
     * @brief ファイルを開く
     */
    void openFile();

    /**
     * @brief ファイルを保存
     * @return 保存に成功した場合はtrue
     */
    bool saveFile();

    /**
     * @brief 名前を付けてファイルを保存
     * @return 保存に成功した場合はtrue
     */
    bool saveFileAs();

    /**
     * @brief PDFとしてエクスポート
     */
    void exportAsPdf();

    /**
     * @brief EPUBとしてエクスポート
     */
    void exportAsEpub();

    /**
     * @brief HTMLとしてエクスポート
     */
    void exportAsHtml();

    /**
     * @brief 元に戻す
     */
    void undo();

    /**
     * @brief やり直す
     */
    void redo();

    /**
     * @brief 切り取り
     */
    void cut();

    /**
     * @brief コピー
     */
    void copy();

    /**
     * @brief 貼り付け
     */
    void paste();

    /**
     * @brief 検索
     */
    void find();

    /**
     * @brief 置換
     */
    void replace();

    /**
     * @brief 設定ダイアログを表示
     */
    void showSettings();

    /**
     * @brief ヘルプを表示
     */
    void showHelp();

    /**
     * @brief バージョン情報を表示
     */
    void showAbout();

    /**
     * @brief テキストが変更された時の処理
     */
    void textChanged();

    /**
     * @brief スタイルが変更された時の処理
     */
    void styleChanged();

    /**
     * @brief 縦書き/横書きを切り替え
     * @param vertical 縦書きの場合はtrue
     */
    void setVerticalMode(bool vertical);

private:
    /**
     * @brief UIを作成
     */
    void createUi();

    /**
     * @brief アクションを作成
     */
    void createActions();

    /**
     * @brief メニューを作成
     */
    void createMenus();

    /**
     * @brief ツールバーを作成
     */
    void createToolBars();

    /**
     * @brief ステータスバーを作成
     */
    void createStatusBar();

    /**
     * @brief ドックウィジェットを作成
     */
    void createDockWidgets();

    /**
     * @brief 接続を設定
     */
    void createConnections();

    /**
     * @brief 設定を読み込む
     */
    void loadSettings();

    /**
     * @brief 設定を保存する
     */
    void saveSettings();

    /**
     * @brief 文書を読み込む
     * @param filePath ファイルパス
     * @return 読み込みに成功した場合はtrue
     */
    bool loadDocument(const QString& filePath);

    /**
     * @brief 文書を保存する
     * @param filePath ファイルパス
     * @return 保存に成功した場合はtrue
     */
    bool saveDocument(const QString& filePath);

    /**
     * @brief プレビューを更新する
     */
    void updatePreview();

    /**
     * @brief 変更があるか確認する
     * @return 変更がある場合はtrue
     */
    bool maybeSave();

    /**
     * @brief ファイル名を設定する
     * @param filePath ファイルパス
     */
    void setCurrentFile(const QString& filePath);

    /**
     * @brief ウィンドウタイトルを更新する
     */
    void updateWindowTitle();

    // UI要素
    QTextEdit* m_textEdit;                 ///< テキストエディタ
    QGraphicsView* m_previewView;          ///< プレビュービュー
    QDockWidget* m_stylesDock;             ///< スタイルドック
    QDockWidget* m_outlineDock;            ///< アウトラインドック
    QTabWidget* m_stylesTabs;              ///< スタイルタブ
    QStatusBar* m_statusBar;               ///< ステータスバー

    // アクション
    QAction* m_newAction;                  ///< 新規作成アクション
    QAction* m_openAction;                 ///< 開くアクション
    QAction* m_saveAction;                 ///< 保存アクション
    QAction* m_saveAsAction;               ///< 名前を付けて保存アクション
    QAction* m_exportPdfAction;            ///< PDFエクスポートアクション
    QAction* m_exportEpubAction;           ///< EPUBエクスポートアクション
    QAction* m_exportHtmlAction;           ///< HTMLエクスポートアクション
    QAction* m_exitAction;                 ///< 終了アクション
    QAction* m_undoAction;                 ///< 元に戻すアクション
    QAction* m_redoAction;                 ///< やり直しアクション
    QAction* m_cutAction;                  ///< 切り取りアクション
    QAction* m_copyAction;                 ///< コピーアクション
    QAction* m_pasteAction;                ///< 貼り付けアクション
    QAction* m_findAction;                 ///< 検索アクション
    QAction* m_replaceAction;              ///< 置換アクション
    QAction* m_settingsAction;             ///< 設定アクション
    QAction* m_helpAction;                 ///< ヘルプアクション
    QAction* m_aboutAction;                ///< バージョン情報アクション
    QAction* m_verticalModeAction;         ///< 縦書きモードアクション
    QAction* m_horizontalModeAction;       ///< 横書きモードアクション

    // メニュー
    QMenu* m_fileMenu;                     ///< ファイルメニュー
    QMenu* m_editMenu;                     ///< 編集メニュー
    QMenu* m_viewMenu;                     ///< 表示メニュー
    QMenu* m_formatMenu;                   ///< 書式メニュー
    QMenu* m_toolsMenu;                    ///< ツールメニュー
    QMenu* m_helpMenu;                     ///< ヘルプメニュー

    // ツールバー
    QToolBar* m_fileToolBar;               ///< ファイルツールバー
    QToolBar* m_editToolBar;               ///< 編集ツールバー
    QToolBar* m_formatToolBar;             ///< 書式ツールバー

    // 文書関連
    QString m_currentFilePath;             ///< 現在のファイルパス
    bool m_documentModified;               ///< 文書が変更されたかどうか
    core::document::Document m_document;   ///< 文書
    core::style::Style m_style;            ///< スタイル
    core::typesetting::TypesettingEngine m_typesettingEngine; ///< 組版エンジン
    bool m_verticalMode;                   ///< 縦書きモードかどうか
};

} // namespace gui
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_GUI_GUI_H
