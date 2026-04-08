#ifndef IMAGERYWINDOW_H
#define IMAGERYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include "Poem.h"
#include "ImageryGraph.h"

class ImageryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ImageryWindow(const Poem& currentPoem, QWidget* parent = nullptr);

private slots:
    void onCategoryClicked(QListWidgetItem* item);
    void onCategoryResultClicked(QListWidgetItem* item);
    void onSimilarPoemClicked(QListWidgetItem* item);
    void showMostSimilarPoems();

private:
    Poem m_currentPoem;
    ImageryGraph m_graph;
    int m_currentIndex;

    // 左侧：当前诗歌
    QLabel* titleLabel;
    QLabel* authorLabel;
    QTextEdit* contentText;

    // 中间：类别与相关诗歌
    QListWidget* categoryList;
    QListWidget* categoryResultList;
    QPushButton* similarButton;
    QListWidget* similarList;

    // 右侧：点击后显示的相关诗歌
    QLabel* previewTitleLabel;
    QLabel* previewAuthorLabel;
    QTextEdit* previewContentText;

    void initUI();
    void buildGraphData();
    void loadCurrentPoemInfo();
    void loadCategoryList();
    void displayPoemPreviewById(int poemId);
};

#endif // IMAGERYWINDOW_H