#pragma once
//Calculator
#include <string>
#include "Stack.h"

class Calculator {
private:
	std::string expression;//表达式


public:

	//构造函数
	Calculator() :expression("0") {};
	Calculator(const std::string& _expression):expression(_expression) {};

	//测试暂时放到public
	/******/
	//括号匹配检查
	bool checkParentheses(const std::string& expression);
	/******/

};