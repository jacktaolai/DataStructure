#pragma once
//PriorityLinkQueue.h
#include <stdexcept>

template<class T>
struct PriorityQueueNode {
    T data;
    int priority;
    PriorityQueueNode<T>* last;
    PriorityQueueNode() : last(nullptr) {}
    PriorityQueueNode(T _data, int _priority, PriorityQueueNode<T>* _last = nullptr)
        : data(_data), priority(_priority), last(_last) {}
};

template <class T>
class PriorityLinkQueue {
private:
    PriorityQueueNode<T>* front;
    PriorityQueueNode<T>* rear;
    size_t _size;
public:
    PriorityLinkQueue() : front(nullptr), rear(nullptr), _size(0) {}
	//入队
    void enqueue(T _data, int priority);
	//出队
    T dequeue();
	//获取队列长度
    size_t getSize() { return _size; }

    ~PriorityLinkQueue();
};

template<class T>
inline void PriorityLinkQueue<T>::enqueue(T _data, int _priority) {
    PriorityQueueNode<T>* newNode = new PriorityQueueNode<T>(_data, _priority);
    if (_size == 0) {
        front = rear = newNode;
    }
    else if (_priority <= front->priority) {
        newNode->last = front;
        front = newNode;
    }
    else if (_priority >= rear->priority) {
        rear->last = newNode;
        rear = newNode;
    }
    else {
        PriorityQueueNode<T>* current = front;
        while (current->last != nullptr && current->last->priority < _priority) {
            current = current->last;
        }
        newNode->last = current->last;
        current->last = newNode;
    }
    ++_size;
}

template<class T>
inline T PriorityLinkQueue<T>::dequeue() {
    if (_size == 0) {
        throw std::runtime_error("Queue is empty!");
    }
    PriorityQueueNode<T>* temp = front;
    T data = temp->data;
    front = front->last;
    delete temp; 
    --_size;
    return data;
}

template<class T>
inline PriorityLinkQueue<T>::~PriorityLinkQueue() {
    while (_size != 0) {
        dequeue();//使用dequeue析构
    }
}
