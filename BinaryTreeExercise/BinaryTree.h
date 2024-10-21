#pragma once
#include <string>

template<class T>
struct TreeNode
{
	T data;//����
	TreeNode* leftChild;//����
	TreeNode* rightChild;//�Һ���
	TreeNode():leftChild(nullptr),rightChild(nullptr){}
	TreeNode(T _data,TreeNode* _leftChild=nullptr,TreeNode* _rightChild=nullptr)
		:data(_data),leftChild(_leftChild),rightChild(_rightChild){}

	//�Ƚ����������
	bool operator ==(const TreeNode<T>& other)const { return this->data == other.data; }
};
template<class T>
class BinaryTree {
	TreeNode<T>* root;//��������ָ��

public:
	BinaryTree():root(nullptr){}

	//��ǰ�����д���
	void createFromPreorder(const std::string preorder);
	TreeNode<char>* createFromPreorder(int& n, int stringLength, const std::string preorder);//n��ʾ��n���ַ�
	~BinaryTree() {};
};

template<class T>
void BinaryTree<T>::createFromPreorder(const std::string preorder){
	//�������1������#��ʾ����Ϊ�գ�����
	//�������2���������н���������
	//һ��������½��ڵ㣬Ȼ���½���������Ȼ���½�������
	int n = 0;
	int stringLength = preorder.length();
	root = createFromPreorder(n, stringLength, preorder);

}


template<class T>
TreeNode<char>* BinaryTree<T>::createFromPreorder( int& n,int stringLength, const std::string preorder){
	//һ��Ҫ�������ã�����n�Żᱻ�޸�
	if (preorder[n] != '#') {
		TreeNode<char>* newNode = new TreeNode<T>(preorder[n]);
		newNode->leftChild = createFromPreorder(++n, stringLength, preorder);
		newNode->rightChild = createFromPreorder(++n, stringLength, preorder);
		return newNode;
	}
	else return nullptr;
}

