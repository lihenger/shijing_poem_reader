#include <QApplication>
#include <QFont>
#include "PoemManager.h"
#include "TranslationManager.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QFont font("Microsoft YaHei", 10);
    a.setFont(font);

    // 加载诗歌数据
    if (!PoemManager::instance().loadFromJson("resource/诗经.json")) {
        qDebug() << "Failed to load poems data";
        return -1;
    }

    // 加载翻译数据
    if (!TranslationManager::instance().loadFromJson("resource/translations.json")) {
        qDebug() << "Failed to load translations data";
        // 翻译数据不是必需的，即使加载失败也继续运行
    }

    MainWindow w;
    w.show();

    return a.exec();
}