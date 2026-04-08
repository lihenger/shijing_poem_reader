#pragma once
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStack>
#include <QPushButton>

class HistoryPage : public QWidget {
    Q_OBJECT

signals:
    void openReadingPage(const QList<int>& readingList, int startIndex, const QString& sourcePage);
    void historyCleared();

public:
    explicit HistoryPage(QWidget* parent = nullptr);
    void refreshList();
    void setHistoryStack(const QStack<int>& historyStack);
    QList<int> getHistoryList() const { return m_historyList; }

private slots:
    void onItemClicked(QListWidgetItem* item);
    void onClearHistoryClicked();

private:
    QListWidget* m_listWidget;
    QList<int> m_historyList;
};