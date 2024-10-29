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
    BinaryTree<unsigned char> huffmanTree;//�����ļ������Ĺ�������
    //std::string preorder;//�����������ǰ��������պ�����#��ʾ���������¹�����
    std::vector<unsigned int> byteFrequence;//����ÿ���ֽڳ���Ƶ��
    std::string huffmanCode[256];//�洢ÿһλBYTE��Ӧ�ı��룬Ҳ����std::unorder_mapʵ��
    //����Ƶ�ʴ���������
    void createHuffman(const std::vector<unsigned int>& frequence);
    //���ļ����ȡsize��С�ֽڵ��ڴ���
    void readFile(const std::string& fileName, std::vector<char>& charSet, int size);
    //ͳ���ַ������BYTE[0-255]���ֵĴ�����vector�ĵ�nλ�ʹ����BYTEֵΪn
    void charFrequence(const std::vector<char>& charSet, std::vector<unsigned int>& frequence);
    void charFrequence(const std::vector<char>& charSet) { charFrequence(charSet, byteFrequence); }

    //ʹ�õݹ鷽ʽ��ȡ����������
    void getHuffmanCode();
    void getHuffmanCode(TreeNode<unsigned char>* root, std::string* huffmanCode, const std::string& codePath);

public:
    HuffmanCode() :byteFrequence(256, 0){}
    HuffmanCode(const std::string& fileName) :_fileName(fileName), byteFrequence(256, 0) {};
    //ѹ������
    void compress(const std::string& outputFileName);
    void decompress(const std::string& inputFileName);
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

