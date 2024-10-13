//Link.h

#pragma once
#include <iostream>
template<class T>
struct LinkNode {
	T data;
	LinkNode<T>* succ;//后继节点
	//节点初始化函数
	LinkNode(const LinkNode<T>* ptr = nullptr) { succ = ptr; }
	LinkNode(const T& _data, LinkNode<T>* ptr = nullptr) {
		data = _data;
		succ = ptr;
	}
};

template<class T>
class Link {
	unsigned long _size;//链表大小
	LinkNode<T>* trailer;//尾节点

public:
	LinkNode<T>* header;//头节点
	//默认构造函数
	Link() { header = nullptr; trailer = header; _size = 0; }
	//含参构造函数
	Link(const T _data) { header = new LinkNode<T>(_data); trailer = header; _size = 1; }
	//TODO:复制构造函数（深拷贝）
	//Link(const Link<T>& L);
	
	//浅拷贝
	Link(LinkNode<T>* _header);
	//获取链表大小
	unsigned long getSize(int i=0);
	//在链表尾部追加元素
	void append(const T &_data);
	//TODO:重载[]运算符

	//获取头指针
	LinkNode<T>* first() {
		return header;
	}
	//获取尾指针
	LinkNode<T>* last() { return trailer; }
	//合并非递增有序链表
	template<class U>
	friend Link<U> mergeSortedLink(Link<U>& ha, Link<U>& hb);
	//实现链表的逆置
	void reverse();
	//递归顺序输出(因为默认参数里不能有this（运行时确定）故作此下策)
	void printInOrderRecursive(const LinkNode<T>* curr) const;
	//递归逆序输出
	void printInReverseOrderRecursive(const LinkNode<T>* curr) const;
	//递归顺序输出
	void printInOrderRecursive() const { printInOrderRecursive(this->header); }
	//递归逆序输出
	void printInReverseOrderRecursive() const { printInReverseOrderRecursive(this->header); }
	//析构函数
	~Link();

};


template <class T>
Link<T>::Link(LinkNode<T>* _header) {
	header = _header;
	trailer = _header;
	//将没有下一节点的节点定义为尾节点
	while (trailer->succ != nullptr) {
		trailer = trailer->succ;
	}
	//更新链表大小
	getSize();
}

template<class T>
unsigned long Link<T>::getSize(int i/*=0*/)
{
	if (!i) {
		LinkNode<T>* curr = header;
		_size = 0;
		while (curr != nullptr) {
			_size += 1;
			curr = curr->succ;
		}
	}
	return _size;
}

template <class T>
void Link<T>::append(const T& _data) {
	//判断链表是否为空
	if (header == nullptr) {//若为空直接加在首节点
		header = new LinkNode<T>(_data);
		trailer = header;
	}
	else {//若不为空加在尾节点，且尾节点偏移
		trailer->succ = new LinkNode<T>(_data);
		trailer = trailer->succ;
	}
	_size += 1;
}



template <class T>
Link<T> mergeSortedLink(Link<T>& ha, Link<T>& hb) {
	LinkNode<T>* _header = nullptr;
	LinkNode<T>* _trailer = nullptr;

	//将原来链表较大头节点作为新链表的头
	if (ha.header->data >= hb.header->data) {
		_header = ha.header;
		ha.header = ha.header->succ;
	}
	else {
		_header = hb.header;
		hb.header = hb.header->succ;
	}
	_trailer = _header;

	//将较大的一个节点加到新链表上面，直到其中一个为空
	while (ha.header != nullptr && hb.header != nullptr) {
		if (ha.header->data >= hb.header->data) {
			_trailer->succ = ha.header;
			ha.header = ha.header->succ;
		}
		else {
			_trailer->succ = hb.header;
			hb.header = hb.header->succ;
		}
		_trailer = _trailer->succ;
	}

	//将剩余元素加到新链表上
	if (ha.header != nullptr) {
		_trailer->succ = ha.header;
	}
	if (hb.header != nullptr) {
		_trailer->succ = hb.header;
	}
	//将原链表头尾指针置空防止悬空指针
	ha.header = nullptr;
	ha.trailer = nullptr;
	hb.header = nullptr;
	hb.trailer = nullptr;
	return Link<T>(_header);
}

template<class T>
void Link<T>::reverse() {
	//初始化前一个节点为 nullptr
	LinkNode<T>* prev = nullptr;
	//当前节点初始化为头节点
	LinkNode<T>* curr = header;
	//下一个节点的指针
	LinkNode<T>* next = nullptr;

	//将 trailer 指向原链表的头节点，因为反转后它将变成尾节点
	trailer = header;

	//遍历整个链表
	while (curr != nullptr) {
		next = curr->succ;//保存当前节点的下一个节点
		curr->succ = prev;
		prev = curr;
		curr = next;
	}

	header = prev;
}

template<class T>
void Link<T>::printInReverseOrderRecursive(const LinkNode<T>* curr) const
{
	if (curr == nullptr) { return; }//特殊情况
	else {//一般情况
		printInReverseOrderRecursive(curr->succ);
		std::cout << curr->data << " ";
	}
}

template<class T>
void Link<T>::printInOrderRecursive(const LinkNode<T>* curr) const {
	if (curr == nullptr) { return; }
	else {
		std::cout << curr->data << " ";
		printInOrderRecursive(curr->succ);
	}
}


template<class T>
Link<T>::~Link() {
	while (header != trailer) {
		LinkNode<T>* temp = header->succ;
		delete header;
		header = temp;
	}
}
