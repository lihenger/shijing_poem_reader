#include <QApplication>
#include <QFont>
#include "PoemManager.h"
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QFont font("Microsoft YaHei", 10);
    a.setFont(font);

    if (!PoemManager::instance().loadFromJson("resource/诗经.json")) {
        qDebug() << "Failed to load poems data";
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}