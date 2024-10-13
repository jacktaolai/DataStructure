#include "Calculator.h"

bool Calculator::checkParentheses(const std::string& expression)
{
    Stack<char> parenStack;//���ڴ洢�����ŵ�ջ

    for (char ch : expression) {
        if (ch == '(') {
            parenStack.push(ch);//���������ţ�ѹ��ջ
        }
        else if (ch == ')') {
            //����������ʱ�����ջ�Ƿ�Ϊ��
            if (parenStack.isEmpty()) {
                return false;  //ջΪ�գ�˵�����������ţ����Ų�ƥ��
            }
            parenStack.pop(); //��ջ�е���������
        }
    }

    //�����������ʽ��ջ��Ϊ�գ���˵�����������ţ����Ų�ƥ��
    return parenStack.isEmpty();
}

std::vector<Token>& Calculator::convertToPostfix(const std::vector<Token>& expression)
{
    Stack<std::string> operatorStack;  // �����洢�������ջ
    std::vector<Token> postfix;         // �洢��׺���ʽ������

    for (const auto& token : expression) {
        if (!token.isOperator) {
            // ����ǲ�������ֱ���������׺���ʽ
            postfix.push_back(token);
        }
        else if (token.value == "(") {
            // ������ѹ��ջ
            operatorStack.push(token.value);
        }
        else if (token.value == ")") {
            // ������ʱ������ջֱ������������
            while (!operatorStack.isEmpty() && operatorStack.getTop() != "(") {
                postfix.push_back({ operatorStack.pop(), true });
            }
            operatorStack.pop();  // ����������
        }
        else if (isOperator(token.value)) {
            // ������������������ȼ�
            while (!operatorStack.isEmpty() &&
                getPriority(operatorStack.getTop()) >= getPriority(token.value)) {
                postfix.push_back({ operatorStack.pop(), true });
            }
            operatorStack.push(token.value);
        }
    }

    // ��ʣ��Ĳ���������ջ
    while (!operatorStack.isEmpty()) {
        postfix.push_back({ operatorStack.pop(), true });
    }

    return postfix;  // ���غ�׺���ʽ
}



int Calculator::getPriority(std::string anOperator)
{    /***************���еĲ�����**********************
        +---------------- + ------------------ +
        | ������ / ����    | ���ȼ�              |
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
    ***************���еĲ�����**********************/

    //��ȡ�����������ȼ�
    if (anOperator == "sqrt" || anOperator == "pow" || anOperator == "sin" || 
        anOperator == "cos" || anOperator == "tan") {
        return 3;//�������ȼ�Ϊ 3
    }
    else if (anOperator == "^") {
        return 2;//����������ȼ�Ϊ 2
    }
    else if (anOperator == "*" || anOperator == "/" || anOperator == "%") {
        return 2;// */%�����ȼ�Ϊ 2
    }
    else if (anOperator == "+" || anOperator == "-") {
        return 1;//�Ӽ������ȼ�Ϊ 1
    }
    return 0;//����ʶ��������


}

std::vector<Token> Calculator::tokenizeExpression(const std::string& expression){
    std::vector<Token> tokens;
    std::string token;
    bool expectOperator = false;//�������ָ��źͼ��ţ���Ϊ����������������Ͳ�����������֣�

    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];

        ////�����ո�
        //if (isspace(ch)) {
        //    continue;
        //}

        //�������֣�����С���㣩
        if (isdigit(ch) || (ch == '.' )) {//�˴����ܳ��ֵķǷ��������.1��+.
            token += ch;  //ƴ������������
            expectOperator = true;  //��һ�������������
        }
        //����+-*/%^�����
        else if (isOperator(ch)) {
            //����Ǹ�����ͷ
            if (ch == '-' && !expectOperator) {
                token += ch;  //�����ſ������ֵ�һ����
            }
            else {
                if (!token.empty()) {
                    tokens.push_back({ token, false });  //�����ڴ�֮ǰ������
                    token.clear();
                }
                tokens.push_back({ std::string(1, ch), true });  //��ǰ�������һ��Ҫ��һ���ַ�תΪ�ַ���������֮�䲻����ʽת����
                expectOperator = false;  //�ı�����
            }
        }
        //��������
        else if (ch == '(' || ch == ')') {
            if (!token.empty()) {
                tokens.push_back({ token, false });  //��֮ǰ�����ַ���tokens
                token.clear();
            }
            tokens.push_back({ std::string(1, ch), true });  //��ǰ����
            expectOperator = (ch == ')');  //�����ź�������������������Ų��ı�������
        }
        //�����������sin cos tan sqrt pow
        else if (isFunctionStart(ch)) {
            token += ch;
            while (i + 1 < expression.size() && isalpha(expression[i + 1])) {
                token += expression[++i];
                if (token == "sin" || token == "cos" || token == "tan" || token == "sqrt" || token == "pow") {
                    break;//һ���õ��������������˳�
                }
                
            }
            tokens.push_back({ token, true });  //����������Ϊ��������� tokens
            token.clear();
        }
    }

    //�������Ĳ�����������У�
    if (!token.empty()) {
        tokens.push_back({ token, false });
    }

    return tokens;
}