#include "Calculator.h"

bool Calculator::checkParentheses(const std::string& expression){
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

std::vector<Token> Calculator::convertToPostfix(const std::vector<Token>& expression){
    Stack<Token> operatorStack;//用来存储运算符的栈
    std::vector<Token> postfix;//存储后缀表达式的向量

    for (const Token& token : expression) {
        if (!token.isOperator) {
            //如果是操作数，直接输出到后缀表达式
            postfix.push_back(token);
        }
        else if (token.value == "(") {
            //左括号压入栈
            operatorStack.push(token);
        }
        else if (token.value == ",") {
            //逗号时，弹出栈直到遇到左括号
            while (!operatorStack.isEmpty() && operatorStack.getTop().value != "(") {
                postfix.push_back(operatorStack.pop());
            }
            //operatorStack.pop();//不要弹出左括号！
        }
        else if (token.value == ")") {
            //右括号时，弹出栈直到遇到左括号
            while (!operatorStack.isEmpty() && operatorStack.getTop().value != "(") {
                postfix.push_back(operatorStack.pop());
            }
            operatorStack.pop();//弹出左括号
        }
        else if (token.isOperator) {
            // 遇到运算符，处理优先级
            while (!operatorStack.isEmpty() &&
                (getPriority(operatorStack.getTop().value) >= getPriority(token.value))) {
                postfix.push_back(operatorStack.pop());//优先级低则弹栈（等于按照从左往右也是低于）
            }
            operatorStack.push(token);//直到优先级高于栈顶
        }
    }

    //将剩余的操作符弹出栈
    while (!operatorStack.isEmpty()) {
        postfix.push_back(operatorStack.pop());
    }

    return postfix;
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
    //别返回函数内的引用啊:(
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
        if (isdigit(ch) || (ch == '.') || (ch == 'e')) {
            while (i < expression.size() && (isdigit(ch) || (ch == '.') || (ch == 'e'))) {//此处可能出现的非法运算符如.1和+.
                if (ch == 'e') {
                    token += expression[i];
                    i++;//因为e后面跟符号（+-），所以在这里多取一位
                    //waring: 这里会产生1e的错误表达式无法判断
                    token += expression[i];
                }
                else {
                    token += ch;  //拼接完整的数字
                }
                i++;
                ch = expression[i];
                expectOperator = true;  //下一步期望是运算符
            }
            //将操作数压入栈
            tokens.push_back({ token, false });
            token.clear();
        }

        //处理+-*/%^运算符
        if (isOperator(ch)) {
            //如果是负数开头
            if (ch == '-' && !expectOperator) {
                token += ch;  //将负号看作数字的一部分
            }
            else {
                //if (!token.empty()) {
                //    tokens.push_back({ token, false });//处理在此之前的数字
                //    token.clear();
                //}
                tokens.push_back({ std::string(1, ch), true });  //当前运算符，一定要将一个字符转为字符串！他们之间不能隐式转换！
                token.clear();
                expectOperator = false;  //改变期望
            }
        }
        //处理逗号,逗号优先级与右括号一样，而且逗号后面的期望是操作数
        else if (ch == ',') {
            tokens.push_back({ std::string(1, ch), true });  
            token.clear();
            expectOperator = false;
        }
        //处理括号
        else if (ch == '(' || ch == ')') {
            tokens.push_back({ std::string(1, ch), true });  //当前括号
            token.clear();
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


    return tokens;
}

float Calculator::caculate(const std::string& expression) {
    //将表达式转为token
    std::vector<Token> tokens = tokenizeExpression(expression);

    //将token转为后缀表达式
    std::vector<Token> postfix = convertToPostfix(tokens);

    //用栈储存运算结果
    Stack<float> resultStack;

    for (const Token& token : postfix) {
        if (!token.isOperator) {
            //操作数则直接压入栈，（用stof将字符串转为浮点型）
            //warning:这里可能会出现类型转换错误的报错！
            char* end;
            const char* value = token.value.c_str();
            float floatNumber = std::strtof(value,&end);
            //通过end不为空来判断string没有正确被转为float
            if (*end != '\0') {
                throw std::runtime_error("输入错误的小数！");
            }
            resultStack.push(floatNumber);
        }
        else if (isOperator(token.value)) {
            //是操作符则取两个操作数（二元操作符）
            float rightNum = resultStack.pop();//左操作数
            float leftNum = resultStack.pop();//右操作数
            
            //运算
            if (token.value == "+") {
                resultStack.push(leftNum + rightNum);
            }
            else if (token.value == "-") {
                resultStack.push(leftNum - rightNum);
            }
            else if (token.value == "*") {
                resultStack.push(leftNum * rightNum);
            }
            else if (token.value == "/") {
                if (rightNum == 0) {
                    throw std::runtime_error("除数不能为0！");
                }
                resultStack.push(leftNum / rightNum);
            }
            else if (token.value == "%") {
                //要先强制转成整数求！
                resultStack.push(static_cast<int>(leftNum) % static_cast<int>(rightNum));
            }
            else if (token.value == "^") {
                resultStack.push(std::pow(leftNum, rightNum));
            }
        }
        else if (isFunctionStart(token.value[0])) {
            //处理函数运算符
            float operand = resultStack.pop();

            if (token.value == "sqrt") {
                resultStack.push(std::sqrt(operand));
            }
            else if (token.value == "pow") {
                float exponent = resultStack.pop();
                resultStack.push(std::pow(operand, exponent));
            }
            else if (token.value == "sin") {
                resultStack.push(std::sin(operand));
            }
            else if (token.value == "cos") {
                resultStack.push(std::cos(operand));
            }
            else if (token.value == "tan") {
                resultStack.push(std::tan(operand));
            }
        }
    }

    return resultStack.pop();
}

std::string Calculator::expections()
{
    try {
        caculate(_expression);
    }
    catch(const std::runtime_error& e){
        return e.what();
    }
    catch(...){

        return "表达式不合法！";
    }

}
