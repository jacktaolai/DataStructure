#pragma once
//Calculator
#include <string>
#include "Stack.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>   // for isdigit, isalpha

// ���ڴ洢��������Ĳ������������
struct Token {
	std::string value;  // �������������
	bool isOperator;    // true ��ʾ���������false ��ʾ�ǲ�����
};

class Calculator {
public:
	std::string expression;//���ʽ

	//�������ƥ��
	bool checkParentheses(const std::string& expression);

	//ת��Ϊ��׺���ʽ
	std::vector<Token> convertToPostfix(const std::vector<Token>& expression);

	//����Ƿ��������,�������ʺ��ַ���string�ĺ���
	bool isOperator(char ch) {
		return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%';
	}
	//����Ƿ��������,�������ʺ��ַ���string�ĺ���
	bool isOperator(const std::string& token) {
		return token == "+" || token == "-" || token == "*" || token == "/" ||token == "^" || token == "%";
	}

	// ����Ƿ�����ĸ��ͷ�ж��Ƿ��Ǻ���
	bool isFunctionStart(char ch) {
		return isalpha(ch);  // ��������ĸ��ͷ
	}

	//��ȡ���ȼ�
	int getPriority(std::string anOperator);

	//������ı��ʽתΪ�������������
	std::vector<Token> tokenizeExpression(const std::string& expression);

	//���Ժ���
	/******/
	//�򵥵Ĵ�ӡ����
	void printTokens(const std::vector<Token>& tokens) {
		for (const auto& token : tokens) {
			std::cout << token.value << " ";
		}
		std::cout << std::endl;
	}
	/******/

public:

	//���캯��
	Calculator() :expression("0") {};
	Calculator(const std::string& _expression) :expression(_expression) {};

	//������ʱ�ŵ�public
	/******/
	//����ƥ����

	/******/

};