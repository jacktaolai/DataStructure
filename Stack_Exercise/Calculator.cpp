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
