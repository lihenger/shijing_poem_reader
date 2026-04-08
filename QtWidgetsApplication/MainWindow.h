#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QWidget>
#include <QList> // QList 用于实现队列
#include <QTreeWidget>  // 用于显示分类树
#include <QStack>       // 用于历史栈

// 定义三个类别：风、雅、颂
const int CATEGORY_FENG = 0;    // 国风
const int CATEGORY_YA = 1;      // 雅
const int CATEGORY_SONG = 2;    // 颂
const int CATEGORY_COUNT = 3;   // 类别总数

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QLabel* titleLabel;
    QLineEdit* searchEdit;
    QPushButton* btnHome;
    QPushButton* btnHistory;
    QPushButton* btnReadList;
    QPushButton* btnCategory;

    QLabel* poemTitle;
    QLabel* poemChapter;
    QLabel* poemContent;
    QPushButton* btnPrev;
    QPushButton* btnNext;
    QPushButton* btnAddToList;
    QPushButton *btnJumpQueue; // 插队待读按钮
    QPushButton* btnImageryRelation;

    int currentIndex;
    
    QList<int> readQueue;      // 待读队列，存储诗歌的索引
    bool isReadingQueue;       // 标记当前是否正在播放待读队列
    int preQueueIndex;         // 记录点击播放前的诗歌索引，用于播放完后返回

    // 新增：历史记录栈
    QStack<int> historyStack;
    // 新增：二维数组分类存储
    int** m_categoryPoems;      // 二维数组：m_categoryPoems[类别][诗歌索引]
    int m_categorySizes[3];     // 每类诗歌的数量
    QString m_categoryNames[3];  // 类别名称

    void initUI();
    void loadFirstPoem();
    void showPoemByIndex(int index);

    // 初始化分类数据（使用二维数组）
    void initCategoryData();

    // 释放二维数组内存
    void releaseCategoryData();

    // 新增：创建分类树
    QTreeWidget* createCategoryTree();

    // 新增：添加到历史记录
    void addToHistory(int index);

private slots:
    void onPrevClicked();
    void onNextClicked();
    void onImageryRelationClicked();

    void onAddToListClicked();   // 顺序加入待读
    void onJumpQueueClicked();   // 插队加入待读
    void onReadListClicked();    // 开始播放待读队列

    // 新增：分类树点击槽函数
    void onCategoryItemClicked(QTreeWidgetItem* item, int column);
    // 新增：历史记录槽函数
    void onHistoryClicked();
    // 新增：首页所有诗词槽函数
    void onHomeClicked();
    // 新增：搜索功能
    void onSearch();
};

#endif // MAINWINDOW_H