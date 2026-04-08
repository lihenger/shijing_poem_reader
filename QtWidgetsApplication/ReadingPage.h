#pragma once
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>

class ImageryGraph;

class ReadingPage : public QWidget {
    Q_OBJECT

signals:
    void exitReadingPage();
    void addToRecite(int poemIndex);
    void removeFromRecite(int poemIndex);
    void addToHistory(int poemIndex);

public:
    explicit ReadingPage(QWidget* parent = nullptr);
    void setReadingList(const QList<int>& readingList, int startIndex);

private slots:
    void onReturnClicked();
    void onPrevClicked();
    void onNextClicked();
    void onAddToReciteClicked();
    void onRemoveFromReciteClicked();
    void onRecommendClicked(QListWidgetItem* item);
    void onReadingListItemClicked(QListWidgetItem* item);

private:
    void loadPoem(int index);
    void updateImageryAndRecommend();
    void highlightCurrentItem();

    QList<int> m_readingList;
    int m_currentIndex;
    ImageryGraph* m_imageryGraph;

    QPushButton* m_btnReturn;
    QListWidget* m_readingListWidget;
    QLabel* m_poemTitle;
    QLabel* m_poemChapter;
    QTextEdit* m_poemContent;
    QListWidget* m_imageryList;
    QListWidget* m_recommendList;
    QPushButton* m_btnPrev;
    QPushButton* m_btnNext;
    QPushButton* m_btnAddToRecite;
    QPushButton* m_btnRemoveFromRecite;
};