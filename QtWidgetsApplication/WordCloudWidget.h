#pragma once
#include <QWidget>
#include <QMap>
#include <QList>
#include <QPointF>
#include <QRectF>
#include <QTimer>

struct WordItem {
    QString text;
    int frequency;
    QRectF rect;
    QPointF position;
    QColor color;
    int fontSize;
};

class WordCloudWidget : public QWidget {
    Q_OBJECT

public:
    explicit WordCloudWidget(QWidget* parent = nullptr);
    void setWordFrequency(const QMap<QString, int>& frequency);
    void saveToImage(const QString& filePath);

    void zoomIn();
    void zoomOut();
    void resetView();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void calculateLayout();
    bool checkCollision(const QRectF& newRect, const QList<QRectF>& existingRects);
    QColor getColorForFrequency(int freq, int maxFreq);
    void regenerateLayout();
    void updateLayoutAsync();

    QMap<QString, int> m_wordFrequency;
    QList<WordItem> m_wordItems;

    // 视图变换
    qreal m_zoom;
    QPointF m_offset;
    QPointF m_lastMousePos;
    bool m_isDragging;

    // 布局缓存
    QRectF m_contentRect;
    QTimer* m_layoutTimer;
    bool m_needsLayout;

    static constexpr int MIN_FONT_SIZE = 21;   // 原来是14，放大1.5倍
    static constexpr int MAX_FONT_SIZE = 78;   // 原来是52，放大1.5倍
    static constexpr qreal SPIRAL_A = 8.0;
    static constexpr qreal SPIRAL_STEP = 0.1;
};