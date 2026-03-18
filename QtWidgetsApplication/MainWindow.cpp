#include "Mainwindow.h"
#include "PoemManager.h"
#include <QFont>
#include <QPalette>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // 窗口基础设置
    this->setWindowTitle("诗经阅读器");
    this->resize(950, 650);
    this->setStyleSheet(R"(
        QMainWindow { background-color: #F9F6F0; }
        QPushButton {
            background-color: #EFEAE3;
            color: #3D352A;
            border:none;
            border-radius:12px;
            padding:10px;
            font-size:14px;
        }
        QPushButton:hover {
            background-color: #A84E4E;
            color:white;
        }
        QLineEdit {
            border:1px solid #D9C7B8;
            border-radius:10px;
            padding:8px;
            background-color:white;
        }
    )");
     
    loadFirstPoem(); // 一打开就显示第一首诗
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUI()
{
    // 中心部件
    QWidget* central = new QWidget(this);
    this->setCentralWidget(central);

    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 左右分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(5);
    mainLayout->addWidget(splitter);

    // ================== 左侧：菜单 ==================
    QWidget* leftWidget = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setSpacing(10);

    titleLabel = new QLabel("诗经 · 诗词阅读器");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color:#A84E4E");
    titleLabel->setAlignment(Qt::AlignCenter);

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("搜索诗词标题/内容");

    btnHome = new QPushButton("首页所有诗词");
    btnHistory = new QPushButton("阅读历史(栈)");
    btnReadList = new QPushButton("待读列表(队列)");
    btnCategory = new QPushButton("风雅颂分类(树)");

    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(searchEdit);
    leftLayout->addWidget(btnHome);
    leftLayout->addWidget(btnHistory);
    leftLayout->addWidget(btnReadList);
    leftLayout->addWidget(btnCategory);
    leftLayout->addStretch();

    // ================== 右侧：内容 ==================
    QWidget* rightWidget = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(15);

    poemTitle = new QLabel("标题");
    poemTitle->setStyleSheet("font-size:26px; font-weight:bold; color:#3D352A");
    poemTitle->setAlignment(Qt::AlignCenter);

    poemChapter = new QLabel("章节");
    poemChapter->setStyleSheet("font-size:14px; color:#8A7E71");
    poemChapter->setAlignment(Qt::AlignCenter);

    // 滚动区域
    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("border:none");

    poemContent = new QLabel("内容");
    poemContent->setStyleSheet("font-size:18px; color:#3D352A; line-height:1.6");
    poemContent->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    poemContent->setWordWrap(true);
    scroll->setWidget(poemContent);

    // 按钮栏
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnPrev = new QPushButton("上一首");
    btnNext = new QPushButton("下一首");
    btnAddToList = new QPushButton("加入待读");
    btnLayout->addWidget(btnPrev);
    btnLayout->addWidget(btnNext);
    btnLayout->addWidget(btnAddToList);

    rightLayout->addWidget(poemTitle);
    rightLayout->addWidget(poemChapter);
    rightLayout->addWidget(scroll);
    rightLayout->addLayout(btnLayout);

    // 加入分割器
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    splitter->setSizes({ 260, 650 });
}

// 自动加载第一首诗
void MainWindow::loadFirstPoem()
{
    auto& poems = PoemManager::instance().getAllPoems();
    if (poems.isEmpty()) return;

    auto& p = poems[0];
    poemTitle->setText(p.title);
    poemChapter->setText(p.chapter + " · " + p.section);
    poemContent->setText(p.content);
}