#pragma once
#include <QMap>
#include <QList>
#include <QString>
#include <QVector>
#include <QPair>

class StatisticsData {
public:
    StatisticsData();  // 确保这个构造函数存在

    // 基于历史诗歌索引统计词频
    void calculateWordFrequency(const QList<int>& historyIndexes);

    // 获取词频数据（用于词云）
    QMap<QString, int> getWordFrequency() const { return m_wordFrequency; }

    // 获取热力图数据（意象词 × 诗歌位置）
    QMap<QString, QVector<int>> getHeatmapData() const { return m_heatmapData; }

    // 获取诗歌标题列表
    QStringList getPoemTitles() const { return m_poemTitles; }

    // 获取意象词列表（按类别分组）
    QMap<QString, QStringList> getImageryByCategory() const { return m_imageryByCategory; }

    // 获取统计信息
    int getTotalPoems() const { return m_totalPoems; }
    int getTotalImageryWords() const { return m_totalImageryWords; }

    // 清空数据
    void clear();

private:
    QMap<QString, int> m_wordFrequency;           // 词频统计
    QMap<QString, QVector<int>> m_heatmapData;    // 热力图数据
    QStringList m_poemTitles;                      // 诗歌标题列表
    QMap<QString, QStringList> m_imageryByCategory; // 意象按类别分组
    int m_totalPoems;
    int m_totalImageryWords;
};