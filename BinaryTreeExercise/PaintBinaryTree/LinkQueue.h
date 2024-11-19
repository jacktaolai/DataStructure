#pragma once
//LinkQueue.h

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
	//入队
	void enqueue(T _data);
	//出队
	T dequeue();
	//获取队列长度
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
	//warning:这里可能会有空队列出队的错误
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
