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




std::mutex mtx;  // �����������ںϲ����ʱ���߳�ͬ��

// ���߳�ͳ���ַ�Ƶ��
//1G�ļ�1����ʮ��
void countFrequencyInChunk(std::string filename, std::unordered_map<char, int>& freqMap) {
    std::ifstream file(filename, std::ios::binary ); // �����ƺͶ�λ�ļ�ĩβ
    char ch;
    int i = 0;
    while (i<=1024*1024*1024) {
        file.get(ch);
        //std::lock_guard<std::mutex> lock(mtx); // �����Ը���Ƶ��
        freqMap[ch]++;
        ++i;
    }
   
}
//���ļ����ȡsize��С�ֽڵ��ڴ���
void readFile(const std::string& fileName, std::vector<char>& charSet,int size) {
    std::ifstream file;
    file.open(fileName, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Failed to open the file!");
    file.read(charSet.data(), size );
    //��ȡʵ�ʶ�ȡ���ֽ���
    std::streamsize bytesRead = file.gcount();
    // ��ӡ��ȡ���ֽ���
    std::cout << "ʵ�ʶ�ȡ���ֽ���: " << bytesRead << std::endl;
    file.close();
}
//ͳ���ַ������BYTE[0-255]���ֵĴ�����vector�ĵ�nλ�ʹ����BYTEֵΪn
void charFrequence(const std::vector<char>& charSet, std::vector<unsigned int>& frequence) {
    for (unsigned char byte : charSet) {
        frequence[byte]++;
    }
}

void createHuffman(const std::vector<unsigned int>& frequence) {
    //��Ƶ�ʴ�С���󴴽����ȶ��У����ڵ������У��ڵ����unsigned char�����Ӧ��Ƶ��
    PriorityLinkQueue<TreeNode<unsigned char>*> forestRoot;
    for (int i = 0; i <= 255; ++i) {
        forestRoot.enqueue(new TreeNode<unsigned char>(i, frequence[i]),frequence[i]);
    }
    while (forestRoot.getSize() > 1) {
        //��ɭ����ȡ����С��Ȩֵ
        TreeNode<unsigned char>* leftChild = forestRoot.dequeue();
        TreeNode<unsigned char>* rightChild = forestRoot.dequeue();
        //���������ӵ�Ȩֵ��ΪȨֵ
        TreeNode<unsigned char>* newNode = new TreeNode<unsigned char>(0, leftChild->priority + rightChild->priority);
        newNode->leftChild = leftChild;
        newNode->rightChild = rightChild;
        forestRoot.enqueue(newNode, newNode->priority);
    }
    if (forestRoot.getSize() == 1) {
        BinaryTree<unsigned char> huffmanTree(forestRoot.dequeue());
    }
}
//������������

// //���߳��ַ�ͳ��������
//std::unordered_map<char, int> countCharacterFrequencyMultiThreaded(const std::string& filename, int numThreads) {
//    std::unordered_map<char, int> freqMap;
//    std::ifstream file(filename, std::ios::binary | std::ios::ate); // �����ƺͶ�λ�ļ�ĩβ
//
//    if (!file.is_open()) {
//        std::cerr << "Failed to open the file: " << filename << std::endl;
//        return freqMap;
//    }
//
//    std::streampos fileSize = file.tellg();
//    std::vector<std::thread> threads;
//    std::vector<std::unordered_map<char, int>> localFreqMaps(numThreads); // ÿ���̵߳ľֲ�Ƶ�ʱ�
//
//    // �ֿ��С
//    std::streampos chunkSize = fileSize / numThreads;
//
//    for (int i = 0; i < numThreads; ++i) {
//        std::streampos start = i * chunkSize;
//        std::streampos end = (i == numThreads - 1) ? fileSize : start + chunkSize;
//        threads.emplace_back(countFrequencyInChunk, std::ref(file), std::ref(localFreqMaps[i]), start, end);
//    }
//
//    // �ȴ������߳����
//    for (auto& t : threads) {
//        t.join();
//    }
//
//    // �ϲ������̵߳�Ƶ�ʱ�
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

