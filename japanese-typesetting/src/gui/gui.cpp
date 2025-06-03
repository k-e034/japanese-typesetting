/**
 * @file gui.cpp
 * @brief GUIインターフェースの詳細実装
 */

#include "japanese_typesetting/gui/gui.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QTextStream>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QSplitter>
#include <QTreeView>
#include <QStandardItemModel>
#include <QFontComboBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QShortcut>
#include <QKeySequence>
#include <QDebug>
#include <fstream>

namespace japanese_typesetting {
namespace gui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_textEdit(nullptr)
    , m_previewView(nullptr)
    , m_stylesDock(nullptr)
    , m_outlineDock(nullptr)
    , m_stylesTabs(nullptr)
    , m_statusBar(nullptr)
    , m_documentModified(false)
    , m_verticalMode(true) // デフォルトは縦書き
{
    // UIの作成
    createUi();
    
    // 設定の読み込み
    loadSettings();
    
    // 新規文書を作成
    newFile();
    
    // ウィンドウサイズの設定
    resize(1024, 768);
}

MainWindow::~MainWindow() {
    // 設定の保存
    saveSettings();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile() {
    if (maybeSave()) {
        m_textEdit->clear();
        m_document = core::document::Document();
        setCurrentFile("");
        m_documentModified = false;
        updateWindowTitle();
        statusBar()->showMessage(tr("新規文書を作成しました"), 2000);
    }
}

void MainWindow::openFile() {
    if (maybeSave()) {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("ファイルを開く"),
            QString(),
            tr("テキストファイル (*.txt);;すべてのファイル (*)")
        );
        
        if (!filePath.isEmpty()) {
            loadDocument(filePath);
        }
    }
}

bool MainWindow::saveFile() {
    if (m_currentFilePath.isEmpty()) {
        return saveFileAs();
    } else {
        return saveDocument(m_currentFilePath);
    }
}

bool MainWindow::saveFileAs() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("名前を付けて保存"),
        m_currentFilePath.isEmpty() ? QString() : m_currentFilePath,
        tr("テキストファイル (*.txt);;すべてのファイル (*)")
    );
    
    if (filePath.isEmpty()) {
        return false;
    }
    
    return saveDocument(filePath);
}

void MainWindow::exportAsPdf() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("PDFとしてエクスポート"),
        m_currentFilePath.isEmpty() ? QString() : m_currentFilePath.replace(QRegExp("\\.[^.]*$"), ".pdf"),
        tr("PDFファイル (*.pdf)")
    );
    
    if (filePath.isEmpty()) {
        return;
    }
    
    // PDFエクスポート処理（将来的に実装）
    statusBar()->showMessage(tr("PDFエクスポート機能は現在実装中です"), 2000);
}

void MainWindow::exportAsEpub() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("EPUBとしてエクスポート"),
        m_currentFilePath.isEmpty() ? QString() : m_currentFilePath.replace(QRegExp("\\.[^.]*$"), ".epub"),
        tr("EPUBファイル (*.epub)")
    );
    
    if (filePath.isEmpty()) {
        return;
    }
    
    // EPUBエクスポート処理（将来的に実装）
    statusBar()->showMessage(tr("EPUBエクスポート機能は現在実装中です"), 2000);
}

void MainWindow::exportAsHtml() {
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("HTMLとしてエクスポート"),
        m_currentFilePath.isEmpty() ? QString() : m_currentFilePath.replace(QRegExp("\\.[^.]*$"), ".html"),
        tr("HTMLファイル (*.html)")
    );
    
    if (filePath.isEmpty()) {
        return;
    }
    
    // HTMLエクスポート処理（将来的に実装）
    statusBar()->showMessage(tr("HTMLエクスポート機能は現在実装中です"), 2000);
}

void MainWindow::undo() {
    m_textEdit->undo();
}

void MainWindow::redo() {
    m_textEdit->redo();
}

void MainWindow::cut() {
    m_textEdit->cut();
}

void MainWindow::copy() {
    m_textEdit->copy();
}

void MainWindow::paste() {
    m_textEdit->paste();
}

void MainWindow::find() {
    // 検索ダイアログ（将来的に実装）
    statusBar()->showMessage(tr("検索機能は現在実装中です"), 2000);
}

void MainWindow::replace() {
    // 置換ダイアログ（将来的に実装）
    statusBar()->showMessage(tr("置換機能は現在実装中です"), 2000);
}

void MainWindow::showSettings() {
    // 設定ダイアログ（将来的に実装）
    statusBar()->showMessage(tr("設定機能は現在実装中です"), 2000);
}

void MainWindow::showHelp() {
    // ヘルプダイアログ（将来的に実装）
    statusBar()->showMessage(tr("ヘルプ機能は現在実装中です"), 2000);
}

void MainWindow::showAbout() {
    QMessageBox::about(
        this,
        tr("バージョン情報"),
        tr("<h3>日本語組版ソフトウェア</h3>"
           "<p>バージョン 0.1.0</p>"
           "<p>JIS X 4051に準拠した高度な日本語組版機能を提供します。</p>"
           "<p>Copyright (C) 2025 Japanese Typesetting Project</p>")
    );
}

void MainWindow::textChanged() {
    if (!m_documentModified) {
        m_documentModified = true;
        updateWindowTitle();
    }
    
    // テキストの変更を文書に反映
    m_document.setContent(m_textEdit->toPlainText().toStdString());
    
    // プレビューの更新
    updatePreview();
}

void MainWindow::styleChanged() {
    // スタイルの変更を反映
    updatePreview();
}

void MainWindow::setVerticalMode(bool vertical) {
    if (m_verticalMode != vertical) {
        m_verticalMode = vertical;
        
        // 縦書き/横書きモードの切り替え
        m_verticalModeAction->setChecked(vertical);
        m_horizontalModeAction->setChecked(!vertical);
        
        // プレビューの更新
        updatePreview();
        
        statusBar()->showMessage(
            vertical ? tr("縦書きモードに切り替えました") : tr("横書きモードに切り替えました"),
            2000
        );
    }
}

void MainWindow::createUi() {
    // 中央ウィジェットの作成
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // レイアウトの作成
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    
    // スプリッターの作成
    QSplitter* splitter = new QSplitter(Qt::Vertical, centralWidget);
    layout->addWidget(splitter);
    
    // テキストエディタの作成
    m_textEdit = new QTextEdit(splitter);
    m_textEdit->setAcceptRichText(false);
    m_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    splitter->addWidget(m_textEdit);
    
    // プレビュービューの作成
    m_previewView = new QGraphicsView(splitter);
    m_previewView->setScene(new QGraphicsScene(m_previewView));
    m_previewView->setRenderHint(QPainter::Antialiasing);
    m_previewView->setRenderHint(QPainter::TextAntialiasing);
    m_previewView->setRenderHint(QPainter::SmoothPixmapTransform);
    splitter->addWidget(m_previewView);
    
    // スプリッターの初期サイズ比率の設定
    splitter->setSizes(QList<int>() << 300 << 400);
    
    // アクションの作成
    createActions();
    
    // メニューの作成
    createMenus();
    
    // ツールバーの作成
    createToolBars();
    
    // ステータスバーの作成
    createStatusBar();
    
    // ドックウィジェットの作成
    createDockWidgets();
    
    // 接続の設定
    createConnections();
}

void MainWindow::createActions() {
    // ファイルアクション
    m_newAction = new QAction(tr("新規(&N)"), this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setStatusTip(tr("新規文書を作成します"));
    
    m_openAction = new QAction(tr("開く(&O)..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip(tr("既存の文書を開きます"));
    
    m_saveAction = new QAction(tr("保存(&S)"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("文書を保存します"));
    
    m_saveAsAction = new QAction(tr("名前を付けて保存(&A)..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip(tr("文書を新しい名前で保存します"));
    
    m_exportPdfAction = new QAction(tr("PDFとしてエクスポート..."), this);
    m_exportPdfAction->setStatusTip(tr("文書をPDFとしてエクスポートします"));
    
    m_exportEpubAction = new QAction(tr("EPUBとしてエクスポート..."), this);
    m_exportEpubAction->setStatusTip(tr("文書をEPUBとしてエクスポートします"));
    
    m_exportHtmlAction = new QAction(tr("HTMLとしてエクスポート..."), this);
    m_exportHtmlAction->setStatusTip(tr("文書をHTMLとしてエクスポートします"));
    
    m_exitAction = new QAction(tr("終了(&Q)"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("アプリケーションを終了します"));
    
    // 編集アクション
    m_undoAction = new QAction(tr("元に戻す(&U)"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_undoAction->setStatusTip(tr("直前の操作を元に戻します"));
    
    m_redoAction = new QAction(tr("やり直し(&R)"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_redoAction->setStatusTip(tr("元に戻した操作をやり直します"));
    
    m_cutAction = new QAction(tr("切り取り(&T)"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_cutAction->setStatusTip(tr("選択したテキストを切り取ります"));
    
    m_copyAction = new QAction(tr("コピー(&C)"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_copyAction->setStatusTip(tr("選択したテキストをコピーします"));
    
    m_pasteAction = new QAction(tr("貼り付け(&P)"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_pasteAction->setStatusTip(tr("クリップボードの内容を貼り付けます"));
    
    m_findAction = new QAction(tr("検索(&F)..."), this);
    m_findAction->setShortcut(QKeySequence::Find);
    m_findAction->setStatusTip(tr("テキストを検索します"));
    
    m_replaceAction = new QAction(tr("置換(&E)..."), this);
    m_replaceAction->setShortcut(QKeySequence::Replace);
    m_replaceAction->setStatusTip(tr("テキストを置換します"));
    
    // 表示アクション
    m_verticalModeAction = new QAction(tr("縦書き(&V)"), this);
    m_verticalModeAction->setCheckable(true);
    m_verticalModeAction->setChecked(true);
    m_verticalModeAction->setStatusTip(tr("縦書きモードに切り替えます"));
    
    m_horizontalModeAction = new QAction(tr("横書き(&H)"), this);
    m_horizontalModeAction->setCheckable(true);
    m_horizontalModeAction->setChecked(false);
    m_horizontalModeAction->setStatusTip(tr("横書きモードに切り替えます"));
    
    // ツールアクション
    m_settingsAction = new QAction(tr("設定(&S)..."), this);
    m_settingsAction->setStatusTip(tr("アプリケーションの設定を変更します"));
    
    // ヘルプアクション
    m_helpAction = new QAction(tr("ヘルプ(&H)"), this);
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    m_helpAction->setStatusTip(tr("ヘルプを表示します"));
    
    m_aboutAction = new QAction(tr("バージョン情報(&A)..."), this);
    m_aboutAction->setStatusTip(tr("アプリケーションのバージョン情報を表示します"));
}

void MainWindow::createMenus() {
    // ファイルメニュー
    m_fileMenu = menuBar()->addMenu(tr("ファイル(&F)"));
    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exportPdfAction);
    m_fileMenu->addAction(m_exportEpubAction);
    m_fileMenu->addAction(m_exportHtmlAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    
    // 編集メニュー
    m_editMenu = menuBar()->addMenu(tr("編集(&E)"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAction);
    m_editMenu->addAction(m_copyAction);
    m_editMenu->addAction(m_pasteAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_findAction);
    m_editMenu->addAction(m_replaceAction);
    
    // 表示メニュー
    m_viewMenu = menuBar()->addMenu(tr("表示(&V)"));
    m_viewMenu->addAction(m_verticalModeAction);
    m_viewMenu->addAction(m_horizontalModeAction);
    m_viewMenu->addSeparator();
    
    // 書式メニュー
    m_formatMenu = menuBar()->addMenu(tr("書式(&O)"));
    
    // ツールメニュー
    m_toolsMenu = menuBar()->addMenu(tr("ツール(&T)"));
    m_toolsMenu->addAction(m_settingsAction);
    
    // ヘルプメニュー
    m_helpMenu = menuBar()->addMenu(tr("ヘルプ(&H)"));
    m_helpMenu->addAction(m_helpAction);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBars() {
    // ファイルツールバー
    m_fileToolBar = addToolBar(tr("ファイル"));
    m_fileToolBar->addAction(m_newAction);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    
    // 編集ツールバー
    m_editToolBar = addToolBar(tr("編集"));
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_cutAction);
    m_editToolBar->addAction(m_copyAction);
    m_editToolBar->addAction(m_pasteAction);
    
    // 書式ツールバー
    m_formatToolBar = addToolBar(tr("書式"));
    m_formatToolBar->addAction(m_verticalModeAction);
    m_formatToolBar->addAction(m_horizontalModeAction);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("準備完了"));
}

void MainWindow::createDockWidgets() {
    // スタイルドック
    m_stylesDock = new QDockWidget(tr("スタイル"), this);
    m_stylesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    m_stylesTabs = new QTabWidget(m_stylesDock);
    
    // 文書スタイルタブ
    QWidget* documentStyleWidget = new QWidget();
    QFormLayout* documentStyleLayout = new QFormLayout(documentStyleWidget);
    
    QComboBox* paperSizeComboBox = new QComboBox();
    paperSizeComboBox->addItem(tr("A4"), QSize(210, 297));
    paperSizeComboBox->addItem(tr("B5"), QSize(176, 250));
    paperSizeComboBox->addItem(tr("A5"), QSize(148, 210));
    paperSizeComboBox->addItem(tr("カスタム"), QSize(0, 0));
    documentStyleLayout->addRow(tr("用紙サイズ:"), paperSizeComboBox);
    
    QDoubleSpinBox* marginTopSpinBox = new QDoubleSpinBox();
    marginTopSpinBox->setRange(0, 100);
    marginTopSpinBox->setValue(20);
    marginTopSpinBox->setSuffix(tr(" mm"));
    documentStyleLayout->addRow(tr("上マージン:"), marginTopSpinBox);
    
    QDoubleSpinBox* marginBottomSpinBox = new QDoubleSpinBox();
    marginBottomSpinBox->setRange(0, 100);
    marginBottomSpinBox->setValue(20);
    marginBottomSpinBox->setSuffix(tr(" mm"));
    documentStyleLayout->addRow(tr("下マージン:"), marginBottomSpinBox);
    
    QDoubleSpinBox* marginLeftSpinBox = new QDoubleSpinBox();
    marginLeftSpinBox->setRange(0, 100);
    marginLeftSpinBox->setValue(20);
    marginLeftSpinBox->setSuffix(tr(" mm"));
    documentStyleLayout->addRow(tr("左マージン:"), marginLeftSpinBox);
    
    QDoubleSpinBox* marginRightSpinBox = new QDoubleSpinBox();
    marginRightSpinBox->setRange(0, 100);
    marginRightSpinBox->setValue(20);
    marginRightSpinBox->setSuffix(tr(" mm"));
    documentStyleLayout->addRow(tr("右マージン:"), marginRightSpinBox);
    
    m_stylesTabs->addTab(documentStyleWidget, tr("文書"));
    
    // テキストスタイルタブ
    QWidget* textStyleWidget = new QWidget();
    QFormLayout* textStyleLayout = new QFormLayout(textStyleWidget);
    
    QFontComboBox* fontFamilyComboBox = new QFontComboBox();
    textStyleLayout->addRow(tr("フォント:"), fontFamilyComboBox);
    
    QDoubleSpinBox* fontSizeSpinBox = new QDoubleSpinBox();
    fontSizeSpinBox->setRange(6, 72);
    fontSizeSpinBox->setValue(10.5);
    fontSizeSpinBox->setSuffix(tr(" pt"));
    textStyleLayout->addRow(tr("サイズ:"), fontSizeSpinBox);
    
    QDoubleSpinBox* lineHeightSpinBox = new QDoubleSpinBox();
    lineHeightSpinBox->setRange(0.5, 3.0);
    lineHeightSpinBox->setValue(1.5);
    lineHeightSpinBox->setSingleStep(0.1);
    textStyleLayout->addRow(tr("行の高さ:"), lineHeightSpinBox);
    
    m_stylesTabs->addTab(textStyleWidget, tr("テキスト"));
    
    // 組版スタイルタブ
    QWidget* typesettingStyleWidget = new QWidget();
    QFormLayout* typesettingStyleLayout = new QFormLayout(typesettingStyleWidget);
    
    QComboBox* textAlignmentComboBox = new QComboBox();
    textAlignmentComboBox->addItem(tr("左揃え"));
    textAlignmentComboBox->addItem(tr("中央揃え"));
    textAlignmentComboBox->addItem(tr("右揃え"));
    textAlignmentComboBox->addItem(tr("両端揃え"));
    textAlignmentComboBox->setCurrentIndex(3); // デフォルトは両端揃え
    typesettingStyleLayout->addRow(tr("テキスト配置:"), textAlignmentComboBox);
    
    QDoubleSpinBox* characterSpacingSpinBox = new QDoubleSpinBox();
    characterSpacingSpinBox->setRange(-0.5, 2.0);
    characterSpacingSpinBox->setValue(0.0);
    characterSpacingSpinBox->setSingleStep(0.05);
    typesettingStyleLayout->addRow(tr("文字間隔:"), characterSpacingSpinBox);
    
    QCheckBox* hangingPunctuationCheckBox = new QCheckBox(tr("句読点のぶら下げを有効にする"));
    hangingPunctuationCheckBox->setChecked(true);
    typesettingStyleLayout->addRow("", hangingPunctuationCheckBox);
    
    m_stylesTabs->addTab(typesettingStyleWidget, tr("組版"));
    
    m_stylesDock->setWidget(m_stylesTabs);
    addDockWidget(Qt::RightDockWidgetArea, m_stylesDock);
    m_viewMenu->addAction(m_stylesDock->toggleViewAction());
    
    // アウトラインドック
    m_outlineDock = new QDockWidget(tr("アウトライン"), this);
    m_outlineDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    QTreeView* outlineTreeView = new QTreeView(m_outlineDock);
    QStandardItemModel* outlineModel = new QStandardItemModel(outlineTreeView);
    outlineModel->setHorizontalHeaderLabels(QStringList() << tr("見出し"));
    outlineTreeView->setModel(outlineModel);
    
    m_outlineDock->setWidget(outlineTreeView);
    addDockWidget(Qt::LeftDockWidgetArea, m_outlineDock);
    m_viewMenu->addAction(m_outlineDock->toggleViewAction());
}

void MainWindow::createConnections() {
    // ファイルアクション
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(m_exportPdfAction, &QAction::triggered, this, &MainWindow::exportAsPdf);
    connect(m_exportEpubAction, &QAction::triggered, this, &MainWindow::exportAsEpub);
    connect(m_exportHtmlAction, &QAction::triggered, this, &MainWindow::exportAsHtml);
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    
    // 編集アクション
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redo);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::cut);
    connect(m_copyAction, &QAction::triggered, this, &MainWindow::copy);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::paste);
    connect(m_findAction, &QAction::triggered, this, &MainWindow::find);
    connect(m_replaceAction, &QAction::triggered, this, &MainWindow::replace);
    
    // 表示アクション
    connect(m_verticalModeAction, &QAction::triggered, this, [this]() { setVerticalMode(true); });
    connect(m_horizontalModeAction, &QAction::triggered, this, [this]() { setVerticalMode(false); });
    
    // ツールアクション
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    
    // ヘルプアクション
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::showHelp);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    
    // テキスト変更
    connect(m_textEdit, &QTextEdit::textChanged, this, &MainWindow::textChanged);
}

void MainWindow::loadSettings() {
    QSettings settings("Japanese Typesetting Project", "Japanese Typesetting Software");
    
    // ウィンドウの位置とサイズ
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(1024, 768)).toSize();
    move(pos);
    resize(size);
    
    // 縦書き/横書きモード
    bool vertical = settings.value("vertical", true).toBool();
    setVerticalMode(vertical);
    
    // 最近使用したファイル
    // （将来的に実装）
}

void MainWindow::saveSettings() {
    QSettings settings("Japanese Typesetting Project", "Japanese Typesetting Software");
    
    // ウィンドウの位置とサイズ
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    
    // 縦書き/横書きモード
    settings.setValue("vertical", m_verticalMode);
    
    // 最近使用したファイル
    // （将来的に実装）
}

bool MainWindow::loadDocument(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            tr("ファイルを開けません"),
            tr("ファイル %1 を開くことができませんでした。").arg(filePath)
        );
        return false;
    }
    
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString text = in.readAll();
    file.close();
    
    m_textEdit->setPlainText(text);
    m_document.setContent(text.toStdString());
    setCurrentFile(filePath);
    m_documentModified = false;
    updateWindowTitle();
    
    statusBar()->showMessage(tr("ファイルを読み込みました: %1").arg(filePath), 2000);
    
    return true;
}

bool MainWindow::saveDocument(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            tr("ファイルを保存できません"),
            tr("ファイル %1 に保存することができませんでした。").arg(filePath)
        );
        return false;
    }
    
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << m_textEdit->toPlainText();
    file.close();
    
    setCurrentFile(filePath);
    m_documentModified = false;
    updateWindowTitle();
    
    statusBar()->showMessage(tr("ファイルを保存しました: %1").arg(filePath), 2000);
    
    return true;
}

void MainWindow::updatePreview() {
    // プレビューの更新
    QGraphicsScene* scene = m_previewView->scene();
    scene->clear();
    
    // 文書の組版
    double contentWidth = 500.0; // 仮の幅
    std::vector<core::typesetting::TextBlock> blocks = m_typesettingEngine.typesetDocument(m_document, m_style, contentWidth);
    
    // 組版結果の描画（簡易的な実装）
    double y = 20.0;
    for (const auto& block : blocks) {
        for (const auto& line : block.lines) {
            // UTF-32からUTF-8に変換
            core::unicode::UnicodeHandler unicodeHandler;
            std::string utf8Text = unicodeHandler.utf32ToUtf8(line.text);
            
            // テキストアイテムの作成
            QGraphicsTextItem* textItem = scene->addText(QString::fromUtf8(utf8Text.c_str()));
            
            // 縦書き/横書きモードに応じた配置
            if (m_verticalMode) {
                // 縦書きモード（簡易的な実装）
                textItem->setRotation(90);
                textItem->setPos(20.0 + y, 20.0);
                y += line.width + 5.0;
            } else {
                // 横書きモード
                textItem->setPos(20.0, y);
                y += line.height + 5.0;
            }
        }
        
        y += 20.0; // ブロック間の余白
    }
    
    // ビューの更新
    m_previewView->update();
}

bool MainWindow::maybeSave() {
    if (!m_documentModified) {
        return true;
    }
    
    QMessageBox::StandardButton ret = QMessageBox::warning(
        this,
        tr("変更を保存"),
        tr("文書が変更されています。変更を保存しますか？"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );
    
    if (ret == QMessageBox::Save) {
        return saveFile();
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    
    return true;
}

void MainWindow::setCurrentFile(const QString& filePath) {
    m_currentFilePath = filePath;
    updateWindowTitle();
}

void MainWindow::updateWindowTitle() {
    QString title = tr("日本語組版ソフトウェア");
    
    if (!m_currentFilePath.isEmpty()) {
        QFileInfo fileInfo(m_currentFilePath);
        title = fileInfo.fileName() + " - " + title;
    }
    
    if (m_documentModified) {
        title = "* " + title;
    }
    
    setWindowTitle(title);
}

} // namespace gui
} // namespace japanese_typesetting
