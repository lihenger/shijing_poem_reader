// ImageryWindow.h
#pragma once
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include "ImageryGraph.h"
#include "Poem.h"

class ImageryWindow : public QWidget {
    Q_OBJECT

public:
    explicit ImageryWindow(const Poem& currentPoem, QWidget* parent = nullptr);

private slots:
    void onCategoryClicked(QListWidgetItem* item);
    void onSimilarPoemClicked(QListWidgetItem* item);
    void showMostSimilarPoems();

private:
    void initUI();
    void buildGraphData();
    void loadCurrentPoemInfo();
    void loadCategoryList();
    void displayPoemPreviewById(int poemId);

    Poem m_currentPoem;
    ImageryGraph m_graph;
    int m_currentIndex;

    // UI组件
    QLabel* m_titleLabel;
    QLabel* m_authorLabel;
    QTextEdit* m_contentText;
    QListWidget* m_categoryList;
    QPushButton* m_similarButton;
    QListWidget* m_similarList;
    QLabel* m_previewTitleLabel;
    QTextEdit* m_previewContentText;
};