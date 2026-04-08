#include <QApplication>
#include "PoemManager.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // 加载诗经数据
    PoemManager::instance().loadFromJson("resource/诗经.json");

    MainWindow w;
    w.show();

    return a.exec();
}