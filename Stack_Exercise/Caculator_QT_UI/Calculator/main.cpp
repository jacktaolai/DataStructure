#include "caculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Caculator w;
    w.setWindowTitle("计算器");
    w.show();
    return a.exec();
}
