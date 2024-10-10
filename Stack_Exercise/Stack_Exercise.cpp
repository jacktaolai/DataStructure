// Stack_Exercise.cpp 
////author: 未名湖居士
//e-mail: jacktaolai@foxmail.com
//Style: Google Style Guide
//Licence: MIT
//version: 0.1


#include <iostream>
#include "Stack.h"
#include "RadialControllerInterop.h"


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

    return 0;
}

