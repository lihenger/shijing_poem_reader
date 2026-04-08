// ScrollTitle.h
#pragma once
#include <QLabel>
#include <QPainter>
#include <QWidget>

class ScrollTitle : public QLabel {
    Q_OBJECT

public:
    explicit ScrollTitle(const QString& text, QWidget* parent = nullptr)
        : QLabel(text, parent) {
        setAlignment(Qt::AlignCenter);
        setMinimumHeight(80);
        setStyleSheet("color: #8B4513; font-size: 28px; font-weight: bold; background: transparent;");
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QRect rect = this->rect();

        QColor scrollColor(139, 69, 19);
        QColor scrollLight(180, 110, 50);

        int axleWidth = 20;
        int axleHeight = rect.height() - 10;

        // 左轴头
        painter.setBrush(QBrush(scrollColor));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect.left(), rect.top() + 5, axleWidth, axleHeight, 5, 5);
        painter.setBrush(QBrush(scrollLight));
        painter.drawRoundedRect(rect.left() + 3, rect.top() + 8, axleWidth - 6, axleHeight - 6, 3, 3);

        // 右轴头
        painter.setBrush(QBrush(scrollColor));
        painter.drawRoundedRect(rect.right() - axleWidth, rect.top() + 5, axleWidth, axleHeight, 5, 5);
        painter.setBrush(QBrush(scrollLight));
        painter.drawRoundedRect(rect.right() - axleWidth + 3, rect.top() + 8, axleWidth - 6, axleHeight - 6, 3, 3);

        // 中间卷轴区域
        QLinearGradient gradient(rect.left() + axleWidth, 0, rect.right() - axleWidth, 0);
        gradient.setColorAt(0, QColor(245, 240, 230));
        gradient.setColorAt(0.5, QColor(255, 250, 240));
        gradient.setColorAt(1, QColor(245, 240, 230));
        painter.setBrush(QBrush(gradient));
        painter.drawRoundedRect(rect.left() + axleWidth, rect.top() + 10,
            rect.width() - 2 * axleWidth, rect.height() - 20, 8, 8);

        // 绘制装饰线
        painter.setPen(QPen(QColor(210, 180, 140), 1));
        painter.drawLine(rect.left() + axleWidth + 10, rect.top() + 15,
            rect.right() - axleWidth - 10, rect.top() + 15);
        painter.drawLine(rect.left() + axleWidth + 10, rect.bottom() - 15,
            rect.right() - axleWidth - 10, rect.bottom() - 15);

        // 调用父类绘制文字
        QLabel::paintEvent(event);
    }
};