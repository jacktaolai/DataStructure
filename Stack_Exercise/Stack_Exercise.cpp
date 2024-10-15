// Stack_Exercise.cpp 
////author: 未名湖居士
//e-mail: jacktaolai@foxmail.com
//Style: Google Style Guide
//Licence: MIT
//version: 0.1


#include <iostream>
#include "Stack.h"
#include "RearrangingRailroadCars.h"
#include "Pixel_Group.h"
#include "Calculator.h"

int main() {
    // 创建一个整数类型的链式栈
    Stack<int> stack;

    // 测试空栈
    //std::cout << "Stack is empty: " << std::boolalpha << stack.isEmpty() << std::endl;

    //// 测试压栈
    //std::cout << "Pushing 10, 20, 30 into the stack." << std::endl;
    //stack.push(10);
    //stack.push(20);
    //stack.push(30);

    //// 检查栈是否为空
    //std::cout << "Stack is empty: " << std::boolalpha << stack.isEmpty() << std::endl;

    //// 测试弹栈并输出
    //std::cout << "Popped value: " << stack.pop() << std::endl; // 应输出30
    //std::cout << "Popped value: " << stack.pop() << std::endl; // 应输出20
    //std::cout << "Popped value: " << stack.pop() << std::endl; // 应输出10

    //// 再次检查栈是否为空
    //std::cout << "Stack is empty: " << std::boolalpha << stack.isEmpty() << std::endl;

    //int cars1[] = { 3, 5, 8, 1, 7, 4, 2, 9, 6 }; // 输入序列

    //rearrangeCars(cars1, 9); // 调用重新排列函数

    //int cars2[] = { 3, 4, 6, 1, 11, 13, 5, 7, 19, 8, 12, 2, 9, 16, 10, 14, 17, 18, 20, 15 }; // 输入序列
    //rearrangeCars(cars2, 20); // 调用重新排列函数

 
    //string inputFile = "二值图像分组-测试数据.txt";   // 输入文件
    //string outputFile = "labeled_image.txt";  // 输出文件

    //PixelGrouper grouper;
    //grouper.loadImage(inputFile);  // 读取图像
    //grouper.groupPixels(); // 分组像元
    //grouper.writeLabels(outputFile);  // 输出结果

    //cout << "像元分组完成，结果已输出到 " << outputFile << endl;
    //Calculator test("((()))");
    //cout<<test.checkParentheses("()");
    //std::string expression;

    //// 输入表达式
    //std::cout << "请输入表达式: ";
    //std::getline(std::cin, expression);

    //// 拆解表达式为操作数和运算符
    //std::vector<Token> tokens = test.tokenizeExpression(expression);

    //// 输出结果
    //std::cout << "分解结果:\n";
    //for (const Token& token : tokens) {
    //    std::cout << (token.isOperator ? "运算符: " : "操作数: ") << token.value << std::endl;
    //}
    //int a = -3 + 1;
    Calculator test2("((()))");
    // 测试表达式
    std::string expression = "3+pow(4-1*2,2)";

    // 将表达式转换为Token序列
    std::vector<Token> tokenizedExpression = test2.tokenizeExpression(expression);

    // 输出分词结果
    std::cout << "Tokenized Expression: ";
    test2.printTokens(tokenizedExpression);

    // 将中缀表达式转换为后缀表达式
    std::vector<Token> postfixExpression = test2.convertToPostfix(tokenizedExpression);

    // 输出后缀表达式
    std::cout << "Postfix Expression: ";
    test2.printTokens(postfixExpression);
    return 0;
}

