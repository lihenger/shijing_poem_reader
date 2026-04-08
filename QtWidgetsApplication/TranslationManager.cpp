// TranslationManager.cpp
#include "TranslationManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

TranslationManager& TranslationManager::instance() {
    static TranslationManager manager;
    return manager;
}

bool TranslationManager::loadFromJson(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开翻译文件:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qDebug() << "翻译文件JSON解析失败";
        return false;
    }

    m_translations.clear();

    QJsonArray array = doc.array();
    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        TranslationData trans;
        trans.id = obj["id"].toInt();
        trans.title = obj["title"].toString();
        trans.modernTranslation = obj["modern_translation"].toString();
        trans.appreciation = obj["appreciation"].toString();

        m_translations[trans.id] = trans;
    }

    qDebug() << "成功加载翻译数据，共" << m_translations.size() << "首";
    return true;
}

QString TranslationManager::getTranslation(int poemId) const {
    if (m_translations.contains(poemId)) {
        return m_translations[poemId].modernTranslation;
    }
    return QString();
}

QString TranslationManager::getAppreciation(int poemId) const {
    if (m_translations.contains(poemId)) {
        return m_translations[poemId].appreciation;
    }
    return QString();
}

bool TranslationManager::hasTranslation(int poemId) const {
    return m_translations.contains(poemId);
}

void TranslationManager::clear() {
    m_translations.clear();
}