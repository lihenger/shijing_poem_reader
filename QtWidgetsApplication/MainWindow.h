// MainWindow.h
#pragma once
#include <QMainWindow>
#include <QStack>
#include <QList>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onPrevClicked();
    void onNextClicked();
    void onImageryRelationClicked();
    void onAddToListClicked();
    void onJumpQueueClicked();
    void onReadListClicked();
    void onHomeClicked();
    void onHistoryClicked();
    void onCategoryItemClicked(QTreeWidgetItem* item, int column);
    void onSearch();

private:
    void initUI();
    void initCategoryData();
    void releaseCategoryData();
    void loadFirstPoem();
    void showPoemByIndex(int index);
    void addToHistory(int index);
    QTreeWidget* createCategoryTree();
    void connectSignals();

    // 分类相关
    enum { CATEGORY_FENG, CATEGORY_YA, CATEGORY_SONG, CATEGORY_COUNT };
    int** m_categoryPoems;
    int m_categorySizes[CATEGORY_COUNT];
    QString m_categoryNames[CATEGORY_COUNT];

    // UI组件
    QLabel* m_poemTitle;
    QLabel* m_poemChapter;
    QTextEdit* m_poemContent;
    QLineEdit* m_searchEdit;
    QPushButton* m_btnPrev;
    QPushButton* m_btnNext;
    QPushButton* m_btnAddToList;
    QPushButton* m_btnJumpQueue;
    QPushButton* m_btnReadList;
    QPushButton* m_btnHistory;
    QPushButton* m_btnHome;
    QPushButton* m_btnImageryRelation;
    QTreeWidget* m_categoryTree;

    // 数据结构
    int currentIndex;
    bool isReadingQueue;
    int preQueueIndex;
    QStack<int> historyStack;
    QList<int> readQueue;
};