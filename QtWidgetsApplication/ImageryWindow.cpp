// ImageryWindow.cpp
#include "ImageryWindow.h"
#include "PoemManager.h"
#include "ChineseStyle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ImageryWindow::ImageryWindow(const Poem& currentPoem, QWidget* parent)
    : QWidget(parent), m_currentPoem(currentPoem), m_currentIndex(-1)
{
    setWindowTitle("意象关联分析");
    resize(1300, 800);

    buildGraphData();
    initUI();
    loadCurrentPoemInfo();
    loadCategoryList();
}

void ImageryWindow::buildGraphData()
{
    QList<Poem> poems = PoemManager::instance().getAllPoems();
    m_graph.build(poems);
    m_currentIndex = m_graph.indexOfPoem(m_currentPoem.id);
}

void ImageryWindow::initUI()
{
    // 应用国风样式
    this->setStyleSheet(ChineseStyle::getMainStyle());

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // 左侧：当前诗歌
    QWidget* leftCard = new QWidget;
    leftCard->setStyleSheet("background: #FFF8F0; border: 1px solid #D2B48C; border-radius: 15px;");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftCard);

    QLabel* currentTitle = new QLabel("· 今诗 ·");
    currentTitle->setStyleSheet("font-family: '楷体', 'KaiTi'; font-size: 18px; color: #8B4513;");
    currentTitle->setAlignment(Qt::AlignCenter);

    m_titleLabel = new QLabel;
    m_titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #8B4513; font-family: '楷体', 'KaiTi';");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_authorLabel = new QLabel;
    m_authorLabel->setStyleSheet("font-size: 14px; color: #A0522D; font-family: '楷体', 'KaiTi';");
    m_authorLabel->setAlignment(Qt::AlignCenter);

    m_contentText = new QTextEdit;
    m_contentText->setReadOnly(true);

    leftLayout->addWidget(currentTitle);
    leftLayout->addWidget(m_titleLabel);
    leftLayout->addWidget(m_authorLabel);
    leftLayout->addWidget(m_contentText);

    // 中间：意象分析区
    QWidget* middleCard = new QWidget;
    middleCard->setStyleSheet("background: #FFF8F0; border: 1px solid #D2B48C; border-radius: 15px;");
    QVBoxLayout* middleLayout = new QVBoxLayout(middleCard);

    QLabel* categoryTitle = new QLabel("· 意象类别 ·");
    categoryTitle->setStyleSheet("font-family: '楷体', 'KaiTi'; font-size: 18px; color: #8B4513;");
    categoryTitle->setAlignment(Qt::AlignCenter);

    m_categoryList = new QListWidget;

    m_similarButton = new QPushButton("🎋 寻找意象相近的诗");
    m_similarButton->setStyleSheet("QPushButton { background: #B8860B; } QPushButton:hover { background: #CD853F; }");

    QLabel* similarTitle = new QLabel("· 相近之诗 ·");
    similarTitle->setStyleSheet("font-family: '楷体', 'KaiTi'; font-size: 18px; color: #8B4513; margin-top: 15px;");
    similarTitle->setAlignment(Qt::AlignCenter);

    m_similarList = new QListWidget;

    middleLayout->addWidget(categoryTitle);
    middleLayout->addWidget(m_categoryList);
    middleLayout->addWidget(m_similarButton);
    middleLayout->addWidget(similarTitle);
    middleLayout->addWidget(m_similarList);

    // 右侧：预览区
    QWidget* rightCard = new QWidget;
    rightCard->setStyleSheet("background: #FFF8F0; border: 1px solid #D2B48C; border-radius: 15px;");
    QVBoxLayout* rightLayout = new QVBoxLayout(rightCard);

    QLabel* previewTitle = new QLabel("· 诗卷预览 ·");
    previewTitle->setStyleSheet("font-family: '楷体', 'KaiTi'; font-size: 18px; color: #8B4513;");
    previewTitle->setAlignment(Qt::AlignCenter);

    m_previewTitleLabel = new QLabel;
    m_previewTitleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #8B4513; font-family: '楷体', 'KaiTi';");
    m_previewTitleLabel->setAlignment(Qt::AlignCenter);
    m_previewTitleLabel->setText("点击右侧诗歌名称后显示");

    m_previewContentText = new QTextEdit;
    m_previewContentText->setReadOnly(true);
    m_previewContentText->setPlaceholderText("这里将显示你点击的相关诗歌内容");

    rightLayout->addWidget(previewTitle);
    rightLayout->addWidget(m_previewTitleLabel);
    rightLayout->addWidget(m_previewContentText);

    mainLayout->addWidget(leftCard, 2);
    mainLayout->addWidget(middleCard, 2);
    mainLayout->addWidget(rightCard, 3);

    // 连接信号
    connect(m_categoryList, &QListWidget::itemClicked, this, &ImageryWindow::onCategoryClicked);
    connect(m_similarList, &QListWidget::itemClicked, this, &ImageryWindow::onSimilarPoemClicked);
    connect(m_similarButton, &QPushButton::clicked, this, &ImageryWindow::showMostSimilarPoems);
}

void ImageryWindow::loadCurrentPoemInfo()
{
    m_titleLabel->setText("《" + m_currentPoem.title + "》");
    m_authorLabel->setText(m_currentPoem.chapter + " · " + m_currentPoem.section);
    m_contentText->setText(m_currentPoem.content);
}

void ImageryWindow::loadCategoryList()
{
    m_categoryList->clear();

    QStringList categories = m_graph.getImageryCategoriesOfPoem(m_currentPoem);
    for (const QString& category : categories) {
        m_categoryList->addItem(category);
    }
}

void ImageryWindow::onCategoryClicked(QListWidgetItem* item)
{
    if (!item) return;

    QString category = item->text();

    QList<int> poemIndexes = m_graph.findPoemsByCategory(category, m_currentPoem.id);

    m_similarList->clear();

    for (int index : poemIndexes) {
        GraphVertex v = m_graph.vertex(index);

        QListWidgetItem* poemItem = new QListWidgetItem(v.title);
        poemItem->setData(Qt::UserRole, v.poemId);
        m_similarList->addItem(poemItem);
    }

    if (poemIndexes.isEmpty()) {
        m_similarList->addItem("没有找到相关诗歌");
    }
}

void ImageryWindow::showMostSimilarPoems()
{
    m_similarList->clear();

    if (m_currentIndex == -1) {
        m_similarList->addItem("没有找到相关诗歌");
        return;
    }

    QList<GraphEdge> edges = m_graph.neighbors(m_currentIndex);

    for (const GraphEdge& edge : edges) {
        GraphVertex v = m_graph.vertex(edge.to);

        QListWidgetItem* poemItem = new QListWidgetItem(v.title);
        poemItem->setData(Qt::UserRole, v.poemId);
        m_similarList->addItem(poemItem);
    }

    if (edges.isEmpty()) {
        m_similarList->addItem("没有找到相关诗歌");
    }
}

void ImageryWindow::onSimilarPoemClicked(QListWidgetItem* item)
{
    if (!item) return;

    QVariant data = item->data(Qt::UserRole);
    if (!data.isValid()) return;

    int poemId = data.toInt();
    displayPoemPreviewById(poemId);
}

void ImageryWindow::displayPoemPreviewById(int poemId)
{
    QList<Poem> poems = PoemManager::instance().getAllPoems();

    for (const Poem& poem : poems) {
        if (poem.id == poemId) {
            m_previewTitleLabel->setText("《" + poem.title + "》");
            m_previewContentText->setText(poem.content);
            return;
        }
    }

    m_previewTitleLabel->setText("未找到诗歌");
    m_previewContentText->clear();
}