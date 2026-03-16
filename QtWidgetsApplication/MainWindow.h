#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // 左侧控件
    QLabel* titleLabel;
    QLineEdit* searchEdit;
    QPushButton* btnHome;
    QPushButton* btnHistory;
    QPushButton* btnReadList;
    QPushButton* btnCategory;

    // 右侧控件
    QLabel* poemTitle;
    QLabel* poemChapter;
    QLabel* poemContent;
    QPushButton* btnPrev;
    QPushButton* btnNext;
    QPushButton* btnAddToList;

    // 初始化界面
    void initUI();
    // 加载第一首诗
    void loadFirstPoem();
};

#endif // MAINWINDOW_H