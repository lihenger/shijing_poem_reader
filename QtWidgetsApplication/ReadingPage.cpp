#include "ReadingPage.h"
#include "ImageryGraph.h"
#include "PoemManager.h"
#include "TranslationManager.h"
#include "ChineseStyle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>

ReadingPage::ReadingPage(QWidget* parent)
    : QWidget(parent)
    , m_currentIndex(-1)
{
    m_imageryGraph = new ImageryGraph();
    const auto& poems = PoemManager::instance().getAllPoems();
    m_imageryGraph->build(poems);

    setStyleSheet(ChineseStyle::getMainStyle());

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // 顶部：返回按钮
    QHBoxLayout* topLayout = new QHBoxLayout();
    m_btnReturn = new QPushButton("← 返回");
    m_btnReturn->setFixedSize(100, 40);
    m_btnReturn->setStyleSheet("QPushButton { background: #B8860B; color: white; border-radius: 20px; }");
    topLayout->addWidget(m_btnReturn);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // 中间三栏
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    // 左侧：阅读列表（宽度 150px）
    m_readingListWidget = new QListWidget();
    m_readingListWidget->setFixedWidth(150);
    m_readingListWidget->setStyleSheet("QListWidget { background: #FFF8F0; border: 1px solid #D2B48C; border-radius: 10px; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #E8DCC8; }"
        "QListWidget::item:selected { background: #D2B48C; color: white; }");
    contentLayout->addWidget(m_readingListWidget);

    // 中间：诗歌内容 + 翻译
    QWidget* centerWidget = new QWidget();
    QVBoxLayout* centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setSpacing(15);

    // ========== 修改1：标题字体从 28px 改为 22px ==========
    m_poemTitle = new QLabel();
    m_poemTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #8B4513; font-family: '楷体';");
    m_poemTitle->setAlignment(Qt::AlignCenter);

    // ========== 修改2：章节字体从 14px 改为 12px ==========
    m_poemChapter = new QLabel();
    m_poemChapter->setStyleSheet("font-size: 12px; color: #A0522D;");
    m_poemChapter->setAlignment(Qt::AlignCenter);

    // 原文区域
    QLabel* originalLabel = new QLabel("【原文】");
    originalLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #8B4513; margin-top: 10px;");

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    // 外层 ScrollArea 的滚动条也隐藏
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_poemContent = new QTextEdit();
    m_poemContent->setReadOnly(true);

    // ========== 修改3：隐藏 QTextEdit 内部的滚动条 ==========
    m_poemContent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_poemContent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // ========== 修改4：减少内边距，增加内容区域 ==========
    m_poemContent->setStyleSheet(
        "QTextEdit { background: #FFFEF5; border: 1px solid #D2B48C; "
        "border-radius: 15px; padding: 20px 30px; }"
    );
    scrollArea->setWidget(m_poemContent);

    // 翻译区域
    QLabel* translationLabel = new QLabel("【现代文翻译】");
    translationLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #8B4513; margin-top: 15px;");

    m_translationContent = new QTextEdit();
    m_translationContent->setReadOnly(true);
    m_translationContent->setStyleSheet("QTextEdit { background: #F5F0E8; border: 1px solid #D2B48C; border-radius: 15px; padding: 20px; font-size: 16px; line-height: 1.8; color: #3D2B1F; }");
    m_translationContent->setMaximumHeight(200);

    centerLayout->addWidget(m_poemTitle);
    centerLayout->addWidget(m_poemChapter);
    centerLayout->addWidget(originalLabel);
    centerLayout->addWidget(scrollArea);
    centerLayout->addWidget(translationLabel);
    centerLayout->addWidget(m_translationContent);

    contentLayout->addWidget(centerWidget, 2);

    // 右侧：意象 + 猜你喜欢（宽度 150px）
    QWidget* rightWidget = new QWidget();
    rightWidget->setFixedWidth(150);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    QLabel* imageryTitle = new QLabel("意象标签");
    imageryTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #8B4513; margin-bottom: 10px;");
    m_imageryList = new QListWidget();
    m_imageryList->setMaximumHeight(150);
    m_imageryList->setStyleSheet("QListWidget { background: #FFF8F0; border: 1px solid #D2B48C; border-radius: 10px; }");

    QLabel* recommendTitle = new QLabel("猜你喜欢");
    recommendTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #8B4513; margin-top: 20px; margin-bottom: 10px;");
    m_recommendList = new QListWidget();
    m_recommendList->setStyleSheet("QListWidget { background: #FFF8F0; border: 1px solid #D2B48C; border-radius: 10px; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #E8DCC8; }"
        "QListWidget::item:hover { background: #F5DEB3; }");

    rightLayout->addWidget(imageryTitle);
    rightLayout->addWidget(m_imageryList);
    rightLayout->addWidget(recommendTitle);
    rightLayout->addWidget(m_recommendList);
    rightLayout->addStretch();
    contentLayout->addWidget(rightWidget);

    mainLayout->addLayout(contentLayout, 1);

    // 底部按钮栏
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(20);
    bottomLayout->addStretch();

    m_btnPrev = new QPushButton("◀ 上一首");
    m_btnNext = new QPushButton("下一首 ▶");
    m_btnAddToRecite = new QPushButton("📖 加入背诵");
    m_btnRemoveFromRecite = new QPushButton("❌ 取消背诵");

    QList<QPushButton*> btns = { m_btnPrev, m_btnNext, m_btnAddToRecite, m_btnRemoveFromRecite };
    for (auto btn : btns) {
        btn->setFixedSize(120, 45);
        btn->setStyleSheet("QPushButton { background: #B8860B; color: white; border-radius: 22px; font-size: 14px; }"
            "QPushButton:hover { background: #CD853F; }");
        bottomLayout->addWidget(btn);
    }
    bottomLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    // 连接信号
    connect(m_btnReturn, &QPushButton::clicked, this, &ReadingPage::exitReadingPage);
    connect(m_btnPrev, &QPushButton::clicked, this, &ReadingPage::onPrevClicked);
    connect(m_btnNext, &QPushButton::clicked, this, &ReadingPage::onNextClicked);
    connect(m_btnAddToRecite, &QPushButton::clicked, this, &ReadingPage::onAddToReciteClicked);
    connect(m_btnRemoveFromRecite, &QPushButton::clicked, this, &ReadingPage::onRemoveFromReciteClicked);
    connect(m_recommendList, &QListWidget::itemClicked, this, &ReadingPage::onRecommendClicked);
    connect(m_readingListWidget, &QListWidget::itemClicked, this, &ReadingPage::onReadingListItemClicked);
}

void ReadingPage::loadTranslation(int poemId)
{
    TranslationManager& transManager = TranslationManager::instance();

    if (transManager.hasTranslation(poemId)) {
        QString translation = transManager.getTranslation(poemId);
        QString appreciation = transManager.getAppreciation(poemId);

        QString content = translation;
        if (!appreciation.isEmpty()) {
            content += "\n\n【赏析】\n" + appreciation;
        }
        m_translationContent->setText(content);
    }
    else {
        m_translationContent->setText("暂无翻译，敬请期待...");
    }
}

void ReadingPage::setReadingList(const QList<int>& readingList, int startIndex)
{
    m_readingList = readingList;
    m_currentIndex = startIndex;

    m_readingListWidget->clear();
    const auto& poems = PoemManager::instance().getAllPoems();
    for (int idx : m_readingList) {
        if (idx >= 0 && idx < poems.size()) {
            QListWidgetItem* item = new QListWidgetItem(poems[idx].title);
            item->setData(Qt::UserRole, idx);
            m_readingListWidget->addItem(item);
        }
    }

    loadPoem(m_currentIndex);
    highlightCurrentItem();
}

void ReadingPage::loadPoem(int index)
{
    if (index < 0 || index >= m_readingList.size()) return;

    m_currentIndex = index;
    int poemIdx = m_readingList[index];
    const auto& poems = PoemManager::instance().getAllPoems();

    if (poemIdx >= 0 && poemIdx < poems.size()) {
        const Poem& poem = poems[poemIdx];
        m_poemTitle->setText(poem.title);
        m_poemChapter->setText(poem.chapter + " · " + poem.section);

        // 将换行符替换为 <br> 标签
        QString content = poem.content;
        content.replace("\n", "<br>");

        // ========== 修改5：字体 22px，行距 2.0 ==========
        QString styledContent = QString(
            "<div style='text-align: center; font-family: \"楷体\", \"KaiTi\";'>"
            "<p style='font-size: 22px; line-height: 2.0; text-indent: 2em; margin: 0 0 10px 0;'>%1</p>"
            "</div>"
        ).arg(content);

        m_poemContent->setHtml(styledContent);

        loadTranslation(poem.id);

        emit addToHistory(poemIdx);
    }

    updateImageryAndRecommend();
    highlightCurrentItem();
}

void ReadingPage::updateImageryAndRecommend()
{
    if (m_currentIndex < 0 || m_currentIndex >= m_readingList.size()) return;

    int poemIdx = m_readingList[m_currentIndex];
    const auto& poems = PoemManager::instance().getAllPoems();
    const Poem& currentPoem = poems[poemIdx];

    m_imageryList->clear();
    QStringList categories = m_imageryGraph->getImageryCategoriesOfPoem(currentPoem);
    for (const QString& category : categories) {
        m_imageryList->addItem(category);
    }
    if (categories.isEmpty()) {
        m_imageryList->addItem("无意象标签");
    }

    m_recommendList->clear();
    int graphIndex = m_imageryGraph->indexOfPoem(currentPoem.id);
    if (graphIndex != -1) {
        QList<GraphEdge> neighbors = m_imageryGraph->neighbors(graphIndex);
        for (const GraphEdge& edge : neighbors) {
            GraphVertex v = m_imageryGraph->vertex(edge.to);
            QListWidgetItem* item = new QListWidgetItem(v.title);
            item->setData(Qt::UserRole, v.poemId);
            m_recommendList->addItem(item);
        }
    }
    if (m_recommendList->count() == 0) {
        m_recommendList->addItem("暂无推荐");
    }
}

void ReadingPage::highlightCurrentItem()
{
    for (int i = 0; i < m_readingListWidget->count(); ++i) {
        QListWidgetItem* item = m_readingListWidget->item(i);
        if (item->data(Qt::UserRole).toInt() == m_readingList[m_currentIndex]) {
            m_readingListWidget->setCurrentItem(item);
            break;
        }
    }
}

void ReadingPage::onReturnClicked()
{
    emit exitReadingPage();
}

void ReadingPage::onPrevClicked()
{
    if (m_currentIndex > 0) {
        loadPoem(m_currentIndex - 1);
    }
}

void ReadingPage::onNextClicked()
{
    if (m_currentIndex < m_readingList.size() - 1) {
        loadPoem(m_currentIndex + 1);
    }
}

void ReadingPage::onAddToReciteClicked()
{
    int poemIdx = m_readingList[m_currentIndex];
    emit addToRecite(poemIdx);
    QMessageBox::information(this, "提示", "已添加到背诵列表");
}

void ReadingPage::onRemoveFromReciteClicked()
{
    int poemIdx = m_readingList[m_currentIndex];
    emit removeFromRecite(poemIdx);
    QMessageBox::information(this, "提示", "已从背诵列表移除");
}

void ReadingPage::onRecommendClicked(QListWidgetItem* item)
{
    if (!item) return;
    int poemId = item->data(Qt::UserRole).toInt();

    const auto& poems = PoemManager::instance().getAllPoems();
    int targetIndex = -1;
    for (int i = 0; i < poems.size(); ++i) {
        if (poems[i].id == poemId) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) return;

    int listIndex = m_readingList.indexOf(targetIndex);
    if (listIndex != -1) {
        loadPoem(listIndex);
    }
    else {
        m_readingList.append(targetIndex);
        const Poem& poem = poems[targetIndex];
        QListWidgetItem* newItem = new QListWidgetItem(poem.title);
        newItem->setData(Qt::UserRole, targetIndex);
        m_readingListWidget->addItem(newItem);
        loadPoem(m_readingList.size() - 1);
    }
}

void ReadingPage::onReadingListItemClicked(QListWidgetItem* item)
{
    if (!item) return;
    int poemIdx = item->data(Qt::UserRole).toInt();
    int listIndex = m_readingList.indexOf(poemIdx);
    if (listIndex != -1) {
        loadPoem(listIndex);
    }
}