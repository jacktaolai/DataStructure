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

std::vector<Token>& Calculator::convertToPostfix(const std::vector<Token>& expression)
{
    Stack<std::string> operatorStack;  // 用来存储运算符的栈
    std::vector<Token> postfix;         // 存储后缀表达式的向量

    for (const auto& token : expression) {
        if (!token.isOperator) {
            // 如果是操作数，直接输出到后缀表达式
            postfix.push_back(token);
        }
        else if (token.value == "(") {
            // 左括号压入栈
            operatorStack.push(token.value);
        }
        else if (token.value == ")") {
            // 右括号时，弹出栈直到遇到左括号
            while (!operatorStack.isEmpty() && operatorStack.getTop() != "(") {
                postfix.push_back({ operatorStack.pop(), true });
            }
            operatorStack.pop();  // 弹出左括号
        }
        else if (isOperator(token.value)) {
            // 遇到运算符，处理优先级
            while (!operatorStack.isEmpty() &&
                getPriority(operatorStack.getTop()) >= getPriority(token.value)) {
                postfix.push_back({ operatorStack.pop(), true });
            }
            operatorStack.push(token.value);
        }
    }

    // 将剩余的操作符弹出栈
    while (!operatorStack.isEmpty()) {
        postfix.push_back({ operatorStack.pop(), true });
    }

    return postfix;  // 返回后缀表达式
}



int Calculator::getPriority(std::string anOperator)
{    /***************所有的操作符**********************
        +---------------- + ------------------ +
        | 操作符 / 函数    | 优先级              |
        +---------------- + ------------------ +
        | sqrt            | 3                  |
        | pow             | 3                  |
        | sin             | 3                  |
        | cos             | 3                  |
        | tan             | 3                  |
        | ^               | 2                  |
        | *               | 2                  |
        | /               | 2                  |
        | %               | 2                  |
        | +               | 1                  |
        | -               | 1                  |
        +---------------- + ------------------ +
    ***************所有的操作符**********************/

    //获取操作符的优先级
    if (anOperator == "sqrt" || anOperator == "pow" || anOperator == "sin" || 
        anOperator == "cos" || anOperator == "tan") {
        return 3;//函数优先级为 3
    }
    else if (anOperator == "^") {
        return 2;//幂运算的优先级为 2
    }
    else if (anOperator == "*" || anOperator == "/" || anOperator == "%") {
        return 2;// */%的优先级为 2
    }
    else if (anOperator == "+" || anOperator == "-") {
        return 1;//加减的优先级为 1
    }
    return 0;//不能识别的运算符


}

std::vector<Token> Calculator::tokenizeExpression(const std::string& expression){
    std::vector<Token> tokens;
    std::string token;
    bool expectOperator = false;//用于区分负号和减号（因为除操作数外运算符和操作数交替出现）

    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];

        ////跳过空格）
        //if (isspace(ch)) {
        //    continue;
        //}

        //处理数字（包括小数点）
        if (isdigit(ch) || (ch == '.' )) {//此处可能出现的非法运算符如.1和+.
            token += ch;  //拼接完整的数字
            expectOperator = true;  //下一步期望是运算符
        }
        //处理+-*/%^运算符
        else if (isOperator(ch)) {
            //如果是负数开头
            if (ch == '-' && !expectOperator) {
                token += ch;  //将负号看作数字的一部分
            }
            else {
                if (!token.empty()) {
                    tokens.push_back({ token, false });  //处理在此之前的数字
                    token.clear();
                }
                tokens.push_back({ std::string(1, ch), true });  //当前运算符，一定要将一个字符转为字符串！他们之间不能隐式转换！
                expectOperator = false;  //改变期望
            }
        }
        //处理括号
        else if (ch == '(' || ch == ')') {
            if (!token.empty()) {
                tokens.push_back({ token, false });  //把之前的数字放入tokens
                token.clear();
            }
            tokens.push_back({ std::string(1, ch), true });  //当前括号
            expectOperator = (ch == ')');  //右括号后面期望是运算符（括号不改变期望）
        }
        //处理函数运算符sin cos tan sqrt pow
        else if (isFunctionStart(ch)) {
            token += ch;
            while (i + 1 < expression.size() && isalpha(expression[i + 1])) {
                token += expression[++i];
                if (token == "sin" || token == "cos" || token == "tan" || token == "sqrt" || token == "pow") {
                    break;//一旦得到完整函数名即退出
                }
                
            }
            tokens.push_back({ token, true });  //将函数名作为运算符存入 tokens
            token.clear();
        }
    }

    //处理最后的操作数（如果有）
    if (!token.empty()) {
        tokens.push_back({ token, false });
    }

    return tokens;
}