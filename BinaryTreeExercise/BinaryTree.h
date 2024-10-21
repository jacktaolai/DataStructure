#pragma once
#include <string>

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

public:
	BinaryTree():root(nullptr){}

	//从前序序列创建
	void createFromPreorder(const std::string preorder);
	TreeNode<char>* createFromPreorder(int& n, int stringLength, const std::string preorder);//n表示第n个字符
	~BinaryTree() {};
};

template<class T>
void BinaryTree<T>::createFromPreorder(const std::string preorder){
	//特殊情况1，遇到#表示子树为空，返回
	//特殊情况2，整个序列结束，返回
	//一般情况：新建节点，然后新建左子树，然后新建右子树
	int n = 0;
	int stringLength = preorder.length();
	root = createFromPreorder(n, stringLength, preorder);

}


template<class T>
TreeNode<char>* BinaryTree<T>::createFromPreorder( int& n,int stringLength, const std::string preorder){
	//一定要传入引用，这样n才会被修改
	if (preorder[n] != '#') {
		TreeNode<char>* newNode = new TreeNode<T>(preorder[n]);
		newNode->leftChild = createFromPreorder(++n, stringLength, preorder);
		newNode->rightChild = createFromPreorder(++n, stringLength, preorder);
		return newNode;
	}
	else return nullptr;
}

