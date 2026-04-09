#include "WordCloudWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFontMetrics>
#include <QtMath>
#include <QDebug>

WordCloudWidget::WordCloudWidget(QWidget* parent)
    : QWidget(parent)
    , m_zoom(1.0)
    , m_offset(0, 0)
    , m_isDragging(false)
    , m_needsLayout(false)
{
    setMinimumSize(400, 300);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    m_layoutTimer = new QTimer(this);
    m_layoutTimer->setSingleShot(true);
    m_layoutTimer->setInterval(200);  // 增加延迟，避免频繁计算
    connect(m_layoutTimer, &QTimer::timeout, this, &WordCloudWidget::regenerateLayout);
}

void WordCloudWidget::setWordFrequency(const QMap<QString, int>& frequency)
{
    m_wordFrequency = frequency;
    m_needsLayout = true;
    m_layoutTimer->start();
}

void WordCloudWidget::calculateLayout()
{
    if (m_wordFrequency.isEmpty()) return;

    m_wordItems.clear();

    // 1. 转换为列表并按词频排序
    QList<QPair<QString, int>> sortedWords;
    for (auto it = m_wordFrequency.begin(); it != m_wordFrequency.end(); ++it) {
        sortedWords.append(qMakePair(it.key(), it.value()));
    }
    std::sort(sortedWords.begin(), sortedWords.end(),
        [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
            return a.second > b.second;
        });

    int maxFreq = sortedWords.first().second;
    int minFreq = sortedWords.last().second;
    if (minFreq == maxFreq) minFreq = 0;

    // 2. 计算画布中心
    QRectF canvasRect = rect();
    if (canvasRect.width() < 100 || canvasRect.height() < 100) {
        canvasRect = QRectF(0, 0, 800, 600);
    }
    QPointF center(canvasRect.width() / 2, canvasRect.height() / 2);

    QList<QRectF> placedRects;

    // 3. 螺旋布局
    for (const auto& pair : sortedWords) {
        WordItem item;
        item.text = pair.first;
        item.frequency = pair.second;

        // 计算字体大小（已放大1.5倍）
        int range = maxFreq - minFreq;
        if (range > 0) {
            item.fontSize = MIN_FONT_SIZE +
                (pair.second - minFreq) * (MAX_FONT_SIZE - MIN_FONT_SIZE) / range;
        }
        else {
            item.fontSize = (MIN_FONT_SIZE + MAX_FONT_SIZE) / 2;
        }

        // 获取文字矩形
        QFont font("楷体", item.fontSize);
        QFontMetrics fmItem(font);
        QRectF textRect = fmItem.boundingRect(item.text);
        QRectF itemRect(0, 0, textRect.width() + 20, textRect.height() + 10);

        // 螺旋搜索位置
        qreal angle = 0;
        qreal radius = 0;
        bool placed = false;

        for (int step = 0; step < 500; ++step) {
            radius = SPIRAL_A * angle;
            qreal x = center.x() + radius * qCos(angle);
            qreal y = center.y() + radius * qSin(angle);

            QRectF candidateRect(x - itemRect.width() / 2,
                y - itemRect.height() / 2,
                itemRect.width(), itemRect.height());

            // 边界检查
            if (candidateRect.left() < 10 || candidateRect.right() > canvasRect.width() - 10 ||
                candidateRect.top() < 10 || candidateRect.bottom() > canvasRect.height() - 10) {
                angle += SPIRAL_STEP;
                continue;
            }

            // 碰撞检测
            if (!checkCollision(candidateRect, placedRects)) {
                item.rect = candidateRect;
                item.position = QPointF(x, y);
                item.color = getColorForFrequency(pair.second, maxFreq);
                placed = true;
                placedRects.append(candidateRect);
                break;
            }

            angle += SPIRAL_STEP;
        }

        // 如果找不到合适位置，放在角落
        if (!placed) {
            item.rect = QRectF(10, 10, itemRect.width(), itemRect.height());
            item.position = QPointF(10 + itemRect.width() / 2, 10 + itemRect.height() / 2);
            item.color = getColorForFrequency(pair.second, maxFreq);
        }

        m_wordItems.append(item);
    }

    // 更新内容矩形
    m_contentRect = QRectF();
    for (const WordItem& item : m_wordItems) {
        m_contentRect = m_contentRect.united(item.rect);
    }

    m_needsLayout = false;
}

bool WordCloudWidget::checkCollision(const QRectF& newRect, const QList<QRectF>& existingRects)
{
    for (const QRectF& rect : existingRects) {
        if (newRect.intersects(rect)) {
            return true;
        }
    }
    return false;
}

QColor WordCloudWidget::getColorForFrequency(int freq, int maxFreq)
{
    qreal ratio = (qreal)freq / maxFreq;

    // 加深颜色，使用更浓郁的国风色彩
    if (ratio > 0.7) {
        return QColor(101, 47, 24);      // 深赭石色
    }
    else if (ratio > 0.4) {
        return QColor(139, 69, 19);       // 标准赭石色
    }
    else if (ratio > 0.2) {
        return QColor(160, 82, 45);       // 土棕色
    }
    else {
        return QColor(184, 115, 51);      // 深杏色
    }
}

void WordCloudWidget::regenerateLayout()
{
    calculateLayout();
    update();
}

void WordCloudWidget::updateLayoutAsync()
{
    if (m_needsLayout) {
        calculateLayout();
        update();
    }
}

void WordCloudWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 绘制背景
    painter.fillRect(rect(), QColor(255, 248, 240));

    // 如果还没有布局，先计算
    if (m_needsLayout && !m_wordFrequency.isEmpty()) {
        calculateLayout();
    }

    // 应用变换
    painter.save();
    painter.translate(m_offset);
    painter.scale(m_zoom, m_zoom);

    // 绘制词云
    for (const WordItem& item : m_wordItems) {
        QFont font("楷体", item.fontSize);
        font.setBold(true);
        painter.setFont(font);

        // 绘制主文字
        painter.setPen(QPen(item.color, 2));
        painter.drawText(item.rect, Qt::AlignCenter, item.text);
    }

    painter.restore();

    // 如果没有数据
    if (m_wordItems.isEmpty() && !m_wordFrequency.isEmpty()) {
        painter.setPen(QColor(160, 146, 123));
        QFont font("楷体", 18);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "正在生成词云图...");
    }
    else if (m_wordItems.isEmpty()) {
        painter.setPen(QColor(160, 146, 123));
        QFont font("楷体", 18);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "暂无阅读历史\n请先阅读一些诗歌");
    }
}

void WordCloudWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
}

void WordCloudWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging) {
        QPointF delta = event->pos() - m_lastMousePos;
        m_offset += delta;
        m_lastMousePos = event->pos();
        update();
        event->accept();
    }
}

void WordCloudWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
}

void WordCloudWidget::wheelEvent(QWheelEvent* event)
{
    // 获取鼠标在控件上的位置
    QPointF mousePos = event->position();

    // 计算缩放前鼠标对应的场景坐标
    QPointF scenePosBefore = (mousePos - m_offset) / m_zoom;

    // 计算缩放因子
    qreal factor = 1.15;  // 使用更平滑的缩放因子
    if (event->angleDelta().y() < 0) {
        factor = 1.0 / factor;
    }

    // 应用缩放
    qreal newZoom = m_zoom * factor;
    newZoom = qBound(0.3, newZoom, 5.0);

    if (newZoom != m_zoom) {
        // 计算缩放后需要的新偏移量，使鼠标位置保持不变
        QPointF scenePosAfter = (mousePos - m_offset) / newZoom;
        m_offset = mousePos - scenePosBefore * newZoom;
        m_zoom = newZoom;

        update();
    }

    event->accept();
}

void WordCloudWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    m_needsLayout = true;
    m_layoutTimer->start();
}

void WordCloudWidget::zoomIn()
{
    // 以视图中心为缩放中心
    QPointF center(width() / 2.0, height() / 2.0);
    QPointF scenePosBefore = (center - m_offset) / m_zoom;

    qreal newZoom = m_zoom * 1.2;
    newZoom = qMin(newZoom, 5.0);

    if (newZoom != m_zoom) {
        m_offset = center - scenePosBefore * newZoom;
        m_zoom = newZoom;
        update();
    }
}

void WordCloudWidget::zoomOut()
{
    // 以视图中心为缩放中心
    QPointF center(width() / 2.0, height() / 2.0);
    QPointF scenePosBefore = (center - m_offset) / m_zoom;

    qreal newZoom = m_zoom / 1.2;
    newZoom = qMax(newZoom, 0.3);

    if (newZoom != m_zoom) {
        m_offset = center - scenePosBefore * newZoom;
        m_zoom = newZoom;
        update();
    }
}

void WordCloudWidget::resetView()
{
    m_zoom = 1.0;
    m_offset = QPointF(0, 0);
    update();
}

void WordCloudWidget::saveToImage(const QString& filePath)
{
    QPixmap pixmap(size());
    pixmap.fill(QColor(255, 248, 240));

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 临时保存当前变换
    QPointF oldOffset = m_offset;
    qreal oldZoom = m_zoom;

    // 重置变换以便完整绘制
    m_offset = QPointF(0, 0);
    m_zoom = 1.0;

    // 绘制
    for (const WordItem& item : m_wordItems) {
        QFont font("楷体", item.fontSize);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(QPen(item.color, 2));
        painter.drawText(item.rect, Qt::AlignCenter, item.text);
    }

    // 恢复变换
    m_offset = oldOffset;
    m_zoom = oldZoom;

    pixmap.save(filePath);
}