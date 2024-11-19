#include <QApplication>
#include"inputWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    BinaryTreeWindow window;
    window.show();

    return app.exec();
}


