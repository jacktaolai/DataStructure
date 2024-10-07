#pragma once
#include"Link.h"
#include"Fraction.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <cstdio>


class FreSequence {
	unsigned int order;//阶数
	//储存法雷序列的列表
	Link<Fraction<unsigned int>> freySeq = Link<Fraction<unsigned int>>();
	//记录法雷序列的个数
	unsigned long number = 0;
public:

	//初始化法雷序列
	FreSequence():freySeq() {
		order = 0;
		freySeq.append(Fraction<unsigned int>(0, 1));
		freySeq.append(Fraction<unsigned int>(1, 1));
	}
	//生成n阶法雷序列
	FreSequence(unsigned int _order);
	//获取法雷序列个数
	unsigned long getNumber() { return number; }
	

};
//输出法雷序列并释放内存
void outPut(std::ofstream& outfile, LinkNode<Fraction<unsigned int>>* curr, Link<Fraction<unsigned int>>& freySeq);
//只释放内存
void deleteNode(std::ofstream& outfile, LinkNode<Fraction<unsigned int>>* curr, Link<Fraction<unsigned int>>& freySeq);
//修改第一行
int modifyFirstLine(const std::string& filePath, const std::string& newLine);

FreSequence::FreSequence(unsigned int _order){
	//输出到文件里
	std::string filename = std::to_string(_order) + "阶的法雷序列.txt";
	std::ofstream outfile;
	outfile.open(filename);
	outfile << std::endl;//为后续输出个数预留一行
	freySeq.append(Fraction<unsigned int>(0, 1));
	freySeq.append(Fraction<unsigned int>(1, 1));
	order = _order;
	number = 2;//记录法雷序列的个数
	unsigned long listnumber = 2;//链表的长度
	std::thread outputThread;  //用于输出的线程
	//使用函数指针确定是只输出个数还是全部输出法雷序列
	void(*function)(std::ofstream & outfile, LinkNode<Fraction<unsigned int>>*curr, Link<Fraction<unsigned int>>&freySeq) = nullptr;
	if (_order > 5000) function = deleteNode; 
	else{
		function = outPut;
	}
	LinkNode<Fraction<unsigned int>>* curr = freySeq.first();
	while (curr->succ != nullptr) {
		unsigned int a = curr->data.numerator;//当前节点分子
		unsigned int b = curr->data.denominator;//当前节点分母
		unsigned int c = curr->succ->data.numerator;//下一节点分子
		unsigned int d = curr->succ->data.denominator;//下一节点分母
		if (b + d <= order) {//如果分母相加小于阶数就插入
			//插入节点
			LinkNode<Fraction<unsigned int>>* tempPtr = curr->succ;
			curr->succ = new LinkNode<Fraction<unsigned int>>(Fraction<unsigned int>(a + c, b + d));
			number += 1;
			listnumber += 1;
			curr->succ->succ = tempPtr;
			c = curr->succ->data.numerator;//下一节点分子
			d = curr->succ->data.denominator;//下一节点分母
		}
		else {
			curr = curr->succ;
		}
		//每10000个节点输出一次
		if (listnumber == 10000|| curr->succ == nullptr) {
			if (outputThread.joinable()) {
				outputThread.join();  //等待之前的输出线程完成
			}
			//启动线程输出
			outputThread = std::thread(function, std::ref(outfile), curr, std::ref(freySeq));
			listnumber = 1;  //重置计数器
		}
	}

	//确保最后的输出线程完成
	if (outputThread.joinable()) {
		outputThread.join();
	}
	std::string count = std::to_string(_order) + "阶的法雷序列总共有" + std::to_string(number) + "个";
	modifyFirstLine(filename, count);
	outfile.close();
	std::remove(filename.c_str()); //删除原文件
	std::rename((filename+".tmp").c_str(), filename.c_str()); //重命名临时文件
}


void outPut(std::ofstream&outfile,LinkNode<Fraction<unsigned int>>*curr, Link<Fraction<unsigned int>> &freySeq) {
		
	LinkNode<Fraction<unsigned int>>* temp = freySeq.first();
		while (temp != curr) {//输出节点
			outfile << temp->data << ' ';
			//删除已经打印的节点
			LinkNode<Fraction<unsigned int>>* temp2 = temp->succ;
			delete temp;
			temp = temp2;
		}
		outfile << std::endl;
		freySeq.header = curr;
		if(curr->succ==nullptr)	outfile << "1/1"<<std::endl;
}

void deleteNode(std::ofstream& outfile, LinkNode<Fraction<unsigned int>>* curr, Link<Fraction<unsigned int>>& freySeq){
	
	LinkNode<Fraction<unsigned int>>* temp = freySeq.first();
	while (temp != curr) {
		LinkNode<Fraction<unsigned int>>* temp2 = temp->succ;
		delete temp;
		temp = temp2;
	}
	freySeq.header = curr;
}


int modifyFirstLine(const std::string& filePath, const std::string& newLine) {
	if (filePath.empty() || newLine.empty()) {
		return -1; //错误处理
	}

	//创建临时文件
	std::string tempFilePath = filePath + ".tmp";
	std::ofstream tempFile(tempFilePath);
	if (!tempFile.is_open()) {
		std::cerr << "无法创建临时文件" << std::endl;
		return -1;
	}

	//替换第一行
	tempFile << newLine << "\n"; //写入新行

	//读取原文件并写入临时文件
	std::ifstream inputFile(filePath);
	if (!inputFile.is_open()) {
		std::cerr << "无法打开原文件" << std::endl;
		return -1;
	}

	std::string line;
	//跳过原文件的第一行
	std::getline(inputFile, line);
	while (std::getline(inputFile, line)) {
		tempFile << line << "\n"; //写入其余行
	}

	inputFile.close();
	tempFile.close();

	//替换原文件


	return 0; //成功
}
