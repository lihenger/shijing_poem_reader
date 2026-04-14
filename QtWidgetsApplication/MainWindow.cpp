#include "MainWindow.h"
#include "HomePage.h"
#include "CategoryPage.h"
#include "RecitePage.h"
#include "HistoryPage.h"
#include "OtherPage.h"
#include "ReadingPage.h"
#include "ReciteQuizPage.h"
#include "PoemManager.h"
#include "ChineseStyle.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_categoryPoems(nullptr)
    , m_reciteQuizPage(nullptr)
{
    for (int i = 0; i < CATEGORY_COUNT; ++i) {
        m_categorySizes[i] = 0;
    }

    setWindowTitle("诗经");
    resize(1400, 900);
    setMinimumSize(1200, 700);

    initCategoryData();
    initUI();
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

    for (int i = 0; i < totalPoems; ++i) {
        const Poem& poem = poems[i];
        QString chapter = poem.chapter;
        if (chapter.contains("风")) {
            m_categorySizes[CATEGORY_FENG]++;
        }
        else if (chapter.contains("雅")) {
            m_categorySizes[CATEGORY_YA]++;
        }
        else if (chapter.contains("颂")) {
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
        if (chapter.contains("风")) {
            m_categoryPoems[CATEGORY_FENG][currentIndices[CATEGORY_FENG]++] = i;
        }
        else if (chapter.contains("雅")) {
            m_categoryPoems[CATEGORY_YA][currentIndices[CATEGORY_YA]++] = i;
        }
        else if (chapter.contains("颂")) {
            m_categoryPoems[CATEGORY_SONG][currentIndices[CATEGORY_SONG]++] = i;
        }
    }
}

void MainWindow::releaseCategoryData()
{
    if (m_categoryPoems) {
        for (int i = 0; i < CATEGORY_COUNT; ++i) {
            delete[] m_categoryPoems[i];
        }
        delete[] m_categoryPoems;
        m_categoryPoems = nullptr;
    }
}

void MainWindow::initUI()
{
    this->setStyleSheet(ChineseStyle::getMainStyle());

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    initSidebar();
    mainLayout->addWidget(m_sidebarWidget);

    m_stackedWidget = new QStackedWidget();
    initStackedPages();
    mainLayout->addWidget(m_stackedWidget, 1);

    m_stackedWidget->setCurrentWidget(m_homePage);
}

void MainWindow::initSidebar()
{
    m_sidebarWidget = new QWidget();
    m_sidebarWidget->setFixedWidth(200);
    m_sidebarWidget->setStyleSheet("background: #F0EBE0; border-right: 1px solid #D2B48C;");

    QVBoxLayout* layout = new QVBoxLayout(m_sidebarWidget);
    layout->setContentsMargins(10, 20, 10, 20);
    layout->setSpacing(15);

    QLabel* titleLabel = new QLabel("詩經");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; font-family: '楷体'; padding-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    m_btnHome = new QPushButton("🏠 首页");
    m_btnSequentialRead = new QPushButton("📖 顺序阅读");
    m_btnCategory = new QPushButton("📚 分类");
    m_btnRecite = new QPushButton("📝 背诵列表");      // 原有背诵列表
    m_btnReciteQuiz = new QPushButton("🧠 背诵测验");   // 新增测验按钮
    m_btnHistory = new QPushButton("⏰ 历史");
    m_btnOther = new QPushButton("✨ 其他");

    QList<QPushButton*> btns = { m_btnHome, m_btnSequentialRead, m_btnCategory,
                                 m_btnRecite, m_btnReciteQuiz, m_btnHistory, m_btnOther };
    for (auto btn : btns) {
        btn->setFixedHeight(45);
        btn->setStyleSheet("QPushButton { background: transparent; color: #5C4B3A; text-align: left; padding-left: 20px; border-radius: 10px; }"
            "QPushButton:hover { background: #D2B48C; color: #FFFFFF; }");
        layout->addWidget(btn);
    }

    layout->addStretch();

    connect(m_btnHome, &QPushButton::clicked, this, &MainWindow::onHomeClicked);
    connect(m_btnSequentialRead, &QPushButton::clicked, this, &MainWindow::onSequentialReadClicked);
    connect(m_btnCategory, &QPushButton::clicked, this, &MainWindow::onCategoryClicked);
    connect(m_btnRecite, &QPushButton::clicked, this, &MainWindow::onReciteClicked);
    connect(m_btnReciteQuiz, &QPushButton::clicked, this, &MainWindow::onReciteQuizClicked);
    connect(m_btnHistory, &QPushButton::clicked, this, &MainWindow::onHistoryClicked);
    connect(m_btnOther, &QPushButton::clicked, this, &MainWindow::onOtherClicked);
}

void MainWindow::initStackedPages()
{
    m_homePage = new HomePage(this);
    m_categoryPage = new CategoryPage(this);
    m_recitePage = new RecitePage(this);
    m_historyPage = new HistoryPage(this);
    m_otherPage = new OtherPage(this);
    m_readingPage = new ReadingPage(this);
    m_reciteQuizPage = new ReciteQuizPage(this);

    m_otherPage->setMainWindow(this);
    m_categoryPage->setCategoryData(m_categoryPoems, m_categorySizes, m_categoryNames);
    m_recitePage->setReciteList(m_reciteList);
    m_historyPage->setHistoryStack(m_historyStack);

    m_stackedWidget->addWidget(m_homePage);
    m_stackedWidget->addWidget(m_categoryPage);
    m_stackedWidget->addWidget(m_recitePage);
    m_stackedWidget->addWidget(m_historyPage);
    m_stackedWidget->addWidget(m_otherPage);
    m_stackedWidget->addWidget(m_readingPage);
    m_stackedWidget->addWidget(m_reciteQuizPage);

    connect(m_homePage, &HomePage::openReadingPage, this, &MainWindow::enterReadingPage);
    connect(m_categoryPage, &CategoryPage::openReadingPage, this, &MainWindow::enterReadingPage);
    connect(m_recitePage, &RecitePage::openReadingPage, this, &MainWindow::enterReadingPage);
    connect(m_historyPage, &HistoryPage::openReadingPage, this, &MainWindow::enterReadingPage);
    connect(m_readingPage, &ReadingPage::exitReadingPage, this, &MainWindow::exitReadingPage);
    connect(m_readingPage, &ReadingPage::addToRecite, this, &MainWindow::addToReciteList);
    connect(m_readingPage, &ReadingPage::removeFromRecite, this, &MainWindow::removeFromReciteList);
    connect(m_readingPage, &ReadingPage::addToHistory, this, &MainWindow::addToHistory);
    connect(m_otherPage, &OtherPage::openReadingPage, this, &MainWindow::enterReadingPage);
    connect(m_reciteQuizPage, &ReciteQuizPage::backToMain, this, [this]() {
        m_stackedWidget->setCurrentWidget(m_homePage);
        });

    connect(m_recitePage, &RecitePage::reciteListCleared, this, [this]() {
        m_reciteList.clear();
        });
    connect(m_historyPage, &HistoryPage::historyCleared, this, [this]() {
        m_historyStack.clear();
        });
}

void MainWindow::enterReadingPage(const QList<int>& readingList, int startIndex, const QString& sourcePage)
{
    m_previousPage = sourcePage;
    m_readingPage->setReadingList(readingList, startIndex);
    m_stackedWidget->setCurrentWidget(m_readingPage);
}

void MainWindow::exitReadingPage()
{
    if (m_previousPage == "home") {
        m_stackedWidget->setCurrentWidget(m_homePage);
    }
    else if (m_previousPage == "category") {
        m_stackedWidget->setCurrentWidget(m_categoryPage);
    }
    else if (m_previousPage == "recite") {
        m_recitePage->refreshList();
        m_stackedWidget->setCurrentWidget(m_recitePage);
    }
    else if (m_previousPage == "history") {
        m_historyPage->refreshList();
        m_stackedWidget->setCurrentWidget(m_historyPage);
    }
    else if (m_previousPage == "other") {
        m_otherPage->refreshData();
        m_stackedWidget->setCurrentWidget(m_otherPage);
    }
    else {
        m_stackedWidget->setCurrentWidget(m_homePage);
    }
}

void MainWindow::addToReciteList(int poemIndex)
{
    if (!m_reciteList.contains(poemIndex)) {
        m_reciteList.append(poemIndex);
        m_recitePage->setReciteList(m_reciteList);
        // 注意：测验页面不需要实时同步，因为它会在打开时从 m_reciteList 重新获取
    }
}

void MainWindow::removeFromReciteList(int poemIndex)
{
    m_reciteList.removeAll(poemIndex);
    m_recitePage->setReciteList(m_reciteList);
}

void MainWindow::addToHistory(int poemIndex)
{
    if (!m_historyStack.isEmpty() && m_historyStack.top() == poemIndex) {
        return;
    }

    QStack<int> temp;
    while (!m_historyStack.isEmpty()) {
        int top = m_historyStack.pop();
        if (top != poemIndex) {
            temp.push(top);
        }
    }
    while (!temp.isEmpty()) {
        m_historyStack.push(temp.pop());
    }

    m_historyStack.push(poemIndex);

    if (m_historyStack.size() > 50) {
        QStack<int> newStack;
        QList<int> tempList;
        while (!m_historyStack.isEmpty()) {
            tempList.prepend(m_historyStack.pop());
        }
        for (int i = tempList.size() - 50; i < tempList.size(); ++i) {
            newStack.push(tempList[i]);
        }
        m_historyStack = newStack;
    }

    m_historyPage->setHistoryStack(m_historyStack);
}

void MainWindow::onHomeClicked()
{
    m_stackedWidget->setCurrentWidget(m_homePage);
}

void MainWindow::onSequentialReadClicked()
{
    const auto& poems = PoemManager::instance().getAllPoems();
    QList<int> readingList;
    for (int i = 0; i < poems.size(); ++i) {
        readingList.append(i);
    }
    enterReadingPage(readingList, 0, "home");
}

void MainWindow::onCategoryClicked()
{
    m_stackedWidget->setCurrentWidget(m_categoryPage);
}

void MainWindow::onReciteClicked()
{
    // 原有背诵列表功能：显示 RecitePage
    m_recitePage->refreshList();
    m_stackedWidget->setCurrentWidget(m_recitePage);
}

void MainWindow::onReciteQuizClicked()
{
    // 新测验功能：将当前背诵列表传递给测验页面，并切换到测验页面
    m_reciteQuizPage->setReciteList(m_reciteList);
    m_stackedWidget->setCurrentWidget(m_reciteQuizPage);
}

void MainWindow::onHistoryClicked()
{
    m_historyPage->refreshList();
    m_stackedWidget->setCurrentWidget(m_historyPage);
}

void MainWindow::onOtherClicked()
{
    m_stackedWidget->setCurrentWidget(m_otherPage);
}