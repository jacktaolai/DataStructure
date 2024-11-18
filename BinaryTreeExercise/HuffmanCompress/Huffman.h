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
    std::string _fileName;//�ļ���
    size_t _avaiableMermory;//�����ڴ�
    size_t _fileSize=0;//�ļ���С
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
    void charFrequence(unsigned int start, unsigned int end, std::vector<unsigned int>& frequence);
    void charFrequence(const std::string& charSet) { charFrequence(charSet, byteFrequence); }
    //��ȡ�ļ���С
    size_t getFilesize(std::string fileName);
    //ʹ�õݹ鷽ʽ��ȡ����������
    void getHuffmanCode();
    void getHuffmanCode(TreeNode<unsigned char>* root, std::string* huffmanCode, const std::string& codePath);

public:
    HuffmanCode() :byteFrequence(256, 0) { getMemory(); }
    HuffmanCode(const std::string& fileName) :_fileName(fileName), byteFrequence(256, 0) { 
        _avaiableMermory=getMemory(); 
        _threadNum = getCpuCoreCount();
        _fileSize=getFilesize(_fileName);
    };
    //ѹ������
    void compress(const std::string& outputFileName);
    //��ѹ����
    void decompress(const std::string& inputFileName);
    //���߳�ͳ���ַ�Ƶ��
    void parallelCharFrequency();
    //ͨ��WindowsAPI��ȡ�߳���
    int getCpuCoreCount();
    //ͨ��WindowsAPI��ȡ�����ڴ�
    size_t getMemory();
    //���ÿ����ڴ�
    void setMemory(size_t size){_avaiableMermory=size;}
    //���ÿ����߳�
    void setThreadNum(int num){_threadNum=num;}
};

