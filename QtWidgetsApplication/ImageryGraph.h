#pragma once
#include <QList>
#include <QString>
#include <QMap>
#include "Poem.h"

struct GraphVertex {
    int poemId;
    QString title;
    QStringList imageryWords;
    QStringList imageryCategories;
};

struct GraphEdge {
    int to;
    int weight;
    QStringList sharedWords;
    QStringList sharedCategories;
};

class ImageryGraph {
public:
    ImageryGraph();

    void build(const QList<Poem>& poems);
    int vertexCount() const;
    int indexOfPoem(int poemId) const;
    GraphVertex vertex(int index) const;
    QList<GraphEdge> neighbors(int index) const;

    QStringList getImageryWordsOfPoem(const Poem& poem) const;
    QStringList getImageryCategoriesOfPoem(const Poem& poem) const;

    QList<int> findPoemsByCategory(const QString& category, int currentPoemId) const;
    QList<int> mostSimilarPoems(int poemId) const;

    QList<int> dfs(int startIndex) const;
    QList<int> bfs(int startIndex) const;

private:
    void initImageryDictionary();
    QStringList extractImageryWords(const QString& content) const;
    QStringList mapWordsToCategories(const QStringList& words) const;
    QStringList intersectStringList(const QStringList& a, const QStringList& b) const;
    int calculateWeight(const QStringList& wordsA, const QStringList& wordsB,
        const QStringList& categoriesA, const QStringList& categoriesB,
        QStringList& sharedWords, QStringList& sharedCategories) const;
    void dfsHelper(int u, QVector<bool>& visited, QList<int>& order) const;

    QList<GraphVertex> m_vertices;
    QVector<QList<GraphEdge>> m_adjList;
    QMap<int, int> m_poemIdToIndex;

    QMap<QString, QStringList> m_imageryCategoryMap;
    QMap<QString, QString> m_wordToCategory;
};