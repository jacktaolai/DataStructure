#ifndef CACULATOR_H
#define CACULATOR_H

#include <QMainWindow>
#include <string>


QT_BEGIN_NAMESPACE
namespace Ui {
class Caculator;
}
QT_END_NAMESPACE

class Caculator : public QMainWindow
{
    Q_OBJECT

public:
    Caculator(QWidget *parent = nullptr);
    ~Caculator();

private:
    Ui::Caculator *ui;
    std::string expression;
    float result;

private slots:
    void clickInput(); //槽函数用于点击输入
    void deleteAll();  //槽函数用于清空输入框
    void equal();//槽函数用于计算结果
};
#endif // CACULATOR_H
