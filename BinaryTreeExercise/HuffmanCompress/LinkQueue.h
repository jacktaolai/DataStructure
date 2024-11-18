#pragma once
//LinkQueue.h

#include <cstddef>
#include <stdexcept>
template<class T>
struct QueueNode {
	T data;
	QueueNode<T>* last;
	QueueNode():last(nullptr){}
	QueueNode(T _data, QueueNode<T>* _last=nullptr) :data(_data), last(_last) {}
};
template <class T>
class LinkQueue{
private:
	QueueNode<T>* front;
	QueueNode<T>* rear;
	size_t _size=0;
public:
	LinkQueue():front(nullptr),rear(nullptr),_size(0){}
	//����
	void enqueue(T _data);
	//����
	T dequeue();
	//��ȡ���г���
	size_t getSize() { return _size; }
	~LinkQueue();
};

template<class T>
inline void LinkQueue<T>::enqueue(T _data){
	if (_size==0) {
		front = new QueueNode<T>(_data);
		rear = front;
		_size = 1;
	}
	else {
		rear->last = new QueueNode<T>(_data);
		rear = rear->last;
		++_size;
	}
}

template<class T>
inline T LinkQueue<T>::dequeue(){
	//warning:�������ܻ��пն��г��ӵĴ���
	if (_size == 0) { throw std::runtime_error("Queue is empty!"); }
	QueueNode<T> temp = *front;
	delete front;
	front = temp.last;
	--_size;
	return temp.data;
}

template<class T>
inline LinkQueue<T>::~LinkQueue(){
	while (_size != 0) {
		QueueNode<T> temp = *front;
		delete front;
		front = temp.last;
		--_size;
	}
}
