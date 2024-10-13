//Pixel_Group.h
#pragma once

#include <iostream>
#include <fstream>
#include "MyVector.h"//使用自己定义的vector
#include <string>

using namespace std;

//队列类
template <typename T>
class Queue {
private:
    int front;//头指针
    int rear;//尾指针
    int size; //记录队列个数
    int capacity;//记录容量
    T* queue;
public:
    Queue(int cap) : front(0), rear(0), size(0), capacity(cap) {
        queue = new T[cap];
    }

    ~Queue() {
        delete[] queue;
    }

    //判断队空
    bool isEmpty() {
        return size == 0;
    }

    //判断队满
    bool isFull() {
        return size == capacity;
    }

    //入队
    void enqueue(T value) {
        if (!isFull()) {
            queue[rear] = value;
            rear = (rear + 1) % capacity;
            size++;
        }
    }

    //出队
    T dequeue() {
        if (!isEmpty()) {
            T temp = queue[front];
            front = (front + 1) % capacity;
            size--;
            return temp;
        }
        return T();  //防止出错只能返回一个默认值了
    }
};

//像元分组类
class PixelGrouper {
private:
    int rows;//记录行数
    int cols;//记录列数
    myVector<myVector<int>> image;
    myVector<myVector<int>> labels;
    float version = 0.1;
    int dx[4] = { -1, 1, 0, 0 };//使用数组表示方向
    int dy[4] = { 0, 0, -1, 1 };

public:
    //解析文件头并读取图像数据
    void loadImage(const string& filename) {
        ifstream inFile(filename);
        string line;

        //读取头部信息，直到提示结束
        while (getline(inFile, line)) {
            if (line == "IMAGE_HEADER_END") break;
            //获取行数
            if (line.substr(0, 4) == "Rows") rows = stoi(line.substr(6));
            //获取列数
            if (line.substr(0, 4) == "Cols") cols = stoi(line.substr(6));
        }

        //将文件存到二维数组里

        image.resize(rows, myVector<int>(cols));
        labels.resize(rows, myVector<int>(cols, 0)); //初始化标记矩阵

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                inFile >> image[i][j];
                labels[i][j] = 0;
            }
        }
    }

    //检查是否在有效边界内
    bool isValid(int x, int y) {
        return (x >= 0 && x < rows && y >= 0 && y < cols);
    }

    //进行分组
    void group(int startX, int startY, int groupNum) {
        Queue<pair<int, int>> q(rows * cols);
        q.enqueue({ startX, startY });
        labels[startX][startY] = groupNum;

        while (!q.isEmpty()) {
            pair<int, int> temp = q.dequeue();
            int x = temp.first;
            int y = temp.second;
            for (int i = 0; i < 4; ++i) {
                int newX = x + dx[i];
                int newY = y + dy[i];
                if (isValid(newX, newY) && image[newX][newY] == 1 && labels[newX][newY] == 0) {
                    labels[newX][newY] = groupNum;
                    q.enqueue({ newX, newY });
                }
            }
        }
    }

    //对图像进行像元分组
    void groupPixels() {
        int groupNum = 1;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {

                if (image[i][j] == 1 && labels[i][j] == 0) {//如果出现1并且未被检查过
                    group(i, j, groupNum);
                    groupNum++;
                }
            }
        }
    }

    // 将结果输出到文件
    void writeLabels(const string& outputFilename) {
        ofstream outfile(outputFilename);
        outfile << "IMAGE_HEADER_BEGIN" << std::endl;
        outfile << "Version: " << version << std::endl;
        outfile << "Rows: " << rows << std::endl;
        outfile << "Cols: " << cols << std::endl;
        outfile << "IMAGE_HEADER_END" << std::endl;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                outfile << labels[i][j] << " ";
            }
            outfile << endl;
        }
    }
};
