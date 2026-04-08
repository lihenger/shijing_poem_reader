#ifndef IMAGERYGRAPH_H
#define IMAGERYGRAPH_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include "Poem.h"

struct GraphVertex
{
    int poemId;
    QString title;

    QStringList imageryWords;       // 原始意象词
    QStringList imageryCategories;  // 归类后的意象类别
};

struct GraphEdge
{
    int to;
    QStringList sharedWords;        // 相同意象词
    QStringList sharedCategories;   // 相同意象类别
    int weight;                     // 综合权重
};

class ImageryGraph
{
public:
    ImageryGraph();

    void build(const QList<Poem>& poems);

    int vertexCount() const;
    int indexOfPoem(int poemId) const;
    GraphVertex vertex(int index) const;
    QList<GraphEdge> neighbors(int index) const;

    QStringList getImageryWordsOfPoem(const Poem& poem) const;
    QStringList getImageryCategoriesOfPoem(const Poem& poem) const;

    QList<int> dfs(int startIndex) const;
    QList<int> bfs(int startIndex) const;

    QList<int> findPoemsByCategory(const QString& category, int currentPoemId = -1) const;
    QList<int> mostSimilarPoems(int poemId) const;

private:
    QList<Poem> m_poems;
    QList<GraphVertex> m_vertices;
    QList<QList<GraphEdge>> m_adjList;
    QMap<int, int> m_poemIdToIndex;

    QMap<QString, QStringList> m_imageryCategoryMap; // 类别 -> 词表
    QMap<QString, QString> m_wordToCategory;         // 词 -> 类别

    void initImageryDictionary();

    QStringList extractImageryWords(const QString& content) const;
    QStringList mapWordsToCategories(const QStringList& words) const;

    QStringList intersectStringList(const QStringList& a, const QStringList& b) const;
    int calculateWeight(const QStringList& wordsA,
        const QStringList& wordsB,
        const QStringList& categoriesA,
        const QStringList& categoriesB,
        QStringList& sharedWords,
        QStringList& sharedCategories) const;

    void dfsHelper(int u, QVector<bool>& visited, QList<int>& order) const;
};

#endif // IMAGERYGRAPH_H