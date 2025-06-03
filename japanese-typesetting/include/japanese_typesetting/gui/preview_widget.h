/**
 * @file preview_widget.h
 * @brief リアルタイムプレビューウィジェットの詳細実装
 */

#ifndef JAPANESE_TYPESETTING_GUI_PREVIEW_WIDGET_H
#define JAPANESE_TYPESETTING_GUI_PREVIEW_WIDGET_H

#include "japanese_typesetting/core/document/document.h"
#include "japanese_typesetting/core/style/style.h"
#include "japanese_typesetting/core/typesetting/typesetting_engine.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <memory>

namespace japanese_typesetting {
namespace gui {

/**
 * @class PreviewWidget
 * @brief リアルタイムプレビューを表示するウィジェット
 */
class PreviewWidget : public QGraphicsView {
    Q_OBJECT

public:
    /**
     * @brief コンストラクタ
     * @param parent 親ウィジェット
     */
    explicit PreviewWidget(QWidget* parent = nullptr);

    /**
     * @brief デストラクタ
     */
    ~PreviewWidget();

    /**
     * @brief 文書を設定
     * @param document 文書
     */
    void setDocument(const core::document::Document& document);

    /**
     * @brief スタイルを設定
     * @param style スタイル
     */
    void setStyle(const core::style::Style& style);

    /**
     * @brief 縦書きモードを設定
     * @param vertical 縦書きの場合はtrue
     */
    void setVerticalMode(bool vertical);

    /**
     * @brief プレビューを更新
     */
    void updatePreview();

    /**
     * @brief ズームレベルを設定
     * @param zoomLevel ズームレベル（1.0 = 100%）
     */
    void setZoomLevel(double zoomLevel);

    /**
     * @brief ズームレベルを取得
     * @return ズームレベル
     */
    double getZoomLevel() const;

protected:
    /**
     * @brief リサイズイベント
     * @param event イベント
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief ホイールイベント
     * @param event イベント
     */
    void wheelEvent(QWheelEvent* event) override;

    /**
     * @brief マウスプレスイベント
     * @param event イベント
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief マウスムーブイベント
     * @param event イベント
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief マウスリリースイベント
     * @param event イベント
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    /**
     * @brief 組版結果を描画
     * @param blocks 組版されたテキストブロックのリスト
     */
    void renderBlocks(const std::vector<core::typesetting::TextBlock>& blocks);

    /**
     * @brief ページガイドを描画
     */
    void renderPageGuides();

    QGraphicsScene* m_scene;                ///< グラフィックスシーン
    core::document::Document m_document;    ///< 文書
    core::style::Style m_style;             ///< スタイル
    core::typesetting::TypesettingEngine m_typesettingEngine; ///< 組版エンジン
    bool m_verticalMode;                    ///< 縦書きモードかどうか
    double m_zoomLevel;                     ///< ズームレベル
    QPointF m_lastMousePos;                 ///< 最後のマウス位置
    bool m_isPanning;                       ///< パン中かどうか
    double m_pageWidth;                     ///< ページ幅（mm）
    double m_pageHeight;                    ///< ページ高さ（mm）
    double m_marginTop;                     ///< 上マージン（mm）
    double m_marginBottom;                  ///< 下マージン（mm）
    double m_marginLeft;                    ///< 左マージン（mm）
    double m_marginRight;                   ///< 右マージン（mm）
};

} // namespace gui
} // namespace japanese_typesetting

#endif // JAPANESE_TYPESETTING_GUI_PREVIEW_WIDGET_H
