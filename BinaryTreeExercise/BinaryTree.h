#pragma once
#include <string>
#include "LinkQueue.h"
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
	//��ǰ�����еݹ鴴���ڵ�
	TreeNode<char>* createFromPreorder(int& n, const std::string preorder);//n��ʾ��n���ַ�
public:
	BinaryTree():root(nullptr){}

	//��ǰ�����д�����
	void createFromPreorder(const std::string preorder);

	//�Ӳ����������
	void createFromLevelorder(const std::string levelorder);
	//TODO!
	~BinaryTree() {};
};

template<class T>
void BinaryTree<T>::createFromPreorder(const std::string preorder){
	//�������1������#��ʾ����Ϊ�գ�����
	//�������2���������н���������
	//һ��������½��ڵ㣬Ȼ���½���������Ȼ���½�������
	int n = 0;
	root = createFromPreorder(n, preorder);

}

template<class T>
inline void BinaryTree<T>::createFromLevelorder(const std::string levelorder){
	char nodeData;
	LinkQueue<TreeNode<char>*> levleOrder;//�洢ʣ��ڵ��ջ
	LinkQueue<TreeNode<char>*> currentNode;//�洢���������޺��ӵĽڵ�ջ
	for (char nodeData : levelorder) {
		if (nodeData != '#')
			levleOrder.enqueue(new TreeNode<char>(nodeData));
		else
			levleOrder.enqueue(nullptr);
	}
	root = levleOrder.dequeue();
	currentNode.enqueue(root);//�Ƚ����ڵ����
	while(currentNode.getSize()!=0){
		TreeNode<char>* parentNode = currentNode.dequeue();//��Ҫ��Ӻ��ӵĸ��ڵ�
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
	//һ��Ҫ�������ã�����n�Żᱻ�޸�
	if (preorder[n] != '#') {
		TreeNode<char>* newNode = new TreeNode<T>(preorder[n]);
		newNode->leftChild = createFromPreorder(++n, preorder);
		newNode->rightChild = createFromPreorder(++n, preorder);
		return newNode;
	}
	else return nullptr;
}

