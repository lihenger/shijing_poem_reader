#include "PoemManager.h"
#include <QFile>
#include <QDebug>
#include "json.hpp"

using json = nlohmann::json;

//获取单例
PoemManager& PoemManager::instance() {
	static PoemManager manager;
	return manager;
}

bool PoemManager::loadFromJson(const QString filePath) {
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "打开文件失败" << filePath;
		return false;
	}

	QByteArray data = file.readAll();
	file.close();

	json root = json::parse(data.toStdString(), nullptr, false);
	if (root.is_discarded())
	{
		qDebug() << "JSON 解析失败";
		return false;
	}

	m_poems.clear();

	for (const auto& item : root)
	{
		QString id = QString::fromStdString(item["id"].get<std::string>());
		QString title = QString::fromStdString(item["title"].get<std::string>());
		QString content = QString::fromStdString(item["content"].get<std::string>());
		QString chapter = QString::fromStdString(item["chapter"].get<std::string>());
		QString section = QString::fromStdString(item["section"].get<std::string>());

		Poem poem(id, title, content, chapter, section);
		m_poems.append(poem);
	}

	qDebug() << "成功加载诗歌数量：" << m_poems.size();
	return true;
}

const QList<Poem>& PoemManager::getAllPoems() const {
	return m_poems;
}

void PoemManager::clear() {
	m_poems.clear();
}



