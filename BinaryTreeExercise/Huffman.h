#pragma once
//Huffman
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include "BinaryTree.h"
#include "PriorityQueue.h"

//class HuffmanCode {
//private:
//    BinaryTree<unsigned char> 
//};




std::mutex mtx;  // 互斥锁，用于合并结果时的线程同步

// 单线程统计字符频率
//1G文件1分五十五
void countFrequencyInChunk(std::string filename, std::unordered_map<char, int>& freqMap) {
    std::ifstream file(filename, std::ios::binary ); // 二进制和定位文件末尾
    char ch;
    int i = 0;
    while (i<=1024*1024*1024) {
        file.get(ch);
        //std::lock_guard<std::mutex> lock(mtx); // 锁定以更新频率
        freqMap[ch]++;
        ++i;
    }
   
}
//从文件里读取size大小字节到内存中
void readFile(const std::string& fileName, std::vector<char>& charSet,int size) {
    std::ifstream file;
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Failed to open the file!");
    file.read(charSet.data(), size );
    //获取实际读取的字节数
    std::streamsize bytesRead = file.gcount();
    // 打印读取的字节数
    std::cout << "实际读取的字节数: " << bytesRead << std::endl;
    file.close();
}
//统计字符集里各BYTE[0-255]出现的次数，vector的第n位就代表第BYTE值为n
void charFrequence(const std::vector<char>& charSet, std::vector<unsigned int>& frequence) {
    for (unsigned char byte : charSet) {
        frequence[byte]++;
    }
}

void createHuffman(const std::vector<unsigned int>& frequence) {
    //按频率从小到大创建优先队列，将节点存入队列，节点包含unsigned char和其对应的频率
    PriorityLinkQueue<TreeNode<unsigned char>*> forestRoot;
    for (int i = 0; i <= 255; ++i) {
        forestRoot.enqueue(new TreeNode<unsigned char>(i, frequence[i]),frequence[i]);
    }
    while (forestRoot.getSize() > 1) {
        //从森林里取俩最小的权值
        TreeNode<unsigned char>* leftChild = forestRoot.dequeue();
        TreeNode<unsigned char>* rightChild = forestRoot.dequeue();
        //以两个孩子的权值作为权值
        TreeNode<unsigned char>* newNode = new TreeNode<unsigned char>(0, leftChild->priority + rightChild->priority);
        newNode->leftChild = leftChild;
        newNode->rightChild = rightChild;
        forestRoot.enqueue(newNode, newNode->priority);
    }
    if (forestRoot.getSize() == 1) {
        BinaryTree<unsigned char> huffmanTree(forestRoot.dequeue());
    }
}
//创建哈夫曼树

// //多线程字符统计主函数
//std::unordered_map<char, int> countCharacterFrequencyMultiThreaded(const std::string& filename, int numThreads) {
//    std::unordered_map<char, int> freqMap;
//    std::ifstream file(filename, std::ios::binary | std::ios::ate); // 二进制和定位文件末尾
//
//    if (!file.is_open()) {
//        std::cerr << "Failed to open the file: " << filename << std::endl;
//        return freqMap;
//    }
//
//    std::streampos fileSize = file.tellg();
//    std::vector<std::thread> threads;
//    std::vector<std::unordered_map<char, int>> localFreqMaps(numThreads); // 每个线程的局部频率表
//
//    // 分块大小
//    std::streampos chunkSize = fileSize / numThreads;
//
//    for (int i = 0; i < numThreads; ++i) {
//        std::streampos start = i * chunkSize;
//        std::streampos end = (i == numThreads - 1) ? fileSize : start + chunkSize;
//        threads.emplace_back(countFrequencyInChunk, std::ref(file), std::ref(localFreqMaps[i]), start, end);
//    }
//
//    // 等待所有线程完成
//    for (auto& t : threads) {
//        t.join();
//    }
//
//    // 合并所有线程的频率表
//    for (const auto& localMap : localFreqMaps) {
//        for (const auto& pair : localMap) {
//            freqMap[pair.first] += pair.second;
//        }
//    }
//
//    file.close();
//    return freqMap;
//}
//

