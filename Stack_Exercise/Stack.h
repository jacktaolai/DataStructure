#pragma once
//Stack.h

//ջ��
template <class T>
class Stack {
private:
	int _maxSize;//ջ�������
	T* element;//���ջԪ�صط�
	int top;//ջ��
public:
	int getSize(){ return(top + 1); }//��ȡջ�Ĵ�С
	//Ĭ�ϳ�ʼ����һ�ٵĴ�С
	Stack(int size=100) {
		_maxSize = size;
		element = new T[_maxSize];
		top = -1;
	}
	//�ж�ջ�շ�
	bool isEmpty() const { return top == -1; }
	//�ж�ջ����
	bool isFull() const { return top == _maxSize - 1; }
	//��ȡջ��Ԫ��
	bool getTop() const { return element[top]; }
	//����Ԫ��
	T pop() { T value = element[top]; top--; return value; }
	//��ջ
	bool push(const T& value) { top++; element[top] = value; return top != -1; }
};
