#include "ImageryWindow.h"
#include "PoemManager.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

ImageryWindow::ImageryWindow(const Poem& currentPoem, QWidget* parent)
    : QWidget(parent), m_currentPoem(currentPoem), m_currentIndex(-1)
{
    setWindowTitle("意象关联分析");
    resize(1200, 700);

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
    setStyleSheet(
        "QWidget { background: #F7F4EF; font-size: 14px; }"
        "QLabel { color: #333333; }"
        "QPushButton { background: #8C5A3C; color: white; border: none; border-radius: 5px; padding: 6px 14px; }"
        "QPushButton:hover { background: #A86C47; }"
        "QTextEdit, QListWidget { background: white; color: #333333; border: 1px solid #D8C2B8; border-radius: 6px; }"
    );

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // ========== 左侧：当前诗歌（缩小） ==========
    QWidget* leftWidget = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    QLabel* currentTitle = new QLabel("当前诗歌");
    currentTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #4A3426;");

    titleLabel = new QLabel;
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A3426;");

    authorLabel = new QLabel;
    authorLabel->setStyleSheet("font-size: 14px; color: #6B5B4D;");

    contentText = new QTextEdit;
    contentText->setReadOnly(true);

    leftLayout->addWidget(currentTitle);
    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(authorLabel);
    leftLayout->addWidget(contentText, 1);

    // ========== 中间：意象类别与相关诗歌 ==========
    QWidget* middleWidget = new QWidget;
    QVBoxLayout* middleLayout = new QVBoxLayout(middleWidget);

    QLabel* categoryTitle = new QLabel("本诗包含的意象类别");
    categoryTitle->setStyleSheet("font-size: 16px; font-weight: bold;");
    categoryList = new QListWidget;

    QLabel* categoryResultTitle = new QLabel("包含所选意象类别的诗歌");
    categoryResultTitle->setStyleSheet("font-size: 16px; font-weight: bold;");
    categoryResultList = new QListWidget;

    similarButton = new QPushButton("意象最接近");

    QLabel* similarTitle = new QLabel("最接近的诗歌");
    similarTitle->setStyleSheet("font-size: 16px; font-weight: bold;");
    similarList = new QListWidget;

    middleLayout->addWidget(categoryTitle);
    middleLayout->addWidget(categoryList, 1);
    middleLayout->addWidget(categoryResultTitle);
    middleLayout->addWidget(categoryResultList, 1);
    middleLayout->addWidget(similarButton);
    middleLayout->addWidget(similarTitle);
    middleLayout->addWidget(similarList, 1);

    // ========== 右侧：相关诗歌内容预览 ==========
    QWidget* rightWidget = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    QLabel* previewAreaTitle = new QLabel("相关诗歌内容");
    previewAreaTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #4A3426;");

    previewTitleLabel = new QLabel("点击右侧诗歌名称后显示");
    previewTitleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A3426;");

    previewAuthorLabel = new QLabel("");
    previewAuthorLabel->setStyleSheet("font-size: 14px; color: #6B5B4D;");

    previewContentText = new QTextEdit;
    previewContentText->setReadOnly(true);
    previewContentText->setPlaceholderText("这里将显示你点击的相关诗歌内容");

    rightLayout->addWidget(previewAreaTitle);
    rightLayout->addWidget(previewTitleLabel);
    rightLayout->addWidget(previewAuthorLabel);
    rightLayout->addWidget(previewContentText, 1);

    // 三栏比例
    mainLayout->addWidget(leftWidget, 2);
    mainLayout->addWidget(middleWidget, 2);
    mainLayout->addWidget(rightWidget, 3);

    connect(categoryList, &QListWidget::itemClicked, this, &ImageryWindow::onCategoryClicked);
    connect(categoryResultList, &QListWidget::itemClicked, this, &ImageryWindow::onCategoryResultClicked);
    connect(similarList, &QListWidget::itemClicked, this, &ImageryWindow::onSimilarPoemClicked);
    connect(similarButton, &QPushButton::clicked, this, &ImageryWindow::showMostSimilarPoems);
}

void ImageryWindow::loadCurrentPoemInfo()
{
    titleLabel->setText("《" + m_currentPoem.title + "》");
    contentText->setText(m_currentPoem.content);
}

void ImageryWindow::loadCategoryList()
{
    categoryList->clear();

    QStringList categories = m_graph.getImageryCategoriesOfPoem(m_currentPoem);
    for (const QString& category : categories) {
        categoryList->addItem(category);
    }
}

void ImageryWindow::onCategoryClicked(QListWidgetItem* item)
{
    if (!item) return;

    QString category = item->text();
    categoryResultList->clear();

    QList<int> poemIndexes = m_graph.findPoemsByCategory(category, m_currentPoem.id);

    for (int index : poemIndexes) {
        GraphVertex v = m_graph.vertex(index);

        QListWidgetItem* poemItem = new QListWidgetItem(v.title);
        poemItem->setData(Qt::UserRole, v.poemId);
        categoryResultList->addItem(poemItem);
    }

    if (poemIndexes.isEmpty()) {
        categoryResultList->addItem("没有找到相关诗歌");
    }
}

void ImageryWindow::showMostSimilarPoems()
{
    similarList->clear();

    if (m_currentIndex == -1) {
        similarList->addItem("没有找到相关诗歌");
        return;
    }

    QList<GraphEdge> edges = m_graph.neighbors(m_currentIndex);

    for (const GraphEdge& edge : edges) {
        GraphVertex v = m_graph.vertex(edge.to);

        QListWidgetItem* poemItem = new QListWidgetItem(v.title);
        poemItem->setData(Qt::UserRole, v.poemId);
        similarList->addItem(poemItem);
    }

    if (edges.isEmpty()) {
        similarList->addItem("没有找到相关诗歌");
    }
}

void ImageryWindow::onCategoryResultClicked(QListWidgetItem* item)
{
    if (!item) return;

    QVariant data = item->data(Qt::UserRole);
    if (!data.isValid()) return;

    int poemId = data.toInt();
    displayPoemPreviewById(poemId);
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
            previewTitleLabel->setText("《" + poem.title + "》");
            previewContentText->setText(poem.content);
            return;
        }
    }

    previewTitleLabel->setText("未找到诗歌");
    previewAuthorLabel->clear();
    previewContentText->clear();
}