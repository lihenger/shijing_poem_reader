#include "RecitePage.h"
#include "PoemManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

RecitePage::RecitePage(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("背诵列表");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; font-family: '楷体';");
    layout->addWidget(titleLabel);

    QLabel* hintLabel = new QLabel("在阅读页面点击「加入背诵」可将诗歌添加到此列表，点击「取消背诵」可移除");
    hintLabel->setStyleSheet("color: #A0927B; margin-bottom: 20px;");
    layout->addWidget(hintLabel);

    m_listWidget = new QListWidget();
    m_listWidget->setStyleSheet("QListWidget::item { padding: 12px; border-bottom: 1px solid #E8DCC8; }"
        "QListWidget::item:hover { background-color: #F5DEB3; }");
    layout->addWidget(m_listWidget);

    // 添加清空按钮
    QPushButton* clearBtn = new QPushButton("清空背诵列表");
    clearBtn->setFixedHeight(40);
    clearBtn->setStyleSheet("QPushButton { background-color: #8B4513; }"
        "QPushButton:hover { background-color: #A0522D; }");
    layout->addWidget(clearBtn);

    connect(m_listWidget, &QListWidget::itemClicked, this, &RecitePage::onItemClicked);
    connect(clearBtn, &QPushButton::clicked, this, &RecitePage::onClearListClicked);
}

void RecitePage::setReciteList(const QList<int>& reciteList)
{
    m_reciteList = reciteList;
    refreshList();
}

void RecitePage::refreshList()
{
    m_listWidget->clear();
    const auto& poems = PoemManager::instance().getAllPoems();

    if (m_reciteList.isEmpty()) {
        QListWidgetItem* item = new QListWidgetItem("暂无背诵诗歌，请在阅读页面点击「加入背诵」添加");
        item->setFlags(Qt::NoItemFlags);
        item->setForeground(QColor(160, 146, 123));
        m_listWidget->addItem(item);
        return;
    }

    for (int i = 0; i < m_reciteList.size(); ++i) {
        int idx = m_reciteList[i];
        if (idx >= 0 && idx < poems.size()) {
            const Poem& poem = poems[idx];
            QListWidgetItem* item = new QListWidgetItem(QString("%1. %2").arg(i + 1).arg(poem.title));
            item->setData(Qt::UserRole, idx);
            m_listWidget->addItem(item);
        }
    }
}

void RecitePage::onItemClicked(QListWidgetItem* item)
{
    if (!item) return;
    int index = item->data(Qt::UserRole).toInt();
    if (index < 0) return;

    // 找到在背诵列表中的位置
    int listIndex = m_reciteList.indexOf(index);
    if (listIndex != -1) {
        emit openReadingPage(m_reciteList, listIndex, "recite");
    }
}

void RecitePage::onClearListClicked()
{
    if (m_reciteList.isEmpty()) {
        QMessageBox::information(this, "提示", "背诵列表已经是空的");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认",
        "确定要清空所有背诵列表吗？此操作不可撤销。",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_reciteList.clear();
        refreshList();
        emit reciteListCleared();
        QMessageBox::information(this, "提示", "背诵列表已清空");
    }
}