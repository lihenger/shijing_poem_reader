#include "MainWindow.h"
#include "PoemManager.h"
#include "ImageryWindow.h"

#include <QFont>
#include <QMessageBox> 
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), currentIndex(0), isReadingQueue(false), preQueueIndex(-1)
{
    // 初始化二维数组指针为nullptr
    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        m_categoryPoems = nullptr;
        m_categorySizes[i] = 0;
    }

    setWindowTitle("诗经阅读器");
    resize(1200, 800);
    
    setStyleSheet(R"(
        QMainWindow {
            background-color: #F9F6F0;
        }
        QPushButton {
            background-color: #EFEAE3;
            color: #3D352A;
            border: none;
            border-radius: 12px;
            padding: 10px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #A84E4E;
            color: white;
        }
        QLineEdit {
            border: 1px solid #D9C7B8;
            border-radius: 10px;
            padding: 8px;
            background-color: white;
         QTreeWidget {
            background-color: white;
            border: 1px solid #D9C7B8;
            border-radius: 8px;
            outline: none;
        }
        QTreeWidget::item {
            padding: 6px;
        }
        QTreeWidget::item:hover {
            background-color: #EFEAE3;
        }
        QTreeWidget::item:selected {
            background-color: #A84E4E;
            color: white;
        }
    )");
    initCategoryData();  // 初始化分类数据
    initUI();
    loadFirstPoem();
}

MainWindow::~MainWindow()
{
    releaseCategoryData();  // 释放二维数组内存
}

void MainWindow::initCategoryData()
{
    const auto& poems = PoemManager::instance().getAllPoems();
    int totalPoems = poems.size();

    if (totalPoems == 0) return;

    // 设置类别名称
    m_categoryNames[CATEGORY_FENG] = "国风";
    m_categoryNames[CATEGORY_YA] = "雅";
    m_categoryNames[CATEGORY_SONG] = "颂";

    // 1.统计每类诗歌的数量
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

    // 2.动态分配二维数组内存
    m_categoryPoems = new int* [CATEGORY_COUNT];
    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        m_categoryPoems[i] = new int[m_categorySizes[i]];
    }

    // 3.填充二维数组
    int currentIndices[CATEGORY_COUNT] = { 0, 0, 0 };  // 记录每类当前填充位置

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

    //// 调试输出
    //qDebug() << "=== 二维数组分类统计 ===";
    //qDebug() << "国风:" << m_categorySizes[CATEGORY_FENG] << "首";
    //qDebug() << "雅:" << m_categorySizes[CATEGORY_YA] << "首";
    //qDebug() << "颂:" << m_categorySizes[CATEGORY_SONG] << "首";

    //// 打印前几首作为验证
    //for (int i = 0; i < qMin(3, m_categorySizes[CATEGORY_FENG]); ++i) {
    //    int idx = m_categoryPoems[CATEGORY_FENG][i];
    //    qDebug() << "国风示例:" << poems[idx].title;
    //}
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
    treeWidget->setHeaderLabel("诗经分类");
    treeWidget->setIndentation(15);

    const auto& poems = PoemManager::instance().getAllPoems();

    // 创建三个顶级分类节点
    QList<QTreeWidgetItem*> topItems;

    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        QTreeWidgetItem* categoryItem = new QTreeWidgetItem();
        categoryItem->setText(0, QString("%1 (%2首)").arg(m_categoryNames[i]).arg(m_categorySizes[i]));
        categoryItem->setData(0, Qt::UserRole, -1);  // -1 表示这是分类节点

        // 如果是"雅"，需要再细分为小雅和大雅
        if (i == CATEGORY_YA) {  // 雅
            QTreeWidgetItem* xiaoyaItem = new QTreeWidgetItem();
            xiaoyaItem->setText(0, "小雅");
            xiaoyaItem->setData(0, Qt::UserRole, -2);

            QTreeWidgetItem* dayaItem = new QTreeWidgetItem();
            dayaItem->setText(0, "大雅");
            dayaItem->setData(0, Qt::UserRole, -2);

            int xiaoyaCount = 0;
            int dayaCount = 0;

            // 遍历二维数组中雅的诗歌
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
            // 国风和颂直接添加诗歌（从二维数组读取）
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

    // 展开所有顶级项
    treeWidget->expandAll();

    return treeWidget;
}

void MainWindow::addToHistory(int index)
{
    // 避免连续添加相同的索引
    if (!historyStack.isEmpty() && historyStack.top() == index) {
        return;
    }
    historyStack.push(index);
    // 限制历史记录最大数量为50
    if (historyStack.size() > 50) {
        // QStack没有remove(0)，需要用临时变量
        QStack<int> temp;
        for (int i = 1; i < historyStack.size(); ++i) {
            temp.push(historyStack[i]);
        }
        historyStack = temp;
    }
}

void MainWindow::initUI()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // 左侧菜单
    QWidget* leftWidget = new QWidget;
    leftWidget->setFixedWidth(240);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(12);

    titleLabel = new QLabel("诗经 · 诗词阅读器");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color:#A84E4E;");
    titleLabel->setAlignment(Qt::AlignCenter);

    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("搜索诗词标题/内容");

    btnHome = new QPushButton("首页所有诗词");
    btnHistory = new QPushButton("阅读历史(栈)");
    btnReadList = new QPushButton("待读列表(队列)");

    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(searchEdit);
    leftLayout->addWidget(btnHome);
    leftLayout->addWidget(btnHistory);
    leftLayout->addWidget(btnReadList);
    leftLayout->addWidget(btnCategory);
    leftLayout->addStretch();

    // 添加分类树
    QLabel* treeLabel = new QLabel("快速导航");
    treeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #3D352A; margin-top: 10px;");
    leftLayout->addWidget(treeLabel);

    QTreeWidget* categoryTree = createCategoryTree();
    leftLayout->addWidget(categoryTree, 1);

    leftLayout->addStretch();

    // 右侧阅读区
    QWidget* rightWidget = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(16);

    poemTitle = new QLabel("标题");
    poemTitle->setAlignment(Qt::AlignCenter);
    poemTitle->setStyleSheet("font-size:28px;font-weight:bold;color:#3D352A;");

    poemChapter = new QLabel("章节");
    poemChapter->setAlignment(Qt::AlignCenter);
    poemChapter->setStyleSheet("font-size:16px;color:#8A7E71;");

    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("border:none;background:transparent;");

    QWidget* contentContainer = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentContainer);

    poemContent = new QLabel("内容");
    poemContent->setWordWrap(true);
    poemContent->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    poemContent->setStyleSheet(R"(
        font-size:20px;
        line-height:1.8;
        color:#3D352A;
        background:white;
        border:1px solid #D9C7B8;
        border-radius:12px;
        padding:20px;
    )");
    poemContent->setMinimumHeight(500);

    contentLayout->addWidget(poemContent);
    scroll->setWidget(contentContainer);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnPrev = new QPushButton("上一首");
    btnNext = new QPushButton("下一首");
    btnAddToList = new QPushButton("加入待读");
    btnJumpQueue = new QPushButton("插队待读"); // 新增实例化
    btnImageryRelation = new QPushButton("意象相关");

    btnLayout->addWidget(btnPrev);
    btnLayout->addWidget(btnNext);
    btnLayout->addWidget(btnAddToList);
    btnLayout->addWidget(btnJumpQueue); // 加入到布局中
    btnLayout->addWidget(btnImageryRelation);

    rightLayout->addWidget(poemTitle);
    rightLayout->addWidget(poemChapter);
    rightLayout->addWidget(scroll, 1);
    rightLayout->addLayout(btnLayout);

    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(rightWidget, 1);

    connect(btnPrev, &QPushButton::clicked, this, &MainWindow::onPrevClicked);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(btnImageryRelation, &QPushButton::clicked, this, &MainWindow::onImageryRelationClicked);
    // 连接新增的队列功能信号槽
    connect(btnAddToList, &QPushButton::clicked, this, &MainWindow::onAddToListClicked);
    connect(btnJumpQueue, &QPushButton::clicked, this, &MainWindow::onJumpQueueClicked);
    connect(btnReadList, &QPushButton::clicked, this, &MainWindow::onReadListClicked);
    // 新增分类树信号连接
    connect(btnHome, &QPushButton::clicked, this, &MainWindow::onHomeClicked);
    connect(btnHistory, &QPushButton::clicked, this, &MainWindow::onHistoryClicked);
    connect(categoryTree, &QTreeWidget::itemClicked, this, &MainWindow::onCategoryItemClicked);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
}

void MainWindow::loadFirstPoem()
{
    const auto& poems = PoemManager::instance().getAllPoems();
    if (poems.isEmpty()) {
        poemTitle->setText("未加载到诗歌数据");
        poemChapter->clear();
        poemContent->setText("请检查 resource/诗经.json 是否存在。");
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

    poemTitle->setText(poem.title);
    poemChapter->setText(poem.chapter + " · " + poem.section);
    poemContent->setText(poem.content);
}

void MainWindow::onPrevClicked()
{
    if (isReadingQueue) {
        QMessageBox::information(this, "提示", "正在播放待读队列，请点击“下一首”继续，不支持上一首。");
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
            // 队列还有诗歌，弹出队首并播放
            int nextIndex = readQueue.takeFirst();
            addToHistory(nextIndex);
            showPoemByIndex(nextIndex);
        }
        else {
            // 队列播放完毕，恢复原状态和位置
            isReadingQueue = false;
            showPoemByIndex(preQueueIndex);
            QMessageBox::information(this, "提示", "待读队列已播放完毕，已回到原来的阅读位置。");
        }
        return;
    }

    // 原有的常规下一首逻辑
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
    readQueue.append(currentIndex); // 顺序加入队尾
    QMessageBox::information(this, "提示", "当前诗歌已加入待读队列尾部。");
}

void MainWindow::onJumpQueueClicked()
{
    readQueue.prepend(currentIndex); // 插队加入队首
    QMessageBox::information(this, "提示", "当前诗歌已插队至待读队列头部。");
}

void MainWindow::onReadListClicked()
{
    if (readQueue.isEmpty()) {
        QMessageBox::information(this, "提示", "当前待读队列为空！请先添加诗歌。");
        return;
    }

    if (isReadingQueue) {
        QMessageBox::information(this, "提示", "当前已经在播放待读队列中了。");
        return;
    }

    // 记录开始播放队列前的当前诗歌索引
    preQueueIndex = currentIndex;
    isReadingQueue = true;

    // 取出队首第一首诗开始播放
    int firstIndex = readQueue.takeFirst();
    addToHistory(firstIndex);
    showPoemByIndex(firstIndex);
}

// 新增：分类树点击处理
void MainWindow::onCategoryItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    if (!item) return;

    QVariant data = item->data(0, Qt::UserRole);
    int index = data.toInt();

    // 只有诗歌节点（index >= 0）才进行跳转
    if (index >= 0) {
        addToHistory(index);
        showPoemByIndex(index);
    }
}

// 新增：历史记录处理
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

    QString historyText = "阅读历史记录 (最近10条):\n\n";
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
    msgBox.setDefaultButton(QMessageBox::Yes);

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
        QMessageBox::information(this, "提示", "已返回首页，共" + QString::number(poems.size()) + "首诗歌。");
    }
}

void MainWindow::onSearch()
{
    QString text = searchEdit->text();
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