#include "OtherPage.h"
#include "WordCloudPage.h"
#include "StatisticsData.h"
#include "MainWindow.h"
#include "PoemManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

OtherPage::OtherPage(QWidget* parent)
    : QWidget(parent)
    , m_statistics(nullptr)
    , m_mainWindow(nullptr)
{
    m_statistics = new StatisticsData();
    initUI();
}

OtherPage::~OtherPage()
{
    if (m_statistics) {
        delete m_statistics;
        m_statistics = nullptr;
    }
}

void OtherPage::setMainWindow(MainWindow* mainWindow)
{
    m_mainWindow = mainWindow;
    refreshData();
}

void OtherPage::initUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // 标题
    QLabel* titleLabel = new QLabel("其他功能");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; font-family: '楷体';");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 堆叠页面
    m_stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(m_stackedWidget);

    // ========== 卡片页面 ==========
    m_cardPage = new QWidget();
    QVBoxLayout* cardLayout = new QVBoxLayout(m_cardPage);
    cardLayout->setContentsMargins(0, 20, 0, 20);
    cardLayout->setSpacing(30);

    // 卡片容器 - 水平居中
    QWidget* cardsWidget = new QWidget();
    QHBoxLayout* cardsLayout = new QHBoxLayout(cardsWidget);
    cardsLayout->setSpacing(40);
    cardsLayout->setContentsMargins(50, 30, 50, 30);

    // 卡片：词云图
    QPushButton* cloudCard = new QPushButton();
    cloudCard->setObjectName("cloudCard");
    cloudCard->setFixedSize(320, 280);
    cloudCard->setCursor(Qt::PointingHandCursor);
    cloudCard->setStyleSheet(R"(
        QPushButton#cloudCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FFF8F0, stop:1 #F5EDE0);
            border: 2px solid #D2B48C;
            border-radius: 20px;
        }
        QPushButton#cloudCard:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FFF0E0, stop:1 #EDE0D0);
            border-color: #B8860B;
        }
    )");

    QVBoxLayout* cloudCardLayout = new QVBoxLayout(cloudCard);
    cloudCardLayout->setContentsMargins(20, 30, 20, 30);
    cloudCardLayout->setSpacing(15);

    QLabel* cloudIcon = new QLabel("☁️");
    cloudIcon->setStyleSheet("font-size: 80px;");
    cloudIcon->setAlignment(Qt::AlignCenter);
    QLabel* cloudTitle = new QLabel("词云图");
    cloudTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; font-family: '楷体';");
    cloudTitle->setAlignment(Qt::AlignCenter);
    QLabel* cloudDesc = new QLabel("基于阅读历史\n生成意象词云");
    cloudDesc->setStyleSheet("font-size: 14px; color: #A0927B; font-family: '楷体';");
    cloudDesc->setAlignment(Qt::AlignCenter);
    cloudDesc->setWordWrap(true);

    cloudCardLayout->addWidget(cloudIcon);
    cloudCardLayout->addWidget(cloudTitle);
    cloudCardLayout->addWidget(cloudDesc);
    cloudCardLayout->addStretch();

    cardsLayout->addStretch();
    cardsLayout->addWidget(cloudCard);
    cardsLayout->addStretch();

    cardLayout->addWidget(cardsWidget);
    cardLayout->addStretch();

    m_stackedWidget->addWidget(m_cardPage);

    // ========== 词云图页面 ==========
    m_wordCloudPage = new WordCloudPage();
    m_stackedWidget->addWidget(m_wordCloudPage);

    // 默认显示卡片页面
    m_stackedWidget->setCurrentWidget(m_cardPage);

    // 连接信号
    connect(cloudCard, &QPushButton::clicked, this, &OtherPage::onWordCloudClicked);
    connect(m_wordCloudPage, &WordCloudPage::backClicked, this, &OtherPage::onBackToCards);
}

void OtherPage::refreshData()
{
    updateStatistics();
}

void OtherPage::updateStatistics()
{
    if (!m_mainWindow) {
        return;
    }

    QStack<int> historyStack = m_mainWindow->getHistoryStack();
    QList<int> historyList;
    QStack<int> temp = historyStack;
    while (!temp.isEmpty()) {
        historyList.prepend(temp.pop());
    }

    if (historyList.isEmpty()) {
        return;
    }

    if (m_statistics) {
        m_statistics->calculateWordFrequency(historyList);
    }
}

void OtherPage::onWordCloudClicked()
{
    qDebug() << "onWordCloudClicked called";

    if (!m_mainWindow) {
        QMessageBox::warning(this, "错误", "无法获取主窗口");
        return;
    }

    QStack<int> historyStack = m_mainWindow->getHistoryStack();
    QList<int> historyList;
    QStack<int> temp = historyStack;
    while (!temp.isEmpty()) {
        historyList.prepend(temp.pop());
    }

    if (historyList.isEmpty()) {
        QMessageBox::information(this, "提示", "暂无阅读历史，请先阅读一些诗歌");
        return;
    }

    if (!m_statistics) {
        QMessageBox::warning(this, "错误", "统计数据对象为空");
        return;
    }

    m_statistics->calculateWordFrequency(historyList);

    if (m_wordCloudPage) {
        m_wordCloudPage->setWordFrequency(
            m_statistics->getWordFrequency(),
            m_statistics->getTotalPoems(),
            m_statistics->getTotalImageryWords()
        );
        m_stackedWidget->setCurrentWidget(m_wordCloudPage);
    }
    else {
        QMessageBox::warning(this, "错误", "词云图页面对象为空");
    }
}

void OtherPage::onBackToCards()
{
    refreshData();
    if (m_stackedWidget) {
        m_stackedWidget->setCurrentWidget(m_cardPage);
    }
}