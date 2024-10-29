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
#include <bitset>

class HuffmanCode {
public:
    std::string _fileName;
    BinaryTree<unsigned char> huffmanTree;//根据文件创建的哈夫曼树
    //std::string preorder;//储存哈夫曼树前序遍历，空孩子用#表示，方便重新构建树
    std::vector<unsigned int> byteFrequence;//储存每种字节出现频率
    std::string huffmanCode[256];//存储每一位BYTE对应的编码，也可用std::unorder_map实现
    //根据频率创建二叉树
    void createHuffman(const std::vector<unsigned int>& frequence);
    //从文件里读取size大小字节到内存中
    void readFile(const std::string& fileName, std::vector<char>& charSet, int size);
    //统计字符集里各BYTE[0-255]出现的次数，vector的第n位就代表第BYTE值为n
    void charFrequence(const std::vector<char>& charSet, std::vector<unsigned int>& frequence);
    void charFrequence(const std::vector<char>& charSet) { charFrequence(charSet, byteFrequence); }

    //使用递归方式获取哈夫曼编码
    void getHuffmanCode();
    void getHuffmanCode(TreeNode<unsigned char>* root, std::string* huffmanCode, const std::string& codePath);

public:
    HuffmanCode() :byteFrequence(256, 0){}
    HuffmanCode(const std::string& fileName) :_fileName(fileName), byteFrequence(256, 0) {};
    //压缩函数
    void compress(const std::string& outputFileName) {
        std::ifstream inputFile(_fileName, std::ios::binary);
        if (!inputFile) {
            throw std::runtime_error("Failed to open the file!");
            return;
        }

        std::vector<char> charSet((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        //统计字符频率
        charFrequence(charSet);
        //创建哈夫曼树
        createHuffman(byteFrequence);
        //获取哈夫曼编码
        getHuffmanCode();

        //创建输出文件
        std::ofstream outputFile(outputFileName, std::ios::binary);
        if (!outputFile) {
            throw std::runtime_error("Filed to create output file: ") ;
            return;
        }
        //第一行对应文件名
        outputFile << _fileName;


        //将字符转为哈夫曼编码
        std::string encodedData;
        for (char c : charSet) {
            encodedData += huffmanCode[static_cast<unsigned char>(c)];
        }
        //将编码后的数据按字节写入文件
        std::vector<unsigned char> outputBytes;
        unsigned char Byte=0;
        int bitCount = 0;
        for (char bit : encodedData) {
            Byte = (bit == '1') ? (Byte << 1) | 1 : (Byte << 1) | 0;//实现字符到二进制的转换
            bitCount++;
            if (bitCount == 8) {//每8位生成一个字节
                outputBytes.push_back(Byte);
                Byte = 0;
                bitCount = 0;
            }
        }
        //最后结束如果不满8位的话
        if (bitCount > 0) {
            Byte = Byte << (8 - bitCount);
            outputBytes.push_back(Byte);
        }

        //写入压缩后的字节到文件
        outputFile.write(reinterpret_cast<const char*>(outputBytes.data()), outputBytes.size());
        outputFile.close();
    }
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

