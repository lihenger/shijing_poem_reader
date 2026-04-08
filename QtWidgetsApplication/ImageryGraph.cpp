#include "ImageryGraph.h"
#include <QQueue>
#include <algorithm>

ImageryGraph::ImageryGraph()
{
    initImageryDictionary();
}

void ImageryGraph::initImageryDictionary()
{
    m_imageryCategoryMap.clear();
    m_wordToCategory.clear();

    m_imageryCategoryMap["天象"] = {
        "天", "日", "月", "星", "汉", "牵牛", "织女"
    };

    m_imageryCategoryMap["气候"] = {
        "风", "雨", "雪", "霜", "露", "云", "阴雨"
    };

    m_imageryCategoryMap["时令"] = {
        "春", "夏", "秋", "冬", "正月", "二月", "四月", "六月"
    };

    m_imageryCategoryMap["山地"] = {
        "南山", "北山", "山", "冈", "陵", "丘"
    };

    m_imageryCategoryMap["水域"] = {
        "江汉", "汉水", "江", "河", "川", "泉", "水", "渊", "沼"
    };

    m_imageryCategoryMap["草木"] = {
        "草", "木", "薪", "卉", "花", "黍", "苗", "桑", "柳", "梅", "栗",
        "蕨", "薇", "杞", "桋", "莪", "蒿", "蔚", "谷", "蓝", "瓠叶"
    };

    m_imageryCategoryMap["鸟兽"] = {
        "黄鸟", "乌", "鸟", "雁", "豺虎", "豺", "虎", "兔", "豕", "鱼",
        "鲂", "鳣", "鲔", "牛", "马", "四牡", "鹑", "鸢", "熊", "罴"
    };

    m_imageryCategoryMap["车马征行"] = {
        "大车", "后车", "车", "辇", "舟", "船", "道", "周道", "东征", "征师", "归"
    };

    m_imageryCategoryMap["饮食器物"] = {
        "酒", "浆", "肴", "簋", "匕", "斗", "箕", "弓", "绳"
    };

    for (auto it = m_imageryCategoryMap.begin(); it != m_imageryCategoryMap.end(); ++it) {
        const QString& category = it.key();
        const QStringList& words = it.value();
        for (const QString& word : words) {
            m_wordToCategory[word] = category;
        }
    }
}

QStringList ImageryGraph::extractImageryWords(const QString& content) const
{
    QStringList allWords = m_wordToCategory.keys();

    std::sort(allWords.begin(), allWords.end(), [](const QString& a, const QString& b) {
        return a.length() > b.length();
        });

    QStringList result;
    for (const QString& word : allWords) {
        if (content.contains(word)) {
            result.append(word);
        }
    }
    result.removeDuplicates();
    return result;
}

QStringList ImageryGraph::mapWordsToCategories(const QStringList& words) const
{
    QStringList result;
    for (const QString& word : words) {
        if (m_wordToCategory.contains(word)) {
            result.append(m_wordToCategory[word]);
        }
    }
    result.removeDuplicates();
    return result;
}

QStringList ImageryGraph::intersectStringList(const QStringList& a, const QStringList& b) const
{
    QStringList result;
    for (const QString& item : a) {
        if (b.contains(item)) {
            result.append(item);
        }
    }
    result.removeDuplicates();
    return result;
}

int ImageryGraph::calculateWeight(const QStringList& wordsA,
    const QStringList& wordsB,
    const QStringList& categoriesA,
    const QStringList& categoriesB,
    QStringList& sharedWords,
    QStringList& sharedCategories) const
{
    sharedWords = intersectStringList(wordsA, wordsB);
    sharedCategories = intersectStringList(categoriesA, categoriesB);

    int weight = 0;

    // 1. 相同意象词，权重更高
    weight += sharedWords.size() * 3;

    // 2. 相同意象类别，权重较低
    // 注意：如果某类别已经因为相同词贡献过权重，这里仍保留类别加分，让“既同词又同类”的关联更强
    weight += sharedCategories.size() * 1;

    return weight;
}

void ImageryGraph::build(const QList<Poem>& poems)
{
    m_poems = poems;
    m_vertices.clear();
    m_adjList.clear();
    m_poemIdToIndex.clear();

    for (int i = 0; i < poems.size(); ++i) {
        GraphVertex v;
        v.poemId = poems[i].id;
        v.title = poems[i].title;

        QString fullContent = poems[i].content;
        v.imageryWords = extractImageryWords(fullContent);
        v.imageryCategories = mapWordsToCategories(v.imageryWords);

        m_vertices.append(v);
        m_poemIdToIndex[v.poemId] = i;
    }

    m_adjList.resize(m_vertices.size());

    for (int i = 0; i < m_vertices.size(); ++i) {
        for (int j = i + 1; j < m_vertices.size(); ++j) {
            QStringList sharedWords;
            QStringList sharedCategories;

            int weight = calculateWeight(
                m_vertices[i].imageryWords,
                m_vertices[j].imageryWords,
                m_vertices[i].imageryCategories,
                m_vertices[j].imageryCategories,
                sharedWords,
                sharedCategories
            );

            if (weight > 0) {
                GraphEdge e1;
                e1.to = j;
                e1.sharedWords = sharedWords;
                e1.sharedCategories = sharedCategories;
                e1.weight = weight;

                GraphEdge e2;
                e2.to = i;
                e2.sharedWords = sharedWords;
                e2.sharedCategories = sharedCategories;
                e2.weight = weight;

                m_adjList[i].append(e1);
                m_adjList[j].append(e2);
            }
        }
    }

    for (int i = 0; i < m_adjList.size(); ++i) {
        std::sort(m_adjList[i].begin(), m_adjList[i].end(),
            [this](const GraphEdge& a, const GraphEdge& b) {
                if (a.weight != b.weight)
                    return a.weight > b.weight;
                return m_vertices[a.to].title < m_vertices[b.to].title;
            });
    }
}

int ImageryGraph::vertexCount() const
{
    return m_vertices.size();
}

int ImageryGraph::indexOfPoem(int poemId) const
{
    return m_poemIdToIndex.contains(poemId) ? m_poemIdToIndex[poemId] : -1;
}

GraphVertex ImageryGraph::vertex(int index) const
{
    return m_vertices[index];
}

QList<GraphEdge> ImageryGraph::neighbors(int index) const
{
    if (index < 0 || index >= m_adjList.size()) {
        return {};
    }
    return m_adjList[index];
}

QStringList ImageryGraph::getImageryWordsOfPoem(const Poem& poem) const
{
    QStringList words = extractImageryWords(poem.content);
    words.removeDuplicates();
    return words;
}

QStringList ImageryGraph::getImageryCategoriesOfPoem(const Poem& poem) const
{
    QStringList words = extractImageryWords(poem.content);
    QStringList categories = mapWordsToCategories(words);
    categories.removeDuplicates();
    return categories;
}

void ImageryGraph::dfsHelper(int u, QVector<bool>& visited, QList<int>& order) const
{
    visited[u] = true;
    order.append(u);

    for (const GraphEdge& edge : m_adjList[u]) {
        if (!visited[edge.to]) {
            dfsHelper(edge.to, visited, order);
        }
    }
}

QList<int> ImageryGraph::dfs(int startIndex) const
{
    QList<int> order;
    if (startIndex < 0 || startIndex >= m_vertices.size()) {
        return order;
    }

    QVector<bool> visited(m_vertices.size(), false);
    dfsHelper(startIndex, visited, order);
    return order;
}

QList<int> ImageryGraph::bfs(int startIndex) const
{
    QList<int> order;
    if (startIndex < 0 || startIndex >= m_vertices.size()) {
        return order;
    }

    QVector<bool> visited(m_vertices.size(), false);
    QQueue<int> queue;

    visited[startIndex] = true;
    queue.enqueue(startIndex);

    while (!queue.isEmpty()) {
        int u = queue.dequeue();
        order.append(u);

        for (const GraphEdge& edge : m_adjList[u]) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                queue.enqueue(edge.to);
            }
        }
    }

    return order;
}

QList<int> ImageryGraph::findPoemsByCategory(const QString& category, int currentPoemId) const
{
    QList<int> result;
    int currentIndex = indexOfPoem(currentPoemId);

    for (int i = 0; i < m_vertices.size(); ++i) {
        if (m_vertices[i].poemId == currentPoemId) {
            continue;
        }

        if (m_vertices[i].imageryCategories.contains(category)) {
            result.append(i);
        }
    }

    std::sort(result.begin(), result.end(), [this, currentIndex](int a, int b) {
        int wa = 0;
        int wb = 0;

        if (currentIndex != -1) {
            for (const GraphEdge& edge : m_adjList[currentIndex]) {
                if (edge.to == a) wa = edge.weight;
                if (edge.to == b) wb = edge.weight;
            }
        }

        if (wa != wb)
            return wa > wb;
        return m_vertices[a].title < m_vertices[b].title;
        });

    return result;
}

QList<int> ImageryGraph::mostSimilarPoems(int poemId) const
{
    QList<int> result;
    int index = indexOfPoem(poemId);
    if (index == -1) {
        return result;
    }

    QList<GraphEdge> edges = neighbors(index);
    for (const GraphEdge& edge : edges) {
        result.append(edge.to);
    }
    return result;
}