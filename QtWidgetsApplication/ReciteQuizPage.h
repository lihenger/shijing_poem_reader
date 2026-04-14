#ifndef RECITEQUIZPAGE_H
#define RECITEQUIZPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QList>
#include <QListWidget>

class ReciteQuizPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReciteQuizPage(QWidget* parent = nullptr);
    void setReciteList(const QList<int>& reciteIndices);

signals:
    void backToMain();

private slots:
    void onStartQuiz();
    void onCheckAnswer();      // 填入答案
    void onNext();
    void onPrevious();
    void onRestart();
    void onBackToList();

private:
    void setupUI();
    void showSelectionPanel();
    void showQuizPanel();
    void loadPoem(int poemIndex);
    void updateDisplay();
    void finishQuiz();
    int currentFillClauseIndex() const;

    struct ClauseInfo {
        QString text;          // 半句原文（不含标点）
        QString punctuation;   // 后面的标点
    };

    QList<int> m_reciteIndices;
    QMap<int, QString> m_indexToTitle;
    int m_currentPoemIndex;
    QList<ClauseInfo> m_clauses;
    QList<bool> m_needFill;             // 该半句是否需要填空
    QList<QString> m_userAnswers;       // 用户填入的内容
    QList<int> m_fillIndices;           // 需要填空的半句索引
    int m_currentFillPos;               // 当前填空位置
    bool m_quizFinished;

    QWidget* m_selectionWidget;
    QWidget* m_quizWidget;
    QListWidget* m_poemListWidget;
    QPushButton* m_startBtn;
    QLabel* m_titleLabel;
    QProgressBar* m_progressBar;
    QLabel* m_originalTextLabel;
    QLabel* m_questionLabel;
    QLineEdit* m_answerEdit;
    QLabel* m_feedbackLabel;
    QPushButton* m_checkBtn;
    QPushButton* m_nextBtn;
    QPushButton* m_prevBtn;
    QPushButton* m_restartBtn;
    QPushButton* m_backBtn;
};

#endif // RECITEQUIZPAGE_H