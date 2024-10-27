#pragma once
//PriorityLinkQueue.h
#include <stdexcept>

template<class T>
struct PriorityQueueNode {
	T data;
	int priority;
	PriorityQueueNode<T>* last;
	PriorityQueueNode() :last(nullptr) {}
	PriorityQueueNode(T _data,int _priority, PriorityQueueNode<T>* _last = nullptr) :data(_data), priority(_priority),last(_last) {}
};
template <class T>
class PriorityLinkQueue {
private:
	PriorityQueueNode<T>* front;
	PriorityQueueNode<T>* rear;
	size_t _size = 0;
public:
	PriorityLinkQueue() :front(nullptr), rear(nullptr), _size(0) {}
	//入队
	void enqueue(T _data,int priority);
	//出队
	T dequeue();
	//获取队列长度
	size_t getSize() { return _size; }
	~PriorityLinkQueue();
};

template<class T>
inline void PriorityLinkQueue<T>::enqueue(T _data,int _priority) {
	if (_size == 0) {
		front = new PriorityQueueNode<T>(_data,_priority);
		rear = front;
		_size = 1;
	}
	else {
		PriorityQueueNode<T>* current = front;
		if (_priority < front->priority) {
			front = new PriorityQueueNode<T>(_data,_priority);
			front->last = current;
		}
		if (_priority > rear->priority) {
			PriorityQueueNode<T>* temp = rear;
			rear = new PriorityQueueNode<T>(_data,_priority);
			temp->last = rear;
		}
		else while (_priority >= current->priority) {
			if (_priority <= current->last->priority) {
				PriorityQueueNode<T>* temp = current->last;
				current->last = new PriorityQueueNode<T>(_data, _priority);
				current->last->last = temp;
				break;
			}
			current = current->last;
		}
		++_size;
	}
}

template<class T>
inline T PriorityLinkQueue<T>::dequeue() {
	//warning:这里可能会有空队列出队的错误
	if (_size == 0) { throw std::runtime_error("Queue is empty!"); }
	PriorityQueueNode<T> temp = *front;
	delete front;
	front = temp.last;
	--_size;
	return temp.data;
}

template<class T>
inline PriorityLinkQueue<T>::~PriorityLinkQueue() {
	while (_size != 0) {
		PriorityQueueNode<T> temp = *front;
		delete front;
		front = temp.last;
		--_size;
	}
}
