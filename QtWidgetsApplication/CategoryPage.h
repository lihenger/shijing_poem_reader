#pragma once
#include <QWidget>
#include <QTreeWidget>

class CategoryPage : public QWidget {
    Q_OBJECT

signals:
    void openReadingPage(const QList<int>& readingList, int startIndex, const QString& sourcePage);

public:
    explicit CategoryPage(QWidget* parent = nullptr);
    void setCategoryData(int** categoryPoems, int* categorySizes, const QString* categoryNames);

private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);

private:
    QTreeWidget* m_treeWidget;
    int** m_categoryPoems;
    int* m_categorySizes;
    const QString* m_categoryNames;
};