#include "HomePage.h"
#include "PoemManager.h"
#include <QVBoxLayout>
#include <QLabel>

HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(50, 100, 50, 50);
    layout->setSpacing(30);

    QLabel* titleLabel = new QLabel("詩經");
    titleLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: #8B4513; font-family: '楷体';");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("风 · 雅 · 颂");
    subtitleLabel->setStyleSheet("font-size: 18px; color: #A0522D; font-family: '楷体';");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitleLabel);

    layout->addSpacing(50);

    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("输入诗歌标题或内容进行搜索...");
    m_searchEdit->setStyleSheet("QLineEdit { font-size: 16px; padding: 12px 20px; border-radius: 30px; }");
    m_searchEdit->setFixedHeight(50);
    layout->addWidget(m_searchEdit);

    m_resultList = new QListWidget();
    m_resultList->setStyleSheet("QListWidget { border: 1px solid #D2B48C; border-radius: 10px; background: #FFF8F0; }"
        "QListWidget::item { padding: 10px; }");
    m_resultList->setVisible(false);
    m_resultList->setMaximumHeight(300);
    layout->addWidget(m_resultList);

    layout->addStretch();

    connect(m_searchEdit, &QLineEdit::textChanged, this, &HomePage::onSearchTextChanged);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &HomePage::onSearchReturnPressed);
    connect(m_resultList, &QListWidget::itemClicked, this, &HomePage::onSearchResultClicked);
}

void HomePage::onSearchTextChanged(const QString& text)
{
    if (text.trimmed().isEmpty()) {
        m_resultList->setVisible(false);
        m_searchResults.clear();
        return;
    }
    performSearch(text);
}

void HomePage::onSearchReturnPressed()
{
    if (!m_searchResults.isEmpty()) {
        onSearchResultClicked(m_resultList->currentItem());
    }
}

void HomePage::onSearchResultClicked(QListWidgetItem* item)
{
    if (!item) return;
    int index = item->data(Qt::UserRole).toInt();

    const auto& poems = PoemManager::instance().getAllPoems();
    QList<int> readingList;
    for (int i = 0; i < poems.size(); ++i) {
        readingList.append(i);
    }
    emit openReadingPage(readingList, index, "home");
}

void HomePage::performSearch(const QString& keyword)
{
    const auto& poems = PoemManager::instance().getAllPoems();
    m_searchResults.clear();
    m_resultList->clear();

    for (int i = 0; i < poems.size(); ++i) {
        const Poem& poem = poems[i];
        if (poem.title.contains(keyword, Qt::CaseInsensitive) ||
            poem.content.contains(keyword, Qt::CaseInsensitive)) {
            m_searchResults.append(i);
            QListWidgetItem* item = new QListWidgetItem(poem.title);
            item->setData(Qt::UserRole, i);
            m_resultList->addItem(item);
        }
    }

    m_resultList->setVisible(!m_searchResults.isEmpty());
    if (m_searchResults.isEmpty()) {
        QListWidgetItem* item = new QListWidgetItem("未找到相关诗歌");
        item->setFlags(Qt::NoItemFlags);
        m_resultList->addItem(item);
    }
}