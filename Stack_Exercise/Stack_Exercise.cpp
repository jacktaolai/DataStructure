// Stack_Exercise.cpp 
////author: 未名湖居士
//e-mail: jacktaolai@foxmail.com
//Style: Google Style Guide
//Licence: MIT
//version: 0.1


#include <iostream>
#include "Stack.h"
#include "RearrangingRailroadCars.h"


int main() {
    // 创建一个整数类型的链式栈
    Stack<int> stack;

    // 测试空栈
    std::cout << "Stack is empty: " << std::boolalpha << stack.isEmpty() << std::endl;

    // 测试压栈
    std::cout << "Pushing 10, 20, 30 into the stack." << std::endl;
    stack.push(10);
    stack.push(20);
    stack.push(30);

    // 检查栈是否为空
    std::cout << "Stack is empty: " << std::boolalpha << stack.isEmpty() << std::endl;

    // 测试弹栈并输出
    std::cout << "Popped value: " << stack.pop() << std::endl; // 应输出30
    std::cout << "Popped value: " << stack.pop() << std::endl; // 应输出20
    std::cout << "Popped value: " << stack.pop() << std::endl; // 应输出10

    // 再次检查栈是否为空
    std::cout << "Stack is empty: " << std::boolalpha << stack.isEmpty() << std::endl;

    int cars1[] = { 3, 5, 8, 1, 7, 4, 2, 9, 6 }; // 输入序列

    rearrangeCars(cars1, 9); // 调用重新排列函数

    int cars2[] = { 3, 4, 6, 1, 11, 13, 5, 7, 19, 8, 12, 2, 9, 16, 10, 14, 17, 18, 20, 15 }; // 输入序列
    rearrangeCars(cars2, 20); // 调用重新排列函数

    return 0;
}

