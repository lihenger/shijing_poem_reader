#include "WordCloudPage.h"
#include "WordCloudWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>

WordCloudPage::WordCloudPage(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // 顶部栏
    QHBoxLayout* topLayout = new QHBoxLayout();
    m_btnBack = new QPushButton("← 返回");
    m_btnBack->setFixedSize(100, 40);
    m_btnBack->setStyleSheet("QPushButton { background: #B8860B; color: white; border-radius: 20px; }");
    topLayout->addWidget(m_btnBack);
    topLayout->addStretch();

    QLabel* titleLabel = new QLabel("词云图 - 基于阅读历史");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #8B4513; font-family: '楷体';");
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // 词云控件
    m_wordCloudWidget = new WordCloudWidget(this);
    m_wordCloudWidget->setMinimumHeight(500);
    mainLayout->addWidget(m_wordCloudWidget, 1);

    // 信息栏
    m_infoLabel = new QLabel();
    m_infoLabel->setStyleSheet("color: #A0927B; font-family: '楷体';");
    m_infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_infoLabel);

    // 底部按钮栏
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    m_btnSave = new QPushButton("💾 保存图片");
    m_btnZoomIn = new QPushButton("🔍 放大");
    m_btnZoomOut = new QPushButton("🔍 缩小");
    m_btnReset = new QPushButton("⟳ 重置");

    QList<QPushButton*> btns = { m_btnSave, m_btnZoomIn, m_btnZoomOut, m_btnReset };
    for (auto btn : btns) {
        btn->setFixedSize(120, 40);
        btn->setStyleSheet("QPushButton { background: #B8860B; color: white; border-radius: 20px; }"
            "QPushButton:hover { background: #CD853F; }");
        bottomLayout->addWidget(btn);
    }
    bottomLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    // 连接信号
    connect(m_btnBack, &QPushButton::clicked, this, &WordCloudPage::onBackClicked);
    connect(m_btnSave, &QPushButton::clicked, this, &WordCloudPage::onSaveClicked);
    connect(m_btnZoomIn, &QPushButton::clicked, this, &WordCloudPage::onZoomInClicked);
    connect(m_btnZoomOut, &QPushButton::clicked, this, &WordCloudPage::onZoomOutClicked);
    connect(m_btnReset, &QPushButton::clicked, this, &WordCloudPage::onResetClicked);
}

void WordCloudPage::setWordFrequency(const QMap<QString, int>& frequency,
    int totalPoems, int totalWords)
{
    if (m_wordCloudWidget) {
        m_wordCloudWidget->setWordFrequency(frequency);
    }
    if (m_infoLabel) {
        m_infoLabel->setText(QString("共阅读 %1 首诗，涉及 %2 个意象词").arg(totalPoems).arg(totalWords));
    }
}

void WordCloudPage::onBackClicked()
{
    emit backClicked();
}

void WordCloudPage::onSaveClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        "保存词云图",
        QString("词云图_%1.png").arg(QDate::currentDate().toString("yyyyMMdd")),
        "PNG图片 (*.png)");

    if (!filePath.isEmpty() && m_wordCloudWidget) {
        m_wordCloudWidget->saveToImage(filePath);
        QMessageBox::information(this, "保存成功", QString("图片已保存到：\n%1").arg(filePath));
    }
}

void WordCloudPage::onZoomInClicked()
{
    if (m_wordCloudWidget) {
        m_wordCloudWidget->zoomIn();
    }
}

void WordCloudPage::onZoomOutClicked()
{
    if (m_wordCloudWidget) {
        m_wordCloudWidget->zoomOut();
    }
}

void WordCloudPage::onResetClicked()
{
    if (m_wordCloudWidget) {
        m_wordCloudWidget->resetView();
    }
}