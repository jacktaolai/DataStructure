#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 创建主窗口
    MainWindow mainwindow;

    // 显示主窗口
    mainwindow.show();

    // 运行应用程序
    return app.exec();
}
