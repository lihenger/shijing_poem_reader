#include "ReciteQuizPage.h"
#include "PoemManager.h"
#include "ChineseStyle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include <QRandomGenerator>
#include <random>
#include <algorithm>

ReciteQuizPage::ReciteQuizPage(QWidget* parent)
    : QWidget(parent)
    , m_currentPoemIndex(-1)
    , m_currentFillPos(0)
    , m_quizFinished(false)
{
    setupUI();
    showSelectionPanel();
}

void ReciteQuizPage::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);
    setStyleSheet(ChineseStyle::getMainStyle());

    // 选择界面
    m_selectionWidget = new QWidget(this);
    QVBoxLayout* selLayout = new QVBoxLayout(m_selectionWidget);
    QLabel* titleLabel = new QLabel("选择要背诵测验的诗歌", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #8B4513; margin-bottom: 20px;");
    selLayout->addWidget(titleLabel);

    m_poemListWidget = new QListWidget(this);
    m_poemListWidget->setStyleSheet("QListWidget::item { padding: 8px; }");
    selLayout->addWidget(m_poemListWidget);

    m_startBtn = new QPushButton("开始测验", this);
    m_startBtn->setFixedHeight(40);
    selLayout->addWidget(m_startBtn);

    //QPushButton* backBtn = new QPushButton("返回首页", this);
    //backBtn->setFixedHeight(40);
    //selLayout->addWidget(backBtn);
    //connect(backBtn, &QPushButton::clicked, this, &ReciteQuizPage::backToMain);
    connect(m_startBtn, &QPushButton::clicked, this, &ReciteQuizPage::onStartQuiz);

    mainLayout->addWidget(m_selectionWidget);

    // 测验界面
    m_quizWidget = new QWidget(this);
    QVBoxLayout* quizLayout = new QVBoxLayout(m_quizWidget);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("楷体", 18, QFont::Bold);
    m_titleLabel->setFont(titleFont);
    quizLayout->addWidget(m_titleLabel);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setTextVisible(true);
    quizLayout->addWidget(m_progressBar);

    m_originalTextLabel = new QLabel(this);
    m_originalTextLabel->setWordWrap(true);
    m_originalTextLabel->setAlignment(Qt::AlignTop);
    QFont textFont("楷体", 14);
    m_originalTextLabel->setFont(textFont);
    m_originalTextLabel->setStyleSheet("background-color: #FFF8F0; padding: 15px; border-radius: 8px;");
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_originalTextLabel);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(250);
    quizLayout->addWidget(scrollArea);

    QHBoxLayout* fillLayout = new QHBoxLayout();
    m_questionLabel = new QLabel("当前填空：", this);
    m_questionLabel->setFont(QFont("楷体", 12));
    m_answerEdit = new QLineEdit(this);
    m_answerEdit->setPlaceholderText("请输入一个半句（不含标点）");
    m_answerEdit->setMinimumWidth(350);
    m_checkBtn = new QPushButton("填入", this);
    fillLayout->addWidget(m_questionLabel);
    fillLayout->addWidget(m_answerEdit);
    fillLayout->addWidget(m_checkBtn);
    quizLayout->addLayout(fillLayout);

    m_feedbackLabel = new QLabel(this);
    m_feedbackLabel->setAlignment(Qt::AlignCenter);
    m_feedbackLabel->setStyleSheet("color: #8B4513;");
    quizLayout->addWidget(m_feedbackLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_prevBtn = new QPushButton("上一空", this);
    m_nextBtn = new QPushButton("下一空", this);
    m_restartBtn = new QPushButton("重新开始", this);
    m_backBtn = new QPushButton("返回诗歌列表", this);
    buttonLayout->addWidget(m_prevBtn);
    buttonLayout->addWidget(m_nextBtn);
    buttonLayout->addWidget(m_restartBtn);
    buttonLayout->addWidget(m_backBtn);
    quizLayout->addLayout(buttonLayout);

    mainLayout->addWidget(m_quizWidget);

    connect(m_checkBtn, &QPushButton::clicked, this, &ReciteQuizPage::onCheckAnswer);
    connect(m_nextBtn, &QPushButton::clicked, this, &ReciteQuizPage::onNext);
    connect(m_prevBtn, &QPushButton::clicked, this, &ReciteQuizPage::onPrevious);
    connect(m_restartBtn, &QPushButton::clicked, this, &ReciteQuizPage::onRestart);
    connect(m_backBtn, &QPushButton::clicked, this, &ReciteQuizPage::onBackToList);
}

void ReciteQuizPage::setReciteList(const QList<int>& reciteIndices)
{
    m_reciteIndices = reciteIndices;
    m_poemListWidget->clear();
    m_indexToTitle.clear();

    const auto& poems = PoemManager::instance().getAllPoems();
    for (int idx : reciteIndices) {
        if (idx >= 0 && idx < poems.size()) {
            QString title = poems[idx].title;
            m_indexToTitle[idx] = title;
            m_poemListWidget->addItem(title);
        }
    }

    if (m_poemListWidget->count() == 0) {
        m_poemListWidget->addItem("（暂无背诵诗歌，请先在阅读页面加入背诵列表）");
        m_startBtn->setEnabled(false);
    }
    else {
        m_startBtn->setEnabled(true);
    }
}

void ReciteQuizPage::showSelectionPanel()
{
    m_selectionWidget->setVisible(true);
    m_quizWidget->setVisible(false);
}

void ReciteQuizPage::showQuizPanel()
{
    m_selectionWidget->setVisible(false);
    m_quizWidget->setVisible(true);
}

void ReciteQuizPage::onStartQuiz()
{
    int row = m_poemListWidget->currentRow();
    if (row < 0 || row >= m_reciteIndices.size()) {
        QMessageBox::warning(this, "提示", "请先选择一首诗歌");
        return;
    }
    int poemIndex = m_reciteIndices[row];
    loadPoem(poemIndex);
    showQuizPanel();
}

void ReciteQuizPage::loadPoem(int poemIndex)
{
    const QList<Poem>& poems = PoemManager::instance().getAllPoems();
    if (poemIndex < 0 || poemIndex >= poems.size()) {
        qWarning() << "无效诗歌索引" << poemIndex;
        return;
    }

    m_currentPoemIndex = poemIndex;
    const Poem& poem = poems[poemIndex];
    m_titleLabel->setText(poem.title);

    // 解析诗歌内容，保留原始标点
    QString content = poem.content;
    content.replace("\n", " ");
    m_clauses.clear();
    QRegularExpression re("([^，,。]+)([，,。]?)");
    QRegularExpressionMatchIterator it = re.globalMatch(content);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString text = match.captured(1).trimmed();
        QString punct = match.captured(2);
        if (!text.isEmpty()) {
            ClauseInfo info;
            info.text = text;
            info.punctuation = punct;
            m_clauses.append(info);
        }
    }
    if (!m_clauses.isEmpty() && m_clauses.last().punctuation.isEmpty()) {
        m_clauses.last().punctuation = "。";
    }

    // 随机选择 35%-50% 的半句作为挖空
    int total = m_clauses.size();
    int minFill = static_cast<int>(total * 0.35);
    int maxFill = static_cast<int>(total * 0.50);
    if (minFill < 1) minFill = 1;
    if (maxFill > total) maxFill = total;
    int fillCount = QRandomGenerator::global()->bounded(minFill, maxFill + 1);

    m_needFill.fill(false, total);
    QList<int> indices;
    for (int i = 0; i < total; ++i) indices.append(i);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{ std::random_device{}() });
    for (int i = 0; i < fillCount; ++i) {
        m_needFill[indices[i]] = true;
    }

    m_userAnswers.fill("", total);
    m_fillIndices.clear();
    for (int i = 0; i < total; ++i) {
        if (m_needFill[i]) {
            m_fillIndices.append(i);
        }
    }
    m_currentFillPos = 0;
    m_quizFinished = false;

    updateDisplay();
}

void ReciteQuizPage::updateDisplay()
{
    int filledCount = 0;
    for (int idx : m_fillIndices) {
        if (!m_userAnswers[idx].isEmpty()) filledCount++;
    }
    int totalFill = m_fillIndices.size();
    m_progressBar->setMaximum(totalFill);
    m_progressBar->setValue(filledCount);
    m_progressBar->setFormat(QString("填空进度：%1 / %2").arg(filledCount).arg(totalFill));

    // 构建显示文本，保留原始标点
    QString displayText;
    for (int i = 0; i < m_clauses.size(); ++i) {
        const ClauseInfo& clause = m_clauses[i];
        if (!m_needFill[i]) {
            displayText += clause.text + clause.punctuation;
        }
        else {
            if (!m_userAnswers[i].isEmpty()) {
                displayText += m_userAnswers[i] + clause.punctuation;
            }
            else {
                if (i == currentFillClauseIndex() && !m_quizFinished) {
                    displayText += "【________】" + clause.punctuation;
                }
                else {
                    displayText += "【 ? 】" + clause.punctuation;
                }
            }
        }
    }
    m_originalTextLabel->setText(displayText);

    bool canInteract = !m_quizFinished && m_userAnswers[currentFillClauseIndex()].isEmpty();
    m_checkBtn->setEnabled(canInteract);
    m_answerEdit->setEnabled(canInteract);
    m_prevBtn->setEnabled(m_currentFillPos > 0 && !m_quizFinished);
    m_nextBtn->setEnabled(m_currentFillPos < totalFill - 1 && !m_quizFinished);
    if (canInteract) m_answerEdit->setFocus();

    if (filledCount == totalFill && !m_quizFinished) {
        finishQuiz();
    }
}

int ReciteQuizPage::currentFillClauseIndex() const
{
    if (m_currentFillPos >= 0 && m_currentFillPos < m_fillIndices.size())
        return m_fillIndices[m_currentFillPos];
    return -1;
}

void ReciteQuizPage::onCheckAnswer()
{
    if (m_quizFinished) return;
    int clauseIdx = currentFillClauseIndex();
    if (clauseIdx == -1) return;
    if (!m_userAnswers[clauseIdx].isEmpty()) {
        m_feedbackLabel->setText("这一空已经填过了，请跳到下一空");
        return;
    }

    QString userAnswer = m_answerEdit->text().trimmed();
    if (userAnswer.isEmpty()) {
        m_feedbackLabel->setText("请输入内容后再填入");
        return;
    }

    // 直接填入用户输入的内容，不判断对错
    m_userAnswers[clauseIdx] = userAnswer;
    m_feedbackLabel->setText("已填入，可继续下一空");
    m_answerEdit->clear();

    // 自动跳转到下一个未填的空
    for (int i = m_currentFillPos + 1; i < m_fillIndices.size(); ++i) {
        if (m_userAnswers[m_fillIndices[i]].isEmpty()) {
            m_currentFillPos = i;
            break;
        }
    }
    updateDisplay();
}

void ReciteQuizPage::onNext()
{
    if (m_quizFinished) return;
    if (m_currentFillPos + 1 < m_fillIndices.size()) {
        m_currentFillPos++;
        updateDisplay();
        m_feedbackLabel->clear();
    }
}

void ReciteQuizPage::onPrevious()
{
    if (m_quizFinished) return;
    if (m_currentFillPos > 0) {
        m_currentFillPos--;
        updateDisplay();
        m_feedbackLabel->clear();
    }
}

void ReciteQuizPage::onRestart()
{
    loadPoem(m_currentPoemIndex);
    m_feedbackLabel->clear();
}

void ReciteQuizPage::onBackToList()
{
    showSelectionPanel();
}

void ReciteQuizPage::finishQuiz()
{
    m_quizFinished = true;
    int correctCount = 0;
    for (int idx : m_fillIndices) {
        if (m_userAnswers[idx] == m_clauses[idx].text) {
            correctCount++;
        }
    }
    int totalFill = m_fillIndices.size();
    double score = (double)correctCount / totalFill * 100;

    QString resultText = QString("测验完成！\n正确率：%1 / %2 ( %3% )\n\n答案对照（仅显示挖空部分）：\n")
        .arg(correctCount).arg(totalFill).arg(score, 0, 'f', 1);
    int displayIdx = 1;
    for (int idx : m_fillIndices) {
        resultText += QString("%1. 原文：%2\n    您的答案：%3\n")
            .arg(displayIdx++)
            .arg(m_clauses[idx].text)
            .arg(m_userAnswers[idx].isEmpty() ? "未填写" : m_userAnswers[idx]);
    }
    QMessageBox::information(this, "背诵测验结果", resultText);

    // 测验完成后禁用所有交互按钮
    m_checkBtn->setEnabled(false);
    m_answerEdit->setEnabled(false);
    m_nextBtn->setEnabled(false);
    m_prevBtn->setEnabled(false);
} 