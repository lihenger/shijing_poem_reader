#include "StatisticsData.h"
#include "PoemManager.h"
#include "ImageryGraph.h"

// 确保构造函数有实现
StatisticsData::StatisticsData()
    : m_totalPoems(0)
    , m_totalImageryWords(0)
{
    // 初始化意象类别分组
    m_imageryByCategory["天象"] = { "天", "日", "月", "星", "汉", "牵牛", "织女" };
    m_imageryByCategory["气候"] = { "风", "雨", "雪", "霜", "露", "云", "阴雨" };
    m_imageryByCategory["时令"] = { "春", "夏", "秋", "冬", "正月", "二月", "四月", "六月" };
    m_imageryByCategory["山地"] = { "南山", "北山", "山", "冈", "陵", "丘" };
    m_imageryByCategory["水域"] = { "江汉", "汉水", "江", "河", "川", "泉", "水", "渊", "沼" };
    m_imageryByCategory["草木"] = { "草", "木", "薪", "卉", "花", "黍", "苗", "桑", "柳", "梅", "栗", "蕨", "薇", "杞", "桋", "莪", "蒿", "蔚", "谷", "蓝", "瓠叶" };
    m_imageryByCategory["鸟兽"] = { "黄鸟", "乌", "鸟", "雁", "豺虎", "豺", "虎", "兔", "豕", "鱼", "鲂", "鳣", "鲔", "牛", "马", "四牡", "鹑", "鸢", "熊", "罴" };
    m_imageryByCategory["车马征行"] = { "大车", "后车", "车", "辇", "舟", "船", "道", "周道", "东征", "征师", "归" };
    m_imageryByCategory["饮食器物"] = { "酒", "浆", "肴", "簋", "匕", "斗", "箕", "弓", "绳" };
}

void StatisticsData::calculateWordFrequency(const QList<int>& historyIndexes)
{
    clear();

    const auto& poems = PoemManager::instance().getAllPoems();
    ImageryGraph graph;
    graph.build(poems);

    m_totalPoems = historyIndexes.size();

    // 初始化热力图数据结构
    for (auto it = m_imageryByCategory.begin(); it != m_imageryByCategory.end(); ++it) {
        for (const QString& word : it.value()) {
            m_heatmapData[word] = QVector<int>(m_totalPoems, 0);
        }
    }

    // 遍历历史诗歌
    for (int pos = 0; pos < historyIndexes.size(); ++pos) {
        int poemIdx = historyIndexes[pos];
        if (poemIdx < 0 || poemIdx >= poems.size()) continue;

        const Poem& poem = poems[poemIdx];
        m_poemTitles.append(poem.title);

        // 提取意象词
        QStringList words = graph.getImageryWordsOfPoem(poem);
        words.removeDuplicates();

        for (const QString& word : words) {
            // 词频统计
            m_wordFrequency[word]++;
            m_totalImageryWords++;

            // 热力图数据
            if (m_heatmapData.contains(word)) {
                m_heatmapData[word][pos]++;
            }
        }
    }
}

void StatisticsData::clear()
{
    m_wordFrequency.clear();
    m_heatmapData.clear();
    m_poemTitles.clear();
    m_totalPoems = 0;
    m_totalImageryWords = 0;
}