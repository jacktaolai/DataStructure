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
	int size;
public:
	Stack() { size = 0; top = nullptr; }
	//���ƹ��캯��
	Stack(const Stack<T>& other) : top(nullptr), size(other.size) {
		if (!other.top) return;//�������ջΪ�գ�����Ҫ����
		//����ͷ�ڵ�
		top = new StackNode<T>(other.top->data);
		StackNode<T>* current = top;//��ǰջ�еĽڵ�ָ��
		StackNode<T>* otherCurrent = other.top->next;//����ջ�Ľڵ�ָ��

		// ���� other ջ�Ľڵ㣬�������
		while (otherCurrent) {
			current->next = new StackNode<T>(otherCurrent->data);//���Ƶ�ǰ�ڵ�
			current = current->next;//ǰ������һ���ڵ�
			otherCurrent = otherCurrent->next;//ǰ���� other ����һ���ڵ�
		}
	}
	//�ж�ջ��
	bool isEmpty(){ return top == nullptr; }

	//��ȡջ��С
	int getSize() {
		return size;
	}
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
		size++;
	}

	//����תջ�Ĺ��캯��
	Stack(const T* list, int size) {
		top = nullptr;
		for (int i = size - 1; i >= 0; --i) { //������ĩβ����ͷ����ѹ��ջ
			push(list[i]);
		}
	}

	//��Ԫ�ص���ջ
	T pop() {
			StackNode<T>* curr = top;
			top = top->next;
			T value = curr->data;
			delete curr;
			size--;
			return value;
	}

	//��ȡ��Ԫ��
	T getTop() { return top->data; }
	
	~Stack() {
		while (!isEmpty()){
			StackNode<T>* curr = top;
			top = top->next;
			delete curr;
		}
	}

};