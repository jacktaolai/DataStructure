#pragma once
#include <string>
#include "LinkQueue.h"
#include "Stack.h"
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
private:
	TreeNode<T>* _root;//二叉树根指针
	//从前序序列递归创建节点
	TreeNode<char>* createFromPreorder(int& n, const std::string preorder);//n表示第n个字符
	//静态辅助交换函数
	static void swap(TreeNode<T>* node) {
		TreeNode<T>* temp = node->leftChild;
		node->leftChild = node->rightChild;
		node->rightChild = temp;
	}
	//实现前序访问,使用函数指针完成visit操作
	void preVisit(TreeNode<T>* root, void(*visit)(TreeNode<T>* node));
	//使用后序遍历删除整棵树
	void destory(TreeNode<T>* root = _root);

public:
	BinaryTree():_root(nullptr){}

	//从前序序列创建树
	void createFromPreorder(const std::string preorder);

	//从层序遍历创建
	void createFromLevelorder(const std::string levelorder);
	//实现前序访问,使用函数指针完成swap操作
	void preVisit() { preVisit(_root, swap); }
	//使用栈寻找特定值的祖先
	Stack<TreeNode<T>*> find(T key);
	//TODO!
	~BinaryTree() { destory(_root); }
};

template<class T>
void BinaryTree<T>::createFromPreorder(const std::string preorder){
	//特殊情况1，遇到#表示子树为空，返回
	//特殊情况2，整个序列结束，返回
	//一般情况：新建节点，然后新建左子树，然后新建右子树
	int n = 0;
	_root = createFromPreorder(n, preorder);

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
	_root = levleOrder.dequeue();
	currentNode.enqueue(_root);//先将根节点加入
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
inline Stack<TreeNode<T>*> BinaryTree<T>::find(T key) {
	Stack <TreeNode<T>*> ancestorStasck;//存储从根节点到目标节点经过的祖先节点的栈
	Stack <TreeNode<T>*> rightChildStack;//前序遍历先访问左孩子故将右孩子存储起来
	if(_root==nullptr) return Stack <TreeNode<T>*>();
	TreeNode<T>* current = _root;
	bool isFinished = 0;//标记整棵树是否被访问完
	do {
		//将经过的节点视为祖先节点
		ancestorStasck.push(current);
		//找到元素返回祖先栈
		if ((current->leftChild!=nullptr&&current->leftChild->data== key) ||
			(current->rightChild != nullptr && current->rightChild->data== key))
			return ancestorStasck;
		if (current->rightChild != nullptr)
			//将右孩子压入栈
			rightChildStack.push(current->rightChild);
		if (current->leftChild != nullptr) 
			//更新当前节点
			current = current->leftChild;
		else if(!rightChildStack.isEmpty()){//如果左孩子为空
			ancestorStasck.pop();
			current = rightChildStack.pop();
		}
		else { isFinished = 1; }
	} while (!isFinished);//直到所有节点被访问
	return Stack <TreeNode<T>*>();
}

template<class T>
inline void BinaryTree<T>::preVisit(TreeNode<T>* root, void(*visit)(TreeNode<T>* node)){
	if (root == nullptr) return;
	else {
		visit(root);
		preVisit(root->leftChild, visit);
		preVisit(root->rightChild, visit);
	}
}

template<class T>
inline void BinaryTree<T>::destory(TreeNode<T>* root){
	if (root != nullptr) {
		destory(root->leftChild);
		destory(root->rightChild);
		delete root;
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

