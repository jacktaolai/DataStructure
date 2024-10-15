#include "Calculator.h"

bool Calculator::checkParentheses(const std::string& expression){
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

std::vector<Token> Calculator::convertToPostfix(const std::vector<Token>& expression){
    Stack<Token> operatorStack;//�����洢�������ջ
    std::vector<Token> postfix;//�洢��׺���ʽ������

    for (const Token& token : expression) {
        if (!token.isOperator) {
            //����ǲ�������ֱ���������׺���ʽ
            postfix.push_back(token);
        }
        else if (token.value == "(") {
            //������ѹ��ջ
            operatorStack.push(token);
        }
        else if (token.value == ",") {
            //����ʱ������ջֱ������������
            while (!operatorStack.isEmpty() && operatorStack.getTop().value != "(") {
                postfix.push_back(operatorStack.pop());
            }
            //operatorStack.pop();//��Ҫ���������ţ�
        }
        else if (token.value == ")") {
            //������ʱ������ջֱ������������
            while (!operatorStack.isEmpty() && operatorStack.getTop().value != "(") {
                postfix.push_back(operatorStack.pop());
            }
            operatorStack.pop();//����������
        }
        else if (token.isOperator) {
            // ������������������ȼ�
            while (!operatorStack.isEmpty() &&
                (getPriority(operatorStack.getTop().value) >= getPriority(token.value))) {
                postfix.push_back(operatorStack.pop());//���ȼ�����ջ�����ڰ��մ�������Ҳ�ǵ��ڣ�
            }
            operatorStack.push(token);//ֱ�����ȼ�����ջ��
        }
    }

    //��ʣ��Ĳ���������ջ
    while (!operatorStack.isEmpty()) {
        postfix.push_back(operatorStack.pop());
    }

    return postfix;
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
    //�𷵻غ����ڵ����ð�:(
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
        if (isdigit(ch) || (ch == '.') || (ch == 'e')) {
            while (i < expression.size() && (isdigit(ch) || (ch == '.') || (ch == 'e'))) {//�˴����ܳ��ֵķǷ��������.1��+.
                if (ch == 'e') {
                    token += expression[i];
                    i++;//��Ϊe��������ţ�+-���������������ȡһλ
                    //waring: ��������1e�Ĵ�����ʽ�޷��ж�
                    token += expression[i];
                }
                else {
                    token += ch;  //ƴ������������
                }
                i++;
                ch = expression[i];
                expectOperator = true;  //��һ�������������
            }
            //��������ѹ��ջ
            tokens.push_back({ token, false });
            token.clear();
        }

        //����+-*/%^�����
        if (isOperator(ch)) {
            //����Ǹ�����ͷ
            if (ch == '-' && !expectOperator) {
                token += ch;  //�����ſ������ֵ�һ����
            }
            else {
                //if (!token.empty()) {
                //    tokens.push_back({ token, false });//�����ڴ�֮ǰ������
                //    token.clear();
                //}
                tokens.push_back({ std::string(1, ch), true });  //��ǰ�������һ��Ҫ��һ���ַ�תΪ�ַ���������֮�䲻����ʽת����
                token.clear();
                expectOperator = false;  //�ı�����
            }
        }
        //������,�������ȼ���������һ�������Ҷ��ź���������ǲ�����
        else if (ch == ',') {
            tokens.push_back({ std::string(1, ch), true });  
            token.clear();
            expectOperator = false;
        }
        //��������
        else if (ch == '(' || ch == ')') {
            tokens.push_back({ std::string(1, ch), true });  //��ǰ����
            token.clear();
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


    return tokens;
}

float Calculator::caculate(const std::string& expression) {
    //�����ʽתΪtoken
    std::vector<Token> tokens = tokenizeExpression(expression);

    //��tokenתΪ��׺���ʽ
    std::vector<Token> postfix = convertToPostfix(tokens);

    //��ջ����������
    Stack<float> resultStack;

    for (const Token& token : postfix) {
        if (!token.isOperator) {
            //��������ֱ��ѹ��ջ������stof���ַ���תΪ�����ͣ�
            //warning:������ܻ��������ת������ı���
            char* end;
            const char* value = token.value.c_str();
            float floatNumber = std::strtof(value,&end);
            //ͨ��end��Ϊ�����ж�stringû����ȷ��תΪfloat
            if (*end != '\0') {
                throw std::runtime_error("��������С����");
            }
            resultStack.push(floatNumber);
        }
        else if (isOperator(token.value)) {
            //�ǲ�������ȡ��������������Ԫ��������
            float rightNum = resultStack.pop();//�������
            float leftNum = resultStack.pop();//�Ҳ�����
            
            //����
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
                    throw std::runtime_error("��������Ϊ0��");
                }
                resultStack.push(leftNum / rightNum);
            }
            else if (token.value == "%") {
                //Ҫ��ǿ��ת��������
                resultStack.push(static_cast<int>(leftNum) % static_cast<int>(rightNum));
            }
            else if (token.value == "^") {
                resultStack.push(std::pow(leftNum, rightNum));
            }
        }
        else if (isFunctionStart(token.value[0])) {
            //�����������
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

        return "���ʽ���Ϸ���";
    }

}
