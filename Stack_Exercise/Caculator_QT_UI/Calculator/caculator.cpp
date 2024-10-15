#include "caculator.h"
#include "ui_caculator.h"
#include "Calculator.h"

Caculator::Caculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Caculator)
{
    ui->setupUi(this);
    //链接所有按钮到槽函数（除了等于和删除）
    connect(ui->Button_0, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_1, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_2, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_3, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_4, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_5, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_6, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_7, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_8, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_9, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_comma, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_cos, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_divided, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_left, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_minus, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_mod, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_plus, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_point, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_pow, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_pow_2, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_right, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_sqrt, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_tan, &QPushButton::clicked, this, &Caculator::clickInput);
    connect(ui->Button_times, &QPushButton::clicked, this, &Caculator::clickInput);

    //链接删除按钮
    connect(ui->Button_delete, &QPushButton::clicked, this, &Caculator::deleteAll);

    //链接等于计算结果
    connect(ui->Button_equal, &QPushButton::clicked, this, &Caculator::equal);


}


Caculator::~Caculator()
{
    delete ui;
}

//点击输入按钮
void Caculator::clickInput(){
    //获取信号发射对象
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    //勉为其难用qstring吧
    QString buttonText=button->text();
    ui->display->setText(ui->display->toPlainText() + buttonText);
    //将计算表达式存入类的私有成员方便后续调用计算器类
    expression+=buttonText.toStdString();
}

//删除所有元素
void Caculator::deleteAll(){

    ui->display->setText("");
    expression="";
}

//计算结果
void Caculator::equal(){
    Calculator caculate(expression);
    if (caculate.expections()==""){
        float result=caculate.caculate(expression);
        QString resultString=QString::number(result);
        ui->result->setText(resultString);
        ui->display->setText("");
        expression="";
    }
    else{
        QString resultString=QString::fromStdString(caculate.expections());
        ui->result->setText(resultString);
        ui->display->setText("");
        expression="";
    }
}
