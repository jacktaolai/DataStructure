#pragma once
//BinaryTree
#include <string>
#include "LinkQueue.h"
#include "Stack.h"
#include <unordered_map>

template<class T>
struct TreeNode{
	int priority = 0;//Ȩֵ
	T data;//����
	TreeNode* leftChild;//����
	TreeNode* rightChild;//�Һ���
	TreeNode():leftChild(nullptr),rightChild(nullptr){}
	TreeNode(T _data,int _priority=0,TreeNode* _leftChild=nullptr,TreeNode* _rightChild=nullptr)
		:data(_data),priority(_priority),leftChild(_leftChild),rightChild(_rightChild){}

	//�Ƚ����������
	bool operator ==(const TreeNode<T>& other)const { return this->data == other.data; }
};
template<class T>
class BinaryTree {
private:
	TreeNode<T>* _root;//��������ָ��
	//��ǰ�����еݹ鴴���ڵ�
	TreeNode<char>* createFromPreorder(int& n, const std::string preorder);//n��ʾ��n���ַ�
	//��̬������������
	static void swap(TreeNode<T>* node) {
		TreeNode<T>* temp = node->leftChild;
		node->leftChild = node->rightChild;
		node->rightChild = temp;
	}
	//ʵ��ǰ�����,ʹ�ú���ָ�����visit����
	void preVisit(TreeNode<T>* root, void(*visit)(TreeNode<T>* node));
	//ʹ�ú������ɾ��������
	void destory(TreeNode<T>* root = _root);
	//��ǰ������򴴽�
	TreeNode<char>* contructTree(const std::string& preorder, const std::string& inorder,
		int preStart,int preEnd,int inStar,int inEnd);
	//�������ǰ�������ֻ��Խڵ���char��unsigned charʱ��
	std::string getPreorder(TreeNode<T>* root);
public:
	TreeNode<T>* getRoot() { return _root; }
	//�������ڵ����¹�����
	void rebuildTree(TreeNode<T>* root) { _root = root; }
	BinaryTree(TreeNode<T>* root=nullptr):_root(root){}
	//�������ǰ�������ֻ��Խڵ���char��unsigned charʱ��
	std::string getPreorder();

	//��ǰ�����д�����
	void createFromPreorder(const std::string preorder);

	//�Ӳ����������
	void createFromLevelorder(const std::string levelorder);
	//��ǰ������򴴽�
	void contructTree(const std::string &preorder, const std::string &inorder);
	//ʵ��ǰ�����,ʹ�ú���ָ�����swap����
	void preVisit() { preVisit(_root, swap); }
	//ʹ��ջѰ���ض�ֵ������
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
	//��ֹ�ڴ�й©
	destory(_root);
	//�������1������#��ʾ����Ϊ�գ�����
	//�������2���������н���������
	//һ��������½��ڵ㣬Ȼ���½���������Ȼ���½�������
	int n = 0;
	_root = createFromPreorder(n, preorder);

}

template<class T>
inline void BinaryTree<T>::createFromLevelorder(const std::string levelorder){
	//��ֹ�ڴ�й©
	destory(_root);
	char nodeData;
	LinkQueue<TreeNode<char>*> levleOrder;//�洢ʣ��ڵ��ջ
	LinkQueue<TreeNode<char>*> currentNode;//�洢���������޺��ӵĽڵ�ջ
	for (char nodeData : levelorder) {
		if (nodeData != '#')
			levleOrder.enqueue(new TreeNode<char>(nodeData));
		else
			levleOrder.enqueue(nullptr);
	}
	_root = levleOrder.dequeue();
	currentNode.enqueue(_root);//�Ƚ����ڵ����
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
inline void BinaryTree<T>::contructTree(const std::string &preorder, const std::string &inorder){
	//��ֹ�ڴ�й©
	destory(_root);
	//std::unordered_map<char, int> inorderMap;
	if (preorder.size() == 0 || inorder.size() == 0) return;
	_root = contructTree(preorder, inorder, 0, preorder.size() - 1, 0, inorder.size() - 1);
}

template<class T>
inline Stack<TreeNode<T>*> BinaryTree<T>::find(T key) {
	Stack <TreeNode<T>*> ancestorStasck;//�洢�Ӹ��ڵ㵽Ŀ��ڵ㾭�������Ƚڵ��ջ
	Stack <TreeNode<T>*> rightChildStack;//ǰ������ȷ������ӹʽ��Һ��Ӵ洢����
	if(_root==nullptr) return Stack <TreeNode<T>*>();
	TreeNode<T>* current = _root;
	bool isFinished = 0;//����������Ƿ񱻷�����
	do {
		//�������Ľڵ���Ϊ���Ƚڵ�
		ancestorStasck.push(current);
		//�ҵ�Ԫ�ط�������ջ
		if ((current->leftChild!=nullptr&&current->leftChild->data== key) ||
			(current->rightChild != nullptr && current->rightChild->data== key))
			return ancestorStasck;
		if (current->rightChild != nullptr)
			//���Һ���ѹ��ջ
			rightChildStack.push(current->rightChild);
		if (current->leftChild != nullptr) 
			//���µ�ǰ�ڵ�
			current = current->leftChild;
		else if(!rightChildStack.isEmpty()){//�������Ϊ��
			ancestorStasck.pop();
			current = rightChildStack.pop();
		}
		else { isFinished = 1; }
	} while (!isFinished);//ֱ�����нڵ㱻����
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
	//�ҵ�root�����������λ��
	int position = inorder.find(preorder[preStart]);
	int leftSize = position - inStart;//��������С
	//�����������
	if (position != inStart) {
		root->leftChild = contructTree(preorder, inorder, preStart + 1, preStart + leftSize, inStart, position - 1);
	}
	//�����������
	if (position != inEnd) {
		//��������ǰ�����п�ͷ��������ǰ���β��һ
		root->rightChild = contructTree(preorder, inorder, preStart+leftSize+1, preEnd, position + 1, inEnd);
	}
	return root;
}

template<class T>
std::string BinaryTree<T>::getPreorder(TreeNode<T>* root){
	if (root == nullptr) return "#";
	std::string preorder;
	preorder += static_cast<char>(root->data);//��Ӹ��ڵ�����
	preorder += getPreorder(root->leftChild);//������������������
	preorder += getPreorder(root->rightChild);//������������������
	return preorder;
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

