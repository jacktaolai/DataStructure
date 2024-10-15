#include "caculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Caculator w;
    w.show();
    return a.exec();
}
