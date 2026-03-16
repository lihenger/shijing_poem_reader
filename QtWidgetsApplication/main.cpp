#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "PoemManager.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	PoemManager::instance().loadFromJson("resource\\诗经.json");

	MainWindow w;
	w.show();

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