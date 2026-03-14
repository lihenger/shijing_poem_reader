#include "Poem.h"
#include "json.hpp"
#include <Qlist>
#include <QString>

// 单例模式，保证只加载一次文件，其他拓展的数据结构使用指针。
// 整个程序使用这一份数据。

class PoemManager {
public:
	static PoemManager instance();	// 获取全局唯一实例
	bool loadFromJson();	// 用于从文件中加载诗歌
	PoemManager(const PoemManager&) = delete;
	void operator=(const PoemManager&) = delete;

	const QList<Poem>& getAllPoems() const;

	void clear();


};