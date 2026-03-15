#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "PoemManager.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	bool ok = PoemManager::instance().loadFromJson("resource\\诗经.json");

	qDebug() << "加载成功" << ok;

	auto& poems = PoemManager::instance().getAllPoems();

	// 4. 输出数量 + 第一首诗
	qDebug() << "总诗歌数量：" << poems.size();
	if (!poems.isEmpty()) {
		qDebug() << "第一首标题：" << poems[0].title;
		qDebug() << "第一首内容：" << poems[0].content;
		qDebug() << poems[0].chapter;
		qDebug() << poems[0].id;
		qDebug() << poems[0].section;
	}

	return a.exec();
}



//int main(int argc, char* argv[])
//{
//    QApplication a(argc, argv);
//
//    // 1. 窗口
//    QMainWindow w;
//    w.setWindowTitle("我的第一个Qt程序");
//    w.resize(600, 400);
//
//    // 2. 文字
//    QLabel* label = new QLabel(&w);
//    label->setText("等待点击...");
//    label->setAlignment(Qt::AlignCenter);
//    label->setGeometry(0, 100, 600, 100);
//
//    // 3. 按钮
//    QPushButton* btn = new QPushButton("点击显示诗经", &w);
//    btn->setGeometry(200, 300, 200, 50);
//
//    // 4. 点击事件
//    QObject::connect(btn, &QPushButton::clicked, [&]() {
//        label->setText("关关雎鸠，在河之洲\n窈窕淑女，君子好逑");
//        });
//
//    w.show();
//    return a.exec();
//}