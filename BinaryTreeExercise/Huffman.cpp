//Huffman.cpp
#include "Huffman.h"

void HuffmanCode::createHuffman(const std::vector<unsigned int>& frequence) {
    //按频率从小到大创建优先队列，将节点存入队列，节点包含unsigned char和其对应的频率
    PriorityLinkQueue<TreeNode<unsigned char>*> forestRoot;
    for (int i = 0; i <= 255; ++i) {
        forestRoot.enqueue(new TreeNode<unsigned char>(i, frequence[i]), frequence[i]);
    }
    while (forestRoot.getSize() > 1) {
        //从森林里取俩最小的权值
        TreeNode<unsigned char>* leftChild = forestRoot.dequeue();
        TreeNode<unsigned char>* rightChild = forestRoot.dequeue();
        //以两个孩子的权值作为权值
        //此处的非叶子节点统一数据存0，以后通过判断有无左右孩子判断是否子节点
        TreeNode<unsigned char>* newNode = new TreeNode<unsigned char>(0, leftChild->priority + rightChild->priority);
        newNode->leftChild = leftChild;
        newNode->rightChild = rightChild;
        forestRoot.enqueue(newNode, newNode->priority);
    }
    if (forestRoot.getSize() == 1) {
        huffmanTree.rebuildTree(forestRoot.dequeue());
    }
}




std::mutex mtx;  // 互斥锁，用于合并结果时的线程同步

// 单线程统计字符频率
//1G文件1分五十五
static void countFrequencyInChunk(std::string filename, std::unordered_map<char, int>& freqMap) {
    std::ifstream file(filename, std::ios::binary); // 二进制和定位文件末尾
    char ch;
    int i = 0;
    while (i <= 1024 * 1024 * 1024) {
        file.get(ch);
        //std::lock_guard<std::mutex> lock(mtx); // 锁定以更新频率
        freqMap[ch]++;
        ++i;
    }

}

void HuffmanCode::readFile(const std::string& fileName, std::vector<char>& charSet, int size) {
    std::ifstream file;
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Failed to open the file!");
    file.read(charSet.data(), size);
    //获取实际读取的字节数
    std::streamsize bytesRead = file.gcount();
    // 打印读取的字节数
    std::cout << "实际读取的字节数: " << bytesRead << std::endl;
    file.close();
}
void HuffmanCode::charFrequence(const std::vector<char>& charSet, std::vector<unsigned int>& frequence) {
    for (unsigned char byte : charSet) {
        frequence[byte]++;
    }
}

void HuffmanCode::getHuffmanCode(){
    if (huffmanTree.getRoot() == nullptr) throw std::runtime_error("Tree is empty!");
    std::string codePath = "";//左为0右为1
    getHuffmanCode(huffmanTree.getRoot(), huffmanCode,codePath );
}
void HuffmanCode::getHuffmanCode(TreeNode<unsigned char>* root,std::string* huffmanCode, const std::string& codePath) {
        //如果当前节点是叶子节点，存储编码
        if (root->leftChild == nullptr && root->rightChild == nullptr) {
            huffmanCode[root->data] = codePath;
            return;
        }
        //左子树则加 '0'
        getHuffmanCode(root->leftChild, huffmanCode,codePath + '0' );
        //右子树则加 '1'
        getHuffmanCode(root->rightChild, huffmanCode,codePath + '1');
    }
