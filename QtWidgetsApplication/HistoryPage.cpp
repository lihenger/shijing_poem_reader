#include "HistoryPage.h"
#include "PoemManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

HistoryPage::HistoryPage(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("阅读历史");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; font-family: '楷体';");
    layout->addWidget(titleLabel);

    QLabel* hintLabel = new QLabel("最近阅读的诗歌会显示在这里，点击可继续阅读");
    hintLabel->setStyleSheet("color: #A0927B; margin-bottom: 20px;");
    layout->addWidget(hintLabel);

    m_listWidget = new QListWidget();
    m_listWidget->setStyleSheet("QListWidget::item { padding: 12px; border-bottom: 1px solid #E8DCC8; }"
        "QListWidget::item:hover { background-color: #F5DEB3; }");
    layout->addWidget(m_listWidget);

    // 添加清空按钮
    QPushButton* clearBtn = new QPushButton("清空历史记录");
    clearBtn->setFixedHeight(40);
    clearBtn->setStyleSheet("QPushButton { background-color: #8B4513; }"
        "QPushButton:hover { background-color: #A0522D; }");
    layout->addWidget(clearBtn);

    connect(m_listWidget, &QListWidget::itemClicked, this, &HistoryPage::onItemClicked);
    connect(clearBtn, &QPushButton::clicked, this, &HistoryPage::onClearHistoryClicked);
}

void HistoryPage::setHistoryStack(const QStack<int>& historyStack)
{
    m_historyList.clear();
    QStack<int> temp = historyStack;
    QList<int> tempList;
    while (!temp.isEmpty()) {
        tempList.prepend(temp.pop());
    }
    m_historyList = tempList;
    refreshList();
}

void HistoryPage::refreshList()
{
    m_listWidget->clear();
    const auto& poems = PoemManager::instance().getAllPoems();

    if (m_historyList.isEmpty()) {
        QListWidgetItem* item = new QListWidgetItem("暂无阅读历史，开始阅读吧");
        item->setFlags(Qt::NoItemFlags);
        item->setForeground(QColor(160, 146, 123));
        m_listWidget->addItem(item);
        return;
    }

    // 从最新到最旧显示
    for (int i = m_historyList.size() - 1; i >= 0; --i) {
        int idx = m_historyList[i];
        if (idx >= 0 && idx < poems.size()) {
            const Poem& poem = poems[idx];
            int order = m_historyList.size() - i;
            QListWidgetItem* item = new QListWidgetItem(QString("%1. %2").arg(order).arg(poem.title));
            item->setData(Qt::UserRole, idx);
            m_listWidget->addItem(item);
        }
    }
}

void HistoryPage::onItemClicked(QListWidgetItem* item)
{
    if (!item) return;
    int index = item->data(Qt::UserRole).toInt();
    if (index < 0) return;

    // 找到在历史列表中的位置
    int listIndex = m_historyList.indexOf(index);
    if (listIndex != -1) {
        emit openReadingPage(m_historyList, listIndex, "history");
    }
}

void HistoryPage::onClearHistoryClicked()
{
    if (m_historyList.isEmpty()) {
        QMessageBox::information(this, "提示", "历史记录已经是空的");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认",
        "确定要清空所有历史记录吗？此操作不可撤销。",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_historyList.clear();
        refreshList();
        emit historyCleared();
        QMessageBox::information(this, "提示", "历史记录已清空");
    }
}