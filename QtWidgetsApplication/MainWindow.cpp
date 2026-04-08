// MainWindow.cpp
#include "MainWindow.h"
#include "PoemManager.h"
#include "ImageryWindow.h"
#include "ChineseStyle.h"
#include "ScrollTitle.h"
#include "InkDecor.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QScrollArea>
#include <QFrame>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_categoryPoems(nullptr)
    , currentIndex(0)
    , isReadingQueue(false)
    , preQueueIndex(-1)
{
    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        m_categorySizes[i] = 0;
    }

    setWindowTitle("诗经 · 风雅颂");
    resize(1400, 900);

    initCategoryData();
    initUI();
    loadFirstPoem();
}

MainWindow::~MainWindow()
{
    releaseCategoryData();
}

void MainWindow::initCategoryData()
{
    const auto& poems = PoemManager::instance().getAllPoems();
    int totalPoems = poems.size();

    if (totalPoems == 0) return;

    m_categoryNames[CATEGORY_FENG] = "国风";
    m_categoryNames[CATEGORY_YA] = "雅";
    m_categoryNames[CATEGORY_SONG] = "颂";

    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        m_categorySizes[i] = 0;
    }

    for (int i = 0; i < totalPoems; ++i) {
        const Poem& poem = poems[i];
        QString chapter = poem.chapter;

        if (chapter.contains("风") || chapter == "风") {
            m_categorySizes[CATEGORY_FENG]++;
        }
        else if (chapter.contains("雅") || chapter == "雅" ||
            chapter.contains("小雅") || chapter.contains("大雅")) {
            m_categorySizes[CATEGORY_YA]++;
        }
        else if (chapter.contains("颂") || chapter == "颂") {
            m_categorySizes[CATEGORY_SONG]++;
        }
    }

    m_categoryPoems = new int* [CATEGORY_COUNT];
    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        m_categoryPoems[i] = new int[m_categorySizes[i]];
    }

    int currentIndices[CATEGORY_COUNT] = { 0, 0, 0 };

    for (int i = 0; i < totalPoems; ++i) {
        const Poem& poem = poems[i];
        QString chapter = poem.chapter;

        if (chapter.contains("风") || chapter == "风") {
            int pos = currentIndices[CATEGORY_FENG]++;
            m_categoryPoems[CATEGORY_FENG][pos] = i;
        }
        else if (chapter.contains("雅") || chapter == "雅" ||
            chapter.contains("小雅") || chapter.contains("大雅")) {
            int pos = currentIndices[CATEGORY_YA]++;
            m_categoryPoems[CATEGORY_YA][pos] = i;
        }
        else if (chapter.contains("颂") || chapter == "颂") {
            int pos = currentIndices[CATEGORY_SONG]++;
            m_categoryPoems[CATEGORY_SONG][pos] = i;
        }
    }
}

void MainWindow::releaseCategoryData()
{
    if (m_categoryPoems) {
        for (int i = 0; i < CATEGORY_COUNT; ++i) {
            if (m_categoryPoems[i]) {
                delete[] m_categoryPoems[i];
                m_categoryPoems[i] = nullptr;
            }
        }
        delete[] m_categoryPoems;
        m_categoryPoems = nullptr;
    }
}

QTreeWidget* MainWindow::createCategoryTree()
{
    QTreeWidget* treeWidget = new QTreeWidget();
    treeWidget->setHeaderLabel("诗三百");
    treeWidget->setIndentation(15);

    const auto& poems = PoemManager::instance().getAllPoems();

    QList<QTreeWidgetItem*> topItems;

    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        QTreeWidgetItem* categoryItem = new QTreeWidgetItem();
        categoryItem->setText(0, QString("%1 (%2首)").arg(m_categoryNames[i]).arg(m_categorySizes[i]));
        categoryItem->setData(0, Qt::UserRole, -1);

        if (i == CATEGORY_YA) {
            QTreeWidgetItem* xiaoyaItem = new QTreeWidgetItem();
            xiaoyaItem->setText(0, "小雅");
            xiaoyaItem->setData(0, Qt::UserRole, -2);

            QTreeWidgetItem* dayaItem = new QTreeWidgetItem();
            dayaItem->setText(0, "大雅");
            dayaItem->setData(0, Qt::UserRole, -2);

            int xiaoyaCount = 0;
            int dayaCount = 0;

            for (int j = 0; j < m_categorySizes[i]; ++j) {
                int idx = m_categoryPoems[i][j];
                const Poem& poem = poems[idx];
                QTreeWidgetItem* poemItem = new QTreeWidgetItem();
                poemItem->setText(0, poem.title);
                poemItem->setData(0, Qt::UserRole, idx);

                if (poem.section.contains("小雅")) {
                    xiaoyaItem->addChild(poemItem);
                    xiaoyaCount++;
                }
                else if (poem.section.contains("大雅")) {
                    dayaItem->addChild(poemItem);
                    dayaCount++;
                }
            }

            xiaoyaItem->setText(0, QString("小雅 (%1首)").arg(xiaoyaCount));
            dayaItem->setText(0, QString("大雅 (%1首)").arg(dayaCount));

            categoryItem->addChild(xiaoyaItem);
            categoryItem->addChild(dayaItem);
        }
        else {
            for (int j = 0; j < m_categorySizes[i]; ++j) {
                int idx = m_categoryPoems[i][j];
                const Poem& poem = poems[idx];
                QTreeWidgetItem* poemItem = new QTreeWidgetItem();
                poemItem->setText(0, poem.title);
                poemItem->setData(0, Qt::UserRole, idx);
                categoryItem->addChild(poemItem);
            }
        }

        treeWidget->addTopLevelItem(categoryItem);
        topItems.append(categoryItem);
    }

    treeWidget->expandAll();
    return treeWidget;
}

void MainWindow::addToHistory(int index)
{
    if (!historyStack.isEmpty() && historyStack.top() == index) {
        return;
    }
    historyStack.push(index);
    if (historyStack.size() > 50) {
        QStack<int> temp;
        for (int i = 1; i < historyStack.size(); ++i) {
            temp.push(historyStack[i]);
        }
        historyStack = temp;
    }
}

void MainWindow::initUI()
{
    // 应用国风样式
    this->setStyleSheet(ChineseStyle::getMainStyle());

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(25, 20, 25, 20);
    mainLayout->setSpacing(25);

    // ========== 左侧：水墨侧边栏 ==========
    QWidget* leftWidget = new QWidget;
    leftWidget->setFixedWidth(280);
    leftWidget->setObjectName("leftSidebar");

    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(15);
    leftLayout->setContentsMargins(10, 20, 10, 20);

    // 顶部标题（卷轴样式）
    ScrollTitle* scrollTitle = new ScrollTitle("詩經");
    scrollTitle->setObjectName("scrollTitle");
    leftLayout->addWidget(scrollTitle);

    // 装饰：水墨山水（顶部）
    InkDecor* topDecor = new InkDecor(InkDecor::Mountain);
    topDecor->setFixedHeight(60);
    leftLayout->addWidget(topDecor);

    // 搜索区域
    QLabel* searchLabel = new QLabel("· 寻诗 ·");
    searchLabel->setStyleSheet("font-family: '楷体', 'KaiTi'; font-size: 14px; color: #8B4513; margin-top: 10px;");
    searchLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(searchLabel);

    m_searchEdit = new QLineEdit;
    m_searchEdit->setPlaceholderText("输入标题或诗句...");
    leftLayout->addWidget(m_searchEdit);

    // 功能按钮区域
    QWidget* btnWidget = new QWidget;
    QGridLayout* btnLayout = new QGridLayout(btnWidget);
    btnLayout->setSpacing(10);

    m_btnHome = new QPushButton("🏠 首页");
    m_btnHistory = new QPushButton("📜 历史");
    m_btnReadList = new QPushButton("📚 待读队列");

    btnLayout->addWidget(m_btnHome, 0, 0);
    btnLayout->addWidget(m_btnHistory, 0, 1);
    btnLayout->addWidget(m_btnReadList, 1, 0, 1, 2);

    leftLayout->addWidget(btnWidget);

    // 分隔线
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #D2B48C; max-height: 1px;");
    leftLayout->addWidget(line);

    // 分类导航
    QLabel* navLabel = new QLabel("· 分类浏览 ·");
    navLabel->setStyleSheet("font-family: '楷体', 'KaiTi'; font-size: 14px; color: #8B4513;");
    navLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(navLabel);

    m_categoryTree = createCategoryTree();
    m_categoryTree->setMaximumHeight(350);
    leftLayout->addWidget(m_categoryTree);

    // 底部装饰：墨竹
    InkDecor* bambooDecor = new InkDecor(InkDecor::Bamboo);
    bambooDecor->setFixedHeight(80);
    leftLayout->addWidget(bambooDecor);

    leftLayout->addStretch();

    // ========== 右侧：主阅读区 ==========
    QWidget* rightWidget = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(20);

    // 诗歌标题区域
    QWidget* titleWidget = new QWidget;
    titleWidget->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #F5F0E8, stop:0.2 #FFF8F0, stop:0.8 #FFF8F0, stop:1 #F5F0E8);");
    QVBoxLayout* titleLayout = new QVBoxLayout(titleWidget);

    m_poemTitle = new QLabel;
    m_poemTitle->setObjectName("poemTitle");
    m_poemTitle->setAlignment(Qt::AlignCenter);

    m_poemChapter = new QLabel;
    m_poemChapter->setObjectName("sectionTitle");
    m_poemChapter->setAlignment(Qt::AlignCenter);

    titleLayout->addWidget(m_poemTitle);
    titleLayout->addWidget(m_poemChapter);

    rightLayout->addWidget(titleWidget);

    // 诗歌内容
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

    m_poemContent = new QTextEdit;
    m_poemContent->setReadOnly(true);
    scrollArea->setWidget(m_poemContent);
    rightLayout->addWidget(scrollArea, 1);

    // 底部按钮区
    QWidget* btnBar = new QWidget;
    QHBoxLayout* btnBarLayout = new QHBoxLayout(btnBar);
    btnBarLayout->setSpacing(15);

    m_btnPrev = new QPushButton("◀ 上一篇");
    m_btnNext = new QPushButton("下一篇 ▶");
    m_btnAddToList = new QPushButton("📖 加入待读");
    m_btnJumpQueue = new QPushButton("⚡ 插队");
    m_btnImageryRelation = new QPushButton("🎋 意象分析");

    btnBarLayout->addWidget(m_btnPrev);
    btnBarLayout->addWidget(m_btnNext);
    btnBarLayout->addStretch();
    btnBarLayout->addWidget(m_btnAddToList);
    btnBarLayout->addWidget(m_btnJumpQueue);
    btnBarLayout->addWidget(m_btnImageryRelation);

    rightLayout->addWidget(btnBar);

    // 添加右侧装饰：水墨云纹
    InkDecor* cloudDecor = new InkDecor(InkDecor::Cloud);
    cloudDecor->setFixedHeight(40);
    rightLayout->addWidget(cloudDecor);

    mainLayout->addWidget(leftWidget, 1);
    mainLayout->addWidget(rightWidget, 3);

    // 连接信号
    connectSignals();
}

void MainWindow::connectSignals()
{
    connect(m_btnPrev, &QPushButton::clicked, this, &MainWindow::onPrevClicked);
    connect(m_btnNext, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(m_btnImageryRelation, &QPushButton::clicked, this, &MainWindow::onImageryRelationClicked);
    connect(m_btnAddToList, &QPushButton::clicked, this, &MainWindow::onAddToListClicked);
    connect(m_btnJumpQueue, &QPushButton::clicked, this, &MainWindow::onJumpQueueClicked);
    connect(m_btnReadList, &QPushButton::clicked, this, &MainWindow::onReadListClicked);
    connect(m_btnHome, &QPushButton::clicked, this, &MainWindow::onHomeClicked);
    connect(m_btnHistory, &QPushButton::clicked, this, &MainWindow::onHistoryClicked);
    connect(m_categoryTree, &QTreeWidget::itemClicked, this, &MainWindow::onCategoryItemClicked);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
}

void MainWindow::loadFirstPoem()
{
    const auto& poems = PoemManager::instance().getAllPoems();
    if (poems.isEmpty()) {
        m_poemTitle->setText("未加载到诗歌数据");
        m_poemChapter->clear();
        m_poemContent->setText("请检查 resource/诗经.json 是否存在。");
        return;
    }

    currentIndex = 0;
    addToHistory(currentIndex);
    showPoemByIndex(currentIndex);
}

void MainWindow::showPoemByIndex(int index)
{
    const auto& poems = PoemManager::instance().getAllPoems();
    if (index < 0 || index >= poems.size()) return;

    currentIndex = index;
    const Poem& poem = poems[index];

    m_poemTitle->setText(poem.title);
    m_poemChapter->setText(poem.chapter + " · " + poem.section);
    m_poemContent->setText(poem.content);
}

void MainWindow::onPrevClicked()
{
    if (isReadingQueue) {
        QMessageBox::information(this, "提示", "正在播放待读队列，请点击“下一篇”继续。");
        return;
    }
    if (currentIndex > 0) {
        addToHistory(currentIndex - 1);
        showPoemByIndex(currentIndex - 1);
    }
}

void MainWindow::onNextClicked()
{
    if (isReadingQueue) {
        if (!readQueue.isEmpty()) {
            int nextIndex = readQueue.takeFirst();
            addToHistory(nextIndex);
            showPoemByIndex(nextIndex);
        }
        else {
            isReadingQueue = false;
            showPoemByIndex(preQueueIndex);
            QMessageBox::information(this, "提示", "待读队列已播放完毕。");
        }
        return;
    }

    const auto& poems = PoemManager::instance().getAllPoems();
    if (currentIndex < poems.size() - 1) {
        addToHistory(currentIndex + 1);
        showPoemByIndex(currentIndex + 1);
    }
}

void MainWindow::onImageryRelationClicked()
{
    const auto& poems = PoemManager::instance().getAllPoems();
    if (currentIndex < 0 || currentIndex >= poems.size()) return;

    ImageryWindow* w = new ImageryWindow(poems[currentIndex]);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::onAddToListClicked()
{
    readQueue.append(currentIndex);
    QMessageBox::information(this, "提示", "当前诗歌已加入待读队列尾部。");
}

void MainWindow::onJumpQueueClicked()
{
    readQueue.prepend(currentIndex);
    QMessageBox::information(this, "提示", "当前诗歌已插队至待读队列头部。");
}

void MainWindow::onReadListClicked()
{
    if (readQueue.isEmpty()) {
        QMessageBox::information(this, "提示", "当前待读队列为空！请先添加诗歌。");
        return;
    }

    if (isReadingQueue) {
        QMessageBox::information(this, "提示", "当前已经在播放待读队列中。");
        return;
    }

    preQueueIndex = currentIndex;
    isReadingQueue = true;

    int firstIndex = readQueue.takeFirst();
    addToHistory(firstIndex);
    showPoemByIndex(firstIndex);
}

void MainWindow::onCategoryItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    if (!item) return;

    QVariant data = item->data(0, Qt::UserRole);
    int index = data.toInt();

    if (index >= 0) {
        addToHistory(index);
        showPoemByIndex(index);
    }
}

void MainWindow::onHistoryClicked()
{
    if (historyStack.isEmpty()) {
        QMessageBox::information(this, "提示", "暂无阅读历史记录。");
        return;
    }

    if (isReadingQueue) {
        QMessageBox::information(this, "提示", "请先退出待读队列播放模式。");
        return;
    }

    int lastIndex = historyStack.pop();

    QString historyText = "阅读历史记录:\n\n";
    QStack<int> tempStack = historyStack;
    QList<int> tempList;
    while (!tempStack.isEmpty()) {
        tempList.prepend(tempStack.pop());
    }
    tempList.append(lastIndex);

    int count = 0;
    const auto& poems = PoemManager::instance().getAllPoems();
    for (int i = tempList.size() - 1; i >= 0 && count < 10; --i, ++count) {
        if (tempList[i] >= 0 && tempList[i] < poems.size()) {
            historyText += QString("%1. %2\n").arg(count + 1).arg(poems[tempList[i]].title);
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("阅读历史");
    msgBox.setText(historyText);
    msgBox.setInformativeText("是否跳转到最近阅读的诗歌？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        historyStack.push(lastIndex);
        addToHistory(lastIndex);
        showPoemByIndex(lastIndex);
    }
    else {
        historyStack.push(lastIndex);
    }
}

void MainWindow::onHomeClicked()
{
    if (isReadingQueue) {
        QMessageBox::information(this, "提示", "请先退出待读队列播放模式。");
        return;
    }

    const auto& poems = PoemManager::instance().getAllPoems();
    if (!poems.isEmpty()) {
        addToHistory(0);
        showPoemByIndex(0);
        QMessageBox::information(this, "提示", "已返回首页。");
    }
}

void MainWindow::onSearch()
{
    QString text = m_searchEdit->text();
    if (text.trimmed().isEmpty()) {
        QMessageBox::information(this, "提示", "请输入搜索关键词。");
        return;
    }

    const auto& poems = PoemManager::instance().getAllPoems();
    QList<int> searchResults;

    for (int i = 0; i < poems.size(); ++i) {
        const Poem& poem = poems[i];
        if (poem.title.contains(text, Qt::CaseInsensitive) ||
            poem.content.contains(text, Qt::CaseInsensitive)) {
            searchResults.append(i);
        }
    }

    if (!searchResults.isEmpty()) {
        if (searchResults.size() == 1) {
            addToHistory(searchResults.first());
            showPoemByIndex(searchResults.first());
            QMessageBox::information(this, "搜索结果", QString("找到1首相关诗歌：%1").arg(poems[searchResults.first()].title));
        }
        else {
            QString resultText = QString("找到%1首相关诗歌：\n\n").arg(searchResults.size());
            for (int i = 0; i < qMin(10, searchResults.size()); ++i) {
                resultText += QString("%1. %2\n").arg(i + 1).arg(poems[searchResults[i]].title);
            }
            if (searchResults.size() > 10) {
                resultText += "...\n\n将跳转到第一首搜索结果。";
            }

            QMessageBox msgBox;
            msgBox.setWindowTitle("搜索结果");
            msgBox.setText(resultText);
            msgBox.setInformativeText("是否跳转到第一首搜索结果？");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

            if (msgBox.exec() == QMessageBox::Yes) {
                addToHistory(searchResults.first());
                showPoemByIndex(searchResults.first());
            }
        }
    }
    else {
        QMessageBox::information(this, "搜索结果", QString("未找到包含「%1」的诗歌。").arg(text));
    }
}