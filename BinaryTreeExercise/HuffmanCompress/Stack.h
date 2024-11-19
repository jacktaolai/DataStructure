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
	//复制构造函数
	Stack(const Stack<T>& other) : top(nullptr), size(other.size) {
		if (!other.top) return;//如果其他栈为空，不需要复制
		//复制头节点
		top = new StackNode<T>(other.top->data);
		StackNode<T>* current = top;//当前栈中的节点指针
		StackNode<T>* otherCurrent = other.top->next;//其他栈的节点指针

		// 遍历 other 栈的节点，逐个复制
		while (otherCurrent) {
			current->next = new StackNode<T>(otherCurrent->data);//复制当前节点
			current = current->next;//前进到下一个节点
			otherCurrent = otherCurrent->next;//前进到 other 的下一个节点
		}
	}
	//判断栈空
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