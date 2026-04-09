#pragma once
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>

class WordCloudPage;
class StatisticsData;
class MainWindow;

class OtherPage : public QWidget {
    Q_OBJECT

signals:
    void openReadingPage(const QList<int>& readingList, int startIndex, const QString& sourcePage);

public:
    explicit OtherPage(QWidget* parent = nullptr);
    ~OtherPage();

    void refreshData();
    void setMainWindow(MainWindow* mainWindow);

private slots:
    void onWordCloudClicked();
    void onBackToCards();

private:
    void initUI();
    void updateStatistics();

    QStackedWidget* m_stackedWidget;
    QWidget* m_cardPage;
    WordCloudPage* m_wordCloudPage;

    StatisticsData* m_statistics;
    MainWindow* m_mainWindow;
};