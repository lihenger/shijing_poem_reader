#pragma once
#include <QString>

struct Poem {
    int id;
    QString title;
    QString content;
    QString chapter;
    QString section;

    Poem() = default;

    Poem(int id, const QString& title, const QString& content,
        const QString& chapter, const QString& section)
        : id(id), title(title), content(content), chapter(chapter), section(section)
    {
    }
};