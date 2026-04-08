#pragma once
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>

class RecitePage : public QWidget {
    Q_OBJECT

signals:
    void openReadingPage(const QList<int>& readingList, int startIndex, const QString& sourcePage);
    void reciteListCleared();

public:
    explicit RecitePage(QWidget* parent = nullptr);
    void refreshList();
    void setReciteList(const QList<int>& reciteList);
    QList<int> getReciteList() const { return m_reciteList; }

private slots:
    void onItemClicked(QListWidgetItem* item);
    void onClearListClicked();

private:
    QListWidget* m_listWidget;
    QList<int> m_reciteList;
};