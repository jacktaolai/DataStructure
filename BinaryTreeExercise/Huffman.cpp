//Huffman.cpp
#include "Huffman.h"

void HuffmanCode::createHuffman(const std::vector<unsigned int>& frequence) {
    //��Ƶ�ʴ�С���󴴽����ȶ��У����ڵ������У��ڵ����unsigned char�����Ӧ��Ƶ��
    PriorityLinkQueue<TreeNode<unsigned char>*> forestRoot;
    for (int i = 0; i <= 255; ++i) {
        forestRoot.enqueue(new TreeNode<unsigned char>(i, frequence[i]), frequence[i]);
    }
    while (forestRoot.getSize() > 1) {
        //��ɭ����ȡ����С��Ȩֵ
        TreeNode<unsigned char>* leftChild = forestRoot.dequeue();
        TreeNode<unsigned char>* rightChild = forestRoot.dequeue();
        //���������ӵ�Ȩֵ��ΪȨֵ
        //�˴��ķ�Ҷ�ӽڵ�ͳһ���ݴ�0���Ժ�ͨ���ж��������Һ����ж��Ƿ��ӽڵ�
        TreeNode<unsigned char>* newNode = new TreeNode<unsigned char>(0, leftChild->priority + rightChild->priority);
        newNode->leftChild = leftChild;
        newNode->rightChild = rightChild;
        forestRoot.enqueue(newNode, newNode->priority);
    }
    if (forestRoot.getSize() == 1) {
        huffmanTree.rebuildTree(forestRoot.dequeue());
    }
}




std::mutex mtx;  // �����������ںϲ����ʱ���߳�ͬ��

// ���߳�ͳ���ַ�Ƶ��
//1G�ļ�1����ʮ��
static void countFrequencyInChunk(std::string filename, std::unordered_map<char, int>& freqMap) {
    std::ifstream file(filename, std::ios::binary); // �����ƺͶ�λ�ļ�ĩβ
    char ch;
    int i = 0;
    while (i <= 1024 * 1024 * 1024) {
        file.get(ch);
        //std::lock_guard<std::mutex> lock(mtx); // �����Ը���Ƶ��
        freqMap[ch]++;
        ++i;
    }

}

void HuffmanCode::readFile(const std::string& fileName, std::vector<char>& charSet, int size) {
    std::ifstream file;
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Failed to open the file!");
    file.read(charSet.data(), size);
    //��ȡʵ�ʶ�ȡ���ֽ���
    std::streamsize bytesRead = file.gcount();
    // ��ӡ��ȡ���ֽ���
    std::cout << "ʵ�ʶ�ȡ���ֽ���: " << bytesRead << std::endl;
    file.close();
}
void HuffmanCode::charFrequence(const std::vector<char>& charSet, std::vector<unsigned int>& frequence) {
    for (unsigned char byte : charSet) {
        frequence[byte]++;
    }
}

void HuffmanCode::getHuffmanCode(){
    if (huffmanTree.getRoot() == nullptr) throw std::runtime_error("Tree is empty!");
    std::string codePath = "";//��Ϊ0��Ϊ1
    getHuffmanCode(huffmanTree.getRoot(), huffmanCode,codePath );
}
void HuffmanCode::getHuffmanCode(TreeNode<unsigned char>* root,std::string* huffmanCode, const std::string& codePath) {
        //�����ǰ�ڵ���Ҷ�ӽڵ㣬�洢����
        if (root->leftChild == nullptr && root->rightChild == nullptr) {
            huffmanCode[root->data] = codePath;
            return;
        }
        //��������� '0'
        getHuffmanCode(root->leftChild, huffmanCode,codePath + '0' );
        //��������� '1'
        getHuffmanCode(root->rightChild, huffmanCode,codePath + '1');
    }
