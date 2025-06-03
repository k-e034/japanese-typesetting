/**
 * @file main_gui.cpp
 * @brief GUIアプリケーションのエントリーポイント
 */

#include "japanese_typesetting/gui/gui.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

int main(int argc, char* argv[]) {
    try {
        // アプリケーションの作成
        QApplication app(argc, argv);
        
        // アプリケーション情報の設定
        QApplication::setApplicationName("Japanese Typesetting Software");
        QApplication::setApplicationVersion("0.1.0");
        QApplication::setOrganizationName("Japanese Typesetting Project");
        QApplication::setOrganizationDomain("japanese-typesetting.example.com");
        
        // 翻訳の設定
        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(),
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&qtTranslator);
        
        QTranslator appTranslator;
        appTranslator.load("japanese_typesetting_" + QLocale::system().name());
        app.installTranslator(&appTranslator);
        
        // メインウィンドウの作成と表示
        japanese_typesetting::gui::MainWindow mainWindow;
        mainWindow.show();
        
        // アプリケーションの実行
        return app.exec();
    } catch (const std::exception& e) {
        // 例外処理
        QMessageBox::critical(nullptr, "致命的なエラー", QString("エラーが発生しました: %1").arg(e.what()));
        return 1;
    } catch (...) {
        // 不明な例外
        QMessageBox::critical(nullptr, "致命的なエラー", "不明なエラーが発生しました");
        return 1;
    }
}
