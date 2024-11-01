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
    std::string _fileName;//�ļ���
    unsigned int _avaiableMermory;//�����ڴ�
    unsigned int _fileSize=0;//�ļ���С
    unsigned int _threadNum=16;//�߳���
    std::mutex mtx;//������
    BinaryTree<unsigned char> huffmanTree;//�����ļ������Ĺ�������
    //std::string preorder;//�����������ǰ��������պ�����#��ʾ���������¹�����
    std::vector<unsigned int> byteFrequence;//����ÿ���ֽڳ���Ƶ��
    std::string huffmanCode[256];//�洢ÿһλBYTE��Ӧ�ı��룬Ҳ����std::unorder_mapʵ��
    //����Ƶ�ʴ���������
    void createHuffman(const std::vector<unsigned int>& frequence);
    //���ļ����ȡsize��С�ֽڵ��ڴ���
    void readFile(const std::string& fileName, std::vector<char>& charSet, int size);
    //ͳ���ַ������BYTE[0-255]���ֵĴ�����vector�ĵ�nλ�ʹ����BYTEֵΪn
    void charFrequence(const std::string& charSet, std::vector<unsigned int>& frequence);
    void charFrequence(const std::string& charSet, std::vector<unsigned int>& frequence, unsigned int start,unsigned int end);
    void charFrequence(const std::string& charSet) { charFrequence(charSet, byteFrequence); }
    //ͨ��WindowsAPI��ȡ�����ڴ�
    void getMemory();
    //��ȡ�ļ���С
    void getFilesize();
    //ʹ�õݹ鷽ʽ��ȡ����������
    void getHuffmanCode();
    void getHuffmanCode(TreeNode<unsigned char>* root, std::string* huffmanCode, const std::string& codePath);

public:
    HuffmanCode() :byteFrequence(256, 0) { getMemory(); }
    HuffmanCode(const std::string& fileName) :_fileName(fileName), byteFrequence(256, 0) { 
        getMemory(); 
        getFilesize();
    };
    //ѹ������
    void compress(const std::string& outputFileName);
    //��ѹ����
    void decompress(const std::string& inputFileName);
    //���߳�
    void multithreading();
};





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

