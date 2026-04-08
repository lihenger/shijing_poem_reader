#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>

class HomePage : public QWidget {
    Q_OBJECT

signals:
    void openReadingPage(const QList<int>& readingList, int startIndex, const QString& sourcePage);

public:
    explicit HomePage(QWidget* parent = nullptr);

private slots:
    void onSearchTextChanged(const QString& text);
    void onSearchReturnPressed();
    void onSearchResultClicked(QListWidgetItem* item);

private:
    void performSearch(const QString& keyword);

    QLineEdit* m_searchEdit;
    QListWidget* m_resultList;
    QList<int> m_searchResults;
};