#include "OtherPage.h"
#include <QLabel>
#include <QVBoxLayout>

OtherPage::OtherPage(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("更多功能开发中...");
    label->setStyleSheet("font-size: 20px; color: #A0927B; font-family: '楷体';");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}