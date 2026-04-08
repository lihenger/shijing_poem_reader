//#include "GraphView.h"
//#include <QPainter>
//
//GraphView::GraphView(QWidget* parent)
//    : QGraphicsView(parent)
//{
//    setRenderHint(QPainter::Antialiasing, true);
//    setRenderHint(QPainter::TextAntialiasing, true);
//
//    setDragMode(QGraphicsView::ScrollHandDrag);
//    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
//    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
//
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//
//    setStyleSheet("background:white;");
//}
//
//void GraphView::wheelEvent(QWheelEvent* event)
//{
//    const double scaleFactor = 1.15;
//
//    if (event->angleDelta().y() > 0) {
//        scale(scaleFactor, scaleFactor);
//    }
//    else {
//        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
//    }
//}