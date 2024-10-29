#pragma once
//BinaryTree
#include <string>
#include "LinkQueue.h"
#include "Stack.h"
#include <unordered_map>

template<class T>
struct TreeNode{
	int priority = 0;//权值
	T data;//数据
	TreeNode* leftChild;//左孩子
	TreeNode* rightChild;//右孩子
	TreeNode():leftChild(nullptr),rightChild(nullptr){}
	TreeNode(T _data,int _priority=0,TreeNode* _leftChild=nullptr,TreeNode* _rightChild=nullptr)
		:data(_data),priority(_priority),leftChild(_leftChild),rightChild(_rightChild){}

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
	//从前序和中序创建
	TreeNode<char>* contructTree(const std::string& preorder, const std::string& inorder,
		int preStart,int preEnd,int inStar,int inEnd);
	//输出树的前序遍历（只针对节点是char或unsigned char时）
	std::string getPreorder(TreeNode<T>* root);
public:
	TreeNode<T>* getRoot() { return _root; }
	//给定根节点重新构建树
	void rebuildTree(TreeNode<T>* root) { _root = root; }
	BinaryTree(TreeNode<T>* root=nullptr):_root(root){}
	//输出树的前序遍历（只针对节点是char或unsigned char时）
	std::string getPreorder();

	//从前序序列创建树
	void createFromPreorder(const std::string preorder);

	//从层序遍历创建
	void createFromLevelorder(const std::string levelorder);
	//从前序和中序创建
	void contructTree(const std::string &preorder, const std::string &inorder);
	//实现前序访问,使用函数指针完成swap操作
	void preVisit() { preVisit(_root, swap); }
	//使用栈寻找特定值的祖先
	Stack<TreeNode<T>*> find(T key);
	~BinaryTree() { destory(_root); }
};

template<class T>
inline std::string BinaryTree<T>::getPreorder(){
	if (!std::is_same<T, unsigned char>::value && !std::is_same<T, char>::value)
		throw std::runtime_error("Wrong data type!");
	return getPreorder(_root);
}

template<class T>
void BinaryTree<T>::createFromPreorder(const std::string preorder){
	//防止内存泄漏
	destory(_root);
	//特殊情况1，遇到#表示子树为空，返回
	//特殊情况2，整个序列结束，返回
	//一般情况：新建节点，然后新建左子树，然后新建右子树
	int n = 0;
	_root = createFromPreorder(n, preorder);

}

template<class T>
inline void BinaryTree<T>::createFromLevelorder(const std::string levelorder){
	//防止内存泄漏
	destory(_root);
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
inline void BinaryTree<T>::contructTree(const std::string &preorder, const std::string &inorder){
	//防止内存泄漏
	destory(_root);
	//std::unordered_map<char, int> inorderMap;
	if (preorder.size() == 0 || inorder.size() == 0) return;
	_root = contructTree(preorder, inorder, 0, preorder.size() - 1, 0, inorder.size() - 1);
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
inline TreeNode<char>* BinaryTree<T>::contructTree(const std::string& preorder, const std::string& inorder,
	int preStart, int preEnd, int inStart, int inEnd){
	TreeNode<T>* root = new TreeNode<char>(preorder[preStart]);
	//找到root在中序遍历的位置
	int position = inorder.find(preorder[preStart]);
	int leftSize = position - inStart;//左子树大小
	//如果有左子树
	if (position != inStart) {
		root->leftChild = contructTree(preorder, inorder, preStart + 1, preStart + leftSize, inStart, position - 1);
	}
	//如果有右子树
	if (position != inEnd) {
		//右子树的前序序列开头是左子树前序结尾加一
		root->rightChild = contructTree(preorder, inorder, preStart+leftSize+1, preEnd, position + 1, inEnd);
	}
	return root;
}

template<class T>
std::string BinaryTree<T>::getPreorder(TreeNode<T>* root){
	if (root == nullptr) return "#";
	std::string preorder;
	preorder += static_cast<char>(root->data);//添加根节点数据
	preorder += getPreorder(root->leftChild);//左子树的先序遍历结果
	preorder += getPreorder(root->rightChild);//右子树的先序遍历结果
	return preorder;
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

