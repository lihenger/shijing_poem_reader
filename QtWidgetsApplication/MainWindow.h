#pragma once

#include <QMainWindow>
#include <QStack>
#include <QList>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class HomePage;
class CategoryPage;
class RecitePage;
class HistoryPage;
class OtherPage;
class ReadingPage;
class ReciteQuizPage;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void enterReadingPage(const QList<int>& readingList, int startIndex, const QString& sourcePage);
    void exitReadingPage();

    QList<int> getReciteList() const { return m_reciteList; }
    void addToReciteList(int poemIndex);
    void removeFromReciteList(int poemIndex);

    QStack<int> getHistoryStack() const { return m_historyStack; }
    void addToHistory(int poemIndex);

private slots:
    void onHomeClicked();
    void onSequentialReadClicked();
    void onCategoryClicked();
    void onReciteClicked();
    void onReciteQuizClicked();
    void onHistoryClicked();
    void onOtherClicked();
    void onRandomClicked();

private:
    void initUI();
    void initCategoryData();
    void releaseCategoryData();
    void initSidebar();
    void initStackedPages();
    QTreeWidget* createCategoryTree();

    enum { CATEGORY_FENG, CATEGORY_YA, CATEGORY_SONG, CATEGORY_COUNT };
    int** m_categoryPoems;
    int m_categorySizes[CATEGORY_COUNT];
    QString m_categoryNames[CATEGORY_COUNT];

    QWidget* m_centralWidget;
    QStackedWidget* m_stackedWidget;
    QWidget* m_sidebarWidget;

    QPushButton* m_btnHome;
    QPushButton* m_btnSequentialRead;
    QPushButton* m_btnCategory;
    QPushButton* m_btnRecite;
    QPushButton* m_btnReciteQuiz;
    QPushButton* m_btnHistory;
    QPushButton* m_btnOther;
    QPushButton* m_btnRandom;

    HomePage* m_homePage;
    CategoryPage* m_categoryPage;
    RecitePage* m_recitePage;
    HistoryPage* m_historyPage;
    OtherPage* m_otherPage;
    ReadingPage* m_readingPage;
    ReciteQuizPage* m_reciteQuizPage;

    QList<int> m_reciteList;
    QStack<int> m_historyStack;
    QString m_previousPage;
};