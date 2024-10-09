#pragma once
//Stack.h

//栈类
template <class T>
class Stack {
private:
	int _maxSize;//栈最大容量
	T* element;//存放栈元素地方
	int top;//栈顶
public:
	int getSize(){ return(top + 1); }//获取栈的大小
	//默认初始化给一百的大小
	Stack(int size=100) {
		_maxSize = size;
		element = new T[_maxSize];
		top = -1;
	}
	//判断栈空否
	bool isEmpty() const { return top == -1; }
	//判断栈满否
	bool isFull() const { return top == _maxSize - 1; }
	//获取栈顶元素
	bool getTop() const { return element[top]; }
	//弹出元素
	T pop() { T value = element[top]; top--; return value; }
	//入栈
	bool push(const T& value) { top++; element[top] = value; return top != -1; }
};
