// InkDecor.h
#pragma once
#include <QWidget>
#include <QPainter>
#include <QPainterPath>

class InkDecor : public QWidget {
    Q_OBJECT

public:
    enum DecorType { Mountain, Cloud, PlumBlossom, Bamboo };

    explicit InkDecor(DecorType type = Mountain, QWidget* parent = nullptr)
        : QWidget(parent), m_type(type) {
        setAttribute(Qt::WA_TranslucentBackground);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 使用更淡的墨色，确保不影响文字可读性
        QColor inkColor(80, 60, 40, 30);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(inkColor));

        QPainterPath path;

        switch (m_type) {
        case Mountain:
            drawMountain(path);
            break;
        case Cloud:
            drawCloud(path);
            break;
        case PlumBlossom:
            drawPlumBlossom(path);
            break;
        case Bamboo:
            drawBamboo(path);
            break;
        }

        painter.drawPath(path);
    }

private:
    DecorType m_type;

    void drawMountain(QPainterPath& path) {
        int w = width();
        int h = height();

        path.moveTo(0, h);
        path.lineTo(w * 0.2, h * 0.5);
        path.lineTo(w * 0.4, h * 0.7);
        path.lineTo(w * 0.6, h * 0.3);
        path.lineTo(w * 0.8, h * 0.55);
        path.lineTo(w, h * 0.4);
        path.lineTo(w, h);
        path.closeSubpath();
    }

    void drawCloud(QPainterPath& path) {
        int w = width();
        int h = height();

        path.moveTo(0, h * 0.7);
        path.cubicTo(w * 0.2, h * 0.5, w * 0.3, h * 0.6, w * 0.5, h * 0.55);
        path.cubicTo(w * 0.7, h * 0.5, w * 0.8, h * 0.6, w, h * 0.55);
        path.lineTo(w, h);
        path.lineTo(0, h);
        path.closeSubpath();
    }

    void drawPlumBlossom(QPainterPath& path) {
        int centerX = width() / 2;
        int centerY = height() / 2;
        int radius = qMin(width(), height()) / 3;

        for (int i = 0; i < 5; i++) {
            double angle = i * 72 * 3.14159 / 180;
            int x = centerX + radius * cos(angle);
            int y = centerY + radius * sin(angle);
            path.addEllipse(x - 3, y - 3, 6, 6);
        }
        path.addEllipse(centerX - 4, centerY - 4, 8, 8);
    }

    void drawBamboo(QPainterPath& path) {
        int w = width();
        int h = height();

        // 竹竿
        path.addRect(w * 0.48, 0, w * 0.04, h);

        // 竹叶
        for (int i = 1; i <= 4; i++) {
            int y = h * i / 5;
            path.moveTo(w * 0.5, y);
            path.lineTo(w * 0.7, y - 10);
            path.lineTo(w * 0.6, y);
            path.lineTo(w * 0.7, y + 10);
            path.closeSubpath();

            path.moveTo(w * 0.5, y);
            path.lineTo(w * 0.3, y - 10);
            path.lineTo(w * 0.4, y);
            path.lineTo(w * 0.3, y + 10);
            path.closeSubpath();
        }
    }
};