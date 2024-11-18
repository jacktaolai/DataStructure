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

class HuffmanCode {
public:
    std::string _fileName;//文件名
    size_t _avaiableMermory;//可用内存
    size_t _fileSize=0;//文件大小
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
    void charFrequence(unsigned int start, unsigned int end, std::vector<unsigned int>& frequence);
    void charFrequence(const std::string& charSet) { charFrequence(charSet, byteFrequence); }
    //获取文件大小
    size_t getFilesize(std::string fileName);
    //使用递归方式获取哈夫曼编码
    void getHuffmanCode();
    void getHuffmanCode(TreeNode<unsigned char>* root, std::string* huffmanCode, const std::string& codePath);

public:
    HuffmanCode() :byteFrequence(256, 0) { getMemory(); }
    HuffmanCode(const std::string& fileName) :_fileName(fileName), byteFrequence(256, 0) { 
        _avaiableMermory=getMemory(); 
        _threadNum = getCpuCoreCount();
        _fileSize=getFilesize(_fileName);
    };
    //压缩函数
    void compress(const std::string& outputFileName);
    //解压函数
    void decompress(const std::string& inputFileName);
    //多线程统计字符频率
    void parallelCharFrequency();
    //通过WindowsAPI获取线程数
    int getCpuCoreCount();
    //通过WindowsAPI获取可用内存
    size_t getMemory();
    //设置可用内存
    void setMemory(size_t size){_avaiableMermory=size;}
    //设置可用线程
    void setThreadNum(int num){_threadNum=num;}
};

