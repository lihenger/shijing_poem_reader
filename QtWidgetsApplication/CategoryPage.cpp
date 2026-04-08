#include "CategoryPage.h"
#include "PoemManager.h"
#include <QVBoxLayout>
#include <QLabel>

CategoryPage::CategoryPage(QWidget* parent)
    : QWidget(parent)
    , m_categoryPoems(nullptr)
    , m_categorySizes(nullptr)
    , m_categoryNames(nullptr)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("分类浏览");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #8B4513; font-family: '楷体'; margin-bottom: 20px;");
    layout->addWidget(titleLabel);

    m_treeWidget = new QTreeWidget();
    m_treeWidget->setHeaderLabel("诗经分类");
    layout->addWidget(m_treeWidget);

    connect(m_treeWidget, &QTreeWidget::itemClicked, this, &CategoryPage::onItemClicked);
}

void CategoryPage::setCategoryData(int** categoryPoems, int* categorySizes, const QString* categoryNames)
{
    m_categoryPoems = categoryPoems;
    m_categorySizes = categorySizes;
    m_categoryNames = categoryNames;

    m_treeWidget->clear();
    const auto& poems = PoemManager::instance().getAllPoems();

    for (int i = 0; i < 3; ++i) {
        QTreeWidgetItem* categoryItem = new QTreeWidgetItem();
        categoryItem->setText(0, QString("%1 (%2首)").arg(m_categoryNames[i]).arg(m_categorySizes[i]));

        for (int j = 0; j < m_categorySizes[i]; ++j) {
            int idx = m_categoryPoems[i][j];
            QTreeWidgetItem* poemItem = new QTreeWidgetItem();
            poemItem->setText(0, poems[idx].title);
            poemItem->setData(0, Qt::UserRole, idx);
            categoryItem->addChild(poemItem);
        }
        m_treeWidget->addTopLevelItem(categoryItem);
    }
    m_treeWidget->expandAll();
}

void CategoryPage::onItemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    if (!item) return;

    int index = item->data(0, Qt::UserRole).toInt();
    if (index < 0) return;

    const auto& poems = PoemManager::instance().getAllPoems();
    const Poem& poem = poems[index];

    QList<int> categoryList;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < m_categorySizes[i]; ++j) {
            int idx = m_categoryPoems[i][j];
            const Poem& p = poems[idx];
            if (p.chapter == poem.chapter) {
                categoryList.append(idx);
            }
        }
    }

    int startIndex = categoryList.indexOf(index);
    emit openReadingPage(categoryList, startIndex, "category");
}