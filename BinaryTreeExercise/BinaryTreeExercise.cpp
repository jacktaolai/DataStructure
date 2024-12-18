﻿// BinaryTreeExercise.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//author: 未名湖居士
//e-mail: jacktaolai@foxmail.com
//Style: Google Style Guide
//Licence: MIT
//version: 0.1

#include <iostream>
#include "BinaryTree.h"
#include "LinkQueue.h"
#include "Huffman.h"
void printFrequencyMap(const std::unordered_map<char, int>& freqMap) {
    int i = 0;
    for (const auto& pair : freqMap) {
        std::cout <<i<< "   字符: '" << pair.first << "' 出现次数: " << pair.second << '\n';
        ++i;
    }
}
int main()
{
    LinkQueue<int> test2;
    test2.enqueue(2);
    test2.enqueue(3);
    test2.dequeue();


    BinaryTree<char> test;
    test.createFromPreorder("ABC####");
    test.createFromLevelorder("AB#CD##EF#G####");  
    //test.preVisit();
    Stack<TreeNode<char>*> ancestor = test.find('E');
    test.contructTree("ABDEGCFH", "DBGEACHF");
    std::string pre = test.getPreorder();
    std::cout <<pre<<std::endl<< "Hello World!\n";


    std::string filename = "test2.huf";
    //int numThreads = 16; // 线程数，可根据CPU核心数调整
    std::vector<char> charSet(11388960);
    std::vector<unsigned int> fre(256);
    //HuffmanCode huff("pakchunk11-Windows.pak");
    //HuffmanCode huff("PixPin_2024-10-30_03-27-26.png");
    //HuffmanCode huff("未来城正射影像-2.jpg");
    //HuffmanCode huff("test2.huf");

    //HuffmanCode huff("新建文件夹");
    //HuffmanCode huff("D:/Calculator.h");
    //HuffmanCode huff("新建 文本文档.txt");

    //HuffmanCode huff("tif2002Raw_re.tif");
    HuffmanCode huff("2020年全国_GLC30_30米土地覆盖数据.tif");
    //huff.multithreading();
    //huff.readFile("编码解码-英文测试数据.txt", charSet, 1138860 );
    //huff.charFrequence(charSet, fre);
    //huff.createHuffman(fre);
    //huff.getHuffmanCode();
    //huff._avaiableMermory = 10*1024*1024;
    std::cout << "可用内存"<<huff._avaiableMermory / 1024 / 1024 / 1024 << std::endl;
    huff.compress(filename);
    for (int i = 0; i < 256; ++i) {

        std::cout << "huffmanCode[" << i << "] = " << huff.huffmanCode[i] << '\n';

    }
    //huff.preorder  = huff.huffmanTree.getPreorder();
    //std::cout << huff.preorder << std::endl;
   // auto frequencyMap = countCharacterFrequencyMultiThreaded(filename, numThreads);
    //std::unordered_map<char, int> freqMap;
    //countFrequencyInChunk(filename, freqMap);
    //printFrequencyMap(freqMap);
    //huff.decompress("test2.huf");
     // 循环输出每个非空编码
    for (int i = 0; i < 256; ++i) {
  
            std::cout << "huffmanCode[" << i << "] = " << huff.huffmanCode[i] << '\n';

    }
    for (int i = 0; i < 256; ++i) {
        std::cout << huff.byteFrequence[i]<<std::endl;
    }
}
