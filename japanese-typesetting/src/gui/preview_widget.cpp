/**
 * @file preview_widget.cpp
 * @brief リアルタイムプレビューウィジェットの詳細実装
 */

#include "japanese_typesetting/gui/preview_widget.h"
#include <QPainter>
#include <QScrollBar>
#include <QApplication>
#include <cmath>

namespace japanese_typesetting {
namespace gui {

PreviewWidget::PreviewWidget(QWidget* parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
    , m_verticalMode(true)
    , m_zoomLevel(1.0)
    , m_isPanning(false)
    , m_pageWidth(210.0)  // A4
    , m_pageHeight(297.0) // A4
    , m_marginTop(20.0)
    , m_marginBottom(20.0)
    , m_marginLeft(20.0)
    , m_marginRight(20.0)
{
    // シーンの設定
    setScene(m_scene);
    
    // 描画設定
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::TextAntialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    
    // スクロールバーの設定
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // ドラッグモードの設定
    setDragMode(QGraphicsView::NoDrag);
    
    // 背景色の設定
    setBackgroundBrush(QBrush(QColor(240, 240, 240)));
    
    // 初期表示
    updatePreview();
}

PreviewWidget::~PreviewWidget() {
    // 特に何もしない
}

void PreviewWidget::setDocument(const core::document::Document& document) {
    m_document = document;
    updatePreview();
}

void PreviewWidget::setStyle(const core::style::Style& style) {
    m_style = style;
    updatePreview();
}

void PreviewWidget::setVerticalMode(bool vertical) {
    if (m_verticalMode != vertical) {
        m_verticalMode = vertical;
        updatePreview();
    }
}

void PreviewWidget::updatePreview() {
    // シーンのクリア
    m_scene->clear();
    
    // ページガイドの描画
    renderPageGuides();
    
    // 文書の組版
    double contentWidth = m_pageWidth - m_marginLeft - m_marginRight;
    std::vector<core::typesetting::TextBlock> blocks = m_typesettingEngine.typesetDocument(m_document, m_style, contentWidth);
    
    // 組版結果の描画
    renderBlocks(blocks);
    
    // ビューの更新
    update();
}

void PreviewWidget::setZoomLevel(double zoomLevel) {
    // ズームレベルの制限（10% ～ 500%）
    zoomLevel = std::max(0.1, std::min(5.0, zoomLevel));
    
    if (m_zoomLevel != zoomLevel) {
        m_zoomLevel = zoomLevel;
        
        // スケールの設定
        QTransform transform;
        transform.scale(m_zoomLevel, m_zoomLevel);
        setTransform(transform);
        
        // ビューの更新
        update();
    }
}

double PreviewWidget::getZoomLevel() const {
    return m_zoomLevel;
}

void PreviewWidget::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    
    // シーンの表示範囲を調整
    QRectF sceneRect = m_scene->sceneRect();
    if (sceneRect.width() < viewport()->width()) {
        sceneRect.setWidth(viewport()->width());
    }
    if (sceneRect.height() < viewport()->height()) {
        sceneRect.setHeight(viewport()->height());
    }
    m_scene->setSceneRect(sceneRect);
    
    // ページを中央に配置
    centerOn(m_pageWidth / 2, m_pageHeight / 2);
}

void PreviewWidget::wheelEvent(QWheelEvent* event) {
    // Ctrlキーが押されている場合はズーム
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        // ホイールの回転量に基づいてズームレベルを調整
        double delta = event->angleDelta().y() / 120.0;
        double factor = std::pow(1.2, delta);
        setZoomLevel(m_zoomLevel * factor);
        
        // ズーム中心を設定
        centerOn(mapToScene(event->position().toPoint()));
    } else {
        // 通常のスクロール
        QGraphicsView::wheelEvent(event);
    }
}

void PreviewWidget::mousePressEvent(QMouseEvent* event) {
    // 中ボタンまたはAltキー+左ボタンでパン開始
    if (event->button() == Qt::MiddleButton || 
        (event->button() == Qt::LeftButton && (QApplication::keyboardModifiers() & Qt::AltModifier))) {
        m_isPanning = true;
        m_lastMousePos = event->position();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void PreviewWidget::mouseMoveEvent(QMouseEvent* event) {
    if (m_isPanning) {
        // パン処理
        QPointF delta = event->position() - m_lastMousePos;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        m_lastMousePos = event->position();
        event->accept();
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (m_isPanning && (event->button() == Qt::MiddleButton || event->button() == Qt::LeftButton)) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void PreviewWidget::renderBlocks(const std::vector<core::typesetting::TextBlock>& blocks) {
    // mmをピクセルに変換する係数（仮の値）
    const double mmToPixel = 3.0;
    
    // 描画開始位置
    double x = m_marginLeft * mmToPixel;
    double y = m_marginTop * mmToPixel;
    
    // 各ブロックを描画
    for (const auto& block : blocks) {
        for (const auto& line : block.lines) {
            // UTF-32からUTF-8に変換
            core::unicode::UnicodeHandler unicodeHandler;
            std::string utf8Text = unicodeHandler.utf32ToUtf8(line.text);
            QString text = QString::fromUtf8(utf8Text.c_str());
            
            // テキストアイテムの作成
            QGraphicsTextItem* textItem = m_scene->addText(text);
            
            // フォント設定
            QFont font = textItem->font();
            font.setFamily(QString::fromStdString(m_style.getFontFamily()));
            font.setPointSizeF(m_style.getFontSize());
            textItem->setFont(font);
            
            // 縦書き/横書きモードに応じた配置
            if (m_verticalMode) {
                // 縦書きモード
                textItem->setRotation(90);
                textItem->setPos(x, y);
                y += line.width * mmToPixel;
                
                // 行の高さが最大幅を超える場合は次の列に移動
                if (y > (m_pageHeight - m_marginBottom) * mmToPixel) {
                    y = m_marginTop * mmToPixel;
                    x += line.height * mmToPixel;
                }
            } else {
                // 横書きモード
                textItem->setPos(x, y);
                y += line.height * mmToPixel;
                
                // 行の高さが最大高さを超える場合は次のページに移動（簡易実装）
                if (y > (m_pageHeight - m_marginBottom) * mmToPixel) {
                    y = m_marginTop * mmToPixel;
                    x += (m_pageWidth + 20) * mmToPixel; // ページ間の余白を追加
                }
            }
        }
        
        // ブロック間の余白
        if (m_verticalMode) {
            y += 10 * mmToPixel;
        } else {
            y += 15 * mmToPixel;
        }
    }
    
    // シーンの範囲を設定
    double sceneWidth = std::max(x + 100, m_pageWidth * mmToPixel + 100);
    double sceneHeight = std::max(y + 100, m_pageHeight * mmToPixel + 100);
    m_scene->setSceneRect(0, 0, sceneWidth, sceneHeight);
}

void PreviewWidget::renderPageGuides() {
    // mmをピクセルに変換する係数（仮の値）
    const double mmToPixel = 3.0;
    
    // ページの幅と高さ
    double pageWidth = m_pageWidth * mmToPixel;
    double pageHeight = m_pageHeight * mmToPixel;
    
    // マージン
    double marginTop = m_marginTop * mmToPixel;
    double marginBottom = m_marginBottom * mmToPixel;
    double marginLeft = m_marginLeft * mmToPixel;
    double marginRight = m_marginRight * mmToPixel;
    
    // ページの背景
    QGraphicsRectItem* pageRect = m_scene->addRect(0, 0, pageWidth, pageHeight);
    pageRect->setBrush(QBrush(Qt::white));
    pageRect->setPen(QPen(Qt::gray));
    
    // 本文領域
    QGraphicsRectItem* contentRect = m_scene->addRect(
        marginLeft, marginTop,
        pageWidth - marginLeft - marginRight,
        pageHeight - marginTop - marginBottom
    );
    contentRect->setPen(QPen(QColor(200, 200, 200, 150), 0.5));
    
    // 中心線
    QGraphicsLineItem* centerHLine = m_scene->addLine(0, pageHeight / 2, pageWidth, pageHeight / 2);
    centerHLine->setPen(QPen(QColor(200, 200, 200, 100), 0.5, Qt::DashLine));
    
    QGraphicsLineItem* centerVLine = m_scene->addLine(pageWidth / 2, 0, pageWidth / 2, pageHeight);
    centerVLine->setPen(QPen(QColor(200, 200, 200, 100), 0.5, Qt::DashLine));
}

} // namespace gui
} // namespace japanese_typesetting
