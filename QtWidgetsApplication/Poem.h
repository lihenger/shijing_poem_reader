#pragma once
#include<QString>

//诗经结构体，项目的数据基础

struct Poem{
	int id;
	QString title;		//标题
	QString content;	//内容
	QString chapter;	//类型（风、雅、颂）
	QString section;	

	Poem() = default;

	Poem(int id,QString title,QString content,QString chapter,QString section)
		:id(id),title(title),content(content), chapter(chapter), section(section)
	{ }

};