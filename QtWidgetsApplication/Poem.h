#pragma once
#include <QString>

// 诗经结构体，项目的数据基础
struct Poem {
    int id;
    QString title;      // 标题
    QString content;    // 内容
    QString chapter;    // 类型（风、雅、颂）
    QString section;    // 小节

    Poem() = default;

    Poem(int id, const QString& title, const QString& content,
        const QString& chapter, const QString& section)
        : id(id), title(title), content(content), chapter(chapter), section(section)
    {
    }
};