#pragma once
//Stack.h

//��ʽջ��ڵ�
template <class T>
struct StackNode {
	T data;
	StackNode<T>* next;
	StackNode(T d=0,StackNode<T>* ptr=nullptr):data(d),next(ptr){}
};

//��ʽջ��
template<class T>
class Stack {
private:
	StackNode<T>* top;
public:
	Stack() { top = nullptr; }
	bool isEmpty(){ return top == nullptr; }

	//��Ԫ��ѹ��ջ
	void push(const T& value){
		if (!isEmpty()) {
			StackNode<T>* curr = top;
			top = new StackNode<T>(value);
			top->next = curr;
		}
		else {
			top = new StackNode<T>(value);
		}
	}

	//��Ԫ�ص���ջ
	T pop() {
			StackNode<T>* curr = top;
			top = top->next;
			T value = curr->data;
			delete curr;
			return value;
	}

	
	~Stack() {
		while (!isEmpty()){
			StackNode<T>* curr = top;
			top = top->next;
			delete curr;
		}
	}

};