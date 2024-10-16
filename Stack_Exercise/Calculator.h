#pragma once
//Calculator
#include <string>
#include "Stack.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>   //检查字符是否是字母或数字
#include <cmath>


// 用于存储分离出来的操作数和运算符
struct Token {
	std::string value;  // 操作数或运算符
	bool isOperator;    // true 表示是运算符，false 表示是操作数
};

class Calculator {
private:
	std::string _expression;//表达式

	//阶乘计算函数
	float factorial(float n) {
		if (n < 0 || fmod(n, 1) != 0) { throw std::runtime_error("使用了不正确的数进行阶乘！"); }
		if (n <= 1) {
			return 1;
		}
		return n * factorial(n - 1);
	}
	//检查括号匹配
	bool checkParentheses(const std::string& expression);

	//转换为后缀表达式
	std::vector<Token> convertToPostfix(const std::vector<Token>& expression);

	//检查是否是运算符,重载了适合字符和string的函数
	bool isOperator(char ch) {
		return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%' || ch == '!';
	}
	//检查是否是运算符,重载了适合字符和string的函数
	bool isOperator(const std::string& token) {
		return token == "+" || token == "-" || token == "*" || token == "/" ||token == "^" || token == "%";
	}

	// 检查是否是字母开头判断是否是函数
	bool isFunctionStart(char ch) {
		return isalpha(ch);  // 函数以字母开头
	}

	//获取优先级
	int getPriority(std::string anOperator);

	//将输入的表达式转为操作数和运算符
	std::vector<Token> tokenizeExpression(const std::string& expression);

	//测试函数
	/******/
	//简单的打印函数
	void printTokens(const std::vector<Token>& tokens) {
		for (const auto& token : tokens) {
			std::cout << token.value << " ";
		}
		std::cout << std::endl;
	}
	/******/

public:

	//构造函数
	Calculator() :_expression("0") {};
	Calculator(const std::string& _expression) :_expression(_expression) {};


	//计算(不包含括号匹配的过程）
	float caculate(const std::string& expression);
	float caculate() { return caculate(_expression); };
	//异常处理函数
	std::string expections ();

	//测试暂时放到public
	/******/
	//括号匹配检查

	/******/

};