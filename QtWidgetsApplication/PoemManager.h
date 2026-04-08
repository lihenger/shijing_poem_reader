#pragma once
#include <QList>
#include <QString>
#include "Poem.h"

class PoemManager {
public:
    static PoemManager& instance();

    bool loadFromJson(const QString& filePath);
    const QList<Poem>& getAllPoems() const;
    void clear();

private:
    PoemManager() = default;
    ~PoemManager() = default;
    PoemManager(const PoemManager&) = delete;
    PoemManager& operator=(const PoemManager&) = delete;

    QList<Poem> m_poems;
};