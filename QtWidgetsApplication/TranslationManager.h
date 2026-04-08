// TranslationManager.h
#pragma once
#include <QMap>
#include <QString>

struct TranslationData {
    int id;
    QString title;
    QString modernTranslation;
    QString appreciation;
};

class TranslationManager {
public:
    static TranslationManager& instance();

    bool loadFromJson(const QString& filePath);

    QString getTranslation(int poemId) const;
    QString getAppreciation(int poemId) const;
    bool hasTranslation(int poemId) const;
    int getTranslationCount() const { return m_translations.size(); }

    void clear();

private:
    TranslationManager() = default;
    ~TranslationManager() = default;
    TranslationManager(const TranslationManager&) = delete;
    TranslationManager& operator=(const TranslationManager&) = delete;

    QMap<int, TranslationData> m_translations;
};