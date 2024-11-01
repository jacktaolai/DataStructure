#pragma once
//Huffman
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <thread>
#include <mutex>
#include "BinaryTree.h"
#include "PriorityQueue.h"
#include <bitset>

class HuffmanCode {
public:
    std::string _fileName;//文件名
    unsigned int _avaiableMermory;//可用内存
    unsigned int _fileSize=0;//文件大小
    unsigned int _threadNum=16;//线程数
    std::mutex mtx;//互斥锁
    BinaryTree<unsigned char> huffmanTree;//根据文件创建的哈夫曼树
    //std::string preorder;//储存哈夫曼树前序遍历，空孩子用#表示，方便重新构建树
    std::vector<unsigned int> byteFrequence;//储存每种字节出现频率
    std::string huffmanCode[256];//存储每一位BYTE对应的编码，也可用std::unorder_map实现
    //根据频率创建二叉树
    void createHuffman(const std::vector<unsigned int>& frequence);
    //从文件里读取size大小字节到内存中
    void readFile(const std::string& fileName, std::vector<char>& charSet, int size);
    //统计字符集里各BYTE[0-255]出现的次数，vector的第n位就代表第BYTE值为n
    void charFrequence(const std::string& charSet, std::vector<unsigned int>& frequence);
    void charFrequence(const std::string& charSet, std::vector<unsigned int>& frequence, unsigned int start,unsigned int end);
    void charFrequence(const std::string& charSet) { charFrequence(charSet, byteFrequence); }
    //通过WindowsAPI获取可用内存
    void getMemory();
    //获取文件大小
    void getFilesize();
    //使用递归方式获取哈夫曼编码
    void getHuffmanCode();
    void getHuffmanCode(TreeNode<unsigned char>* root, std::string* huffmanCode, const std::string& codePath);

public:
    HuffmanCode() :byteFrequence(256, 0) { getMemory(); }
    HuffmanCode(const std::string& fileName) :_fileName(fileName), byteFrequence(256, 0) { 
        getMemory(); 
        getFilesize();
    };
    //压缩函数
    void compress(const std::string& outputFileName);
    //解压函数
    void decompress(const std::string& inputFileName);
    //多线程
    void multithreading();
};





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

