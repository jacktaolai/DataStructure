#pragma once
//Calculator
#include <string>
#include "Stack.h"

class Calculator {
private:
	std::string expression;//���ʽ


public:

	//���캯��
	Calculator() :expression("0") {};
	Calculator(const std::string& _expression):expression(_expression) {};

	//������ʱ�ŵ�public
	/******/
	//����ƥ����
	bool checkParentheses(const std::string& expression);
	/******/

};