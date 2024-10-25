#pragma once
#include <string>
#include "LinkQueue.h"
template<class T>
struct TreeNode
{
	T data;//数据
	TreeNode* leftChild;//左孩子
	TreeNode* rightChild;//右孩子
	TreeNode():leftChild(nullptr),rightChild(nullptr){}
	TreeNode(T _data,TreeNode* _leftChild=nullptr,TreeNode* _rightChild=nullptr)
		:data(_data),leftChild(_leftChild),rightChild(_rightChild){}

	//比较运算符重载
	bool operator ==(const TreeNode<T>& other)const { return this->data == other.data; }
};
template<class T>
class BinaryTree {
	TreeNode<T>* root;//二叉树根指针
	//从前序序列递归创建节点
	TreeNode<char>* createFromPreorder(int& n, const std::string preorder);//n表示第n个字符
public:
	BinaryTree():root(nullptr){}

	//从前序序列创建树
	void createFromPreorder(const std::string preorder);

	//从层序遍历创建
	void createFromLevelorder(const std::string levelorder);
	//TODO!
	~BinaryTree() {};
};

template<class T>
void BinaryTree<T>::createFromPreorder(const std::string preorder){
	//特殊情况1，遇到#表示子树为空，返回
	//特殊情况2，整个序列结束，返回
	//一般情况：新建节点，然后新建左子树，然后新建右子树
	int n = 0;
	root = createFromPreorder(n, preorder);

}

template<class T>
inline void BinaryTree<T>::createFromLevelorder(const std::string levelorder){
	char nodeData;
	LinkQueue<TreeNode<char>*> levleOrder;//存储剩余节点的栈
	LinkQueue<TreeNode<char>*> currentNode;//存储创建但是无孩子的节点栈
	for (char nodeData : levelorder) {
		if (nodeData != '#')
			levleOrder.enqueue(new TreeNode<char>(nodeData));
		else
			levleOrder.enqueue(nullptr);
	}
	root = levleOrder.dequeue();
	currentNode.enqueue(root);//先将根节点加入
	while(currentNode.getSize()!=0){
		TreeNode<char>* parentNode = currentNode.dequeue();//需要添加孩子的父节点
		if (parentNode != nullptr) {
			TreeNode<char>* leftChild = levleOrder.dequeue();
			TreeNode<char>* rightChild = levleOrder.dequeue();
			parentNode->leftChild = leftChild;
			parentNode->rightChild = rightChild;
			currentNode.enqueue(leftChild);
			currentNode.enqueue(rightChild);
		}
	}

}

template<class T>
TreeNode<char>* BinaryTree<T>::createFromPreorder( int& n, const std::string preorder){
	//一定要传入引用，这样n才会被修改
	if (preorder[n] != '#') {
		TreeNode<char>* newNode = new TreeNode<T>(preorder[n]);
		newNode->leftChild = createFromPreorder(++n, preorder);
		newNode->rightChild = createFromPreorder(++n, preorder);
		return newNode;
	}
	else return nullptr;
}

