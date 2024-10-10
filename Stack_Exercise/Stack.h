#pragma once
//Stack.h

//链式栈类节点
template <class T>
struct StackNode {
	T data;
	StackNode<T>* next;
	StackNode(T d=0,StackNode<T>* ptr=nullptr):data(d),next(ptr){}
};

//链式栈类
template<class T>
class Stack {
private:
	StackNode<T>* top;
public:
	Stack() { top = nullptr; }
	bool isEmpty(){ return top == nullptr; }

	//将元素压入栈
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

	//将元素弹出栈
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