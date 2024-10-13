#include "Calculator.h"

bool Calculator::checkParentheses(const std::string& expression)
{
    Stack<char> parenStack;//用于存储左括号的栈

    for (char ch : expression) {
        if (ch == '(') {
            parenStack.push(ch);//遇到左括号，压入栈
        }
        else if (ch == ')') {
            //遇到右括号时，检查栈是否为空
            if (parenStack.isEmpty()) {
                return false;  //栈为空，说明多了右括号，括号不匹配
            }
            parenStack.pop(); //从栈中弹出左括号
        }
    }

    //如果遍历完表达式后，栈不为空，则说明多了左括号，括号不匹配
    return parenStack.isEmpty();
}
