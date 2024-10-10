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
	int size;
public:
	Stack() { size = 0; top = nullptr; }
	bool isEmpty(){ return top == nullptr; }

	//获取栈大小
	int getSize() {
		return size;
	}
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
		size++;
	}

	//数组转栈的构造函数
	Stack(const T* list, int size) {
		top = nullptr;
		for (int i = size - 1; i >= 0; --i) { //从数组末尾到开头依次压入栈
			push(list[i]);
		}
	}

	//将元素弹出栈
	T pop() {
			StackNode<T>* curr = top;
			top = top->next;
			T value = curr->data;
			delete curr;
			size--;
			return value;
	}

	//获取顶元素
	T getTop() { return top->data; }
	
	~Stack() {
		while (!isEmpty()){
			StackNode<T>* curr = top;
			top = top->next;
			delete curr;
		}
	}

};