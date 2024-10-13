//Pixel_Group.h
#pragma once

#include <iostream>
#include <fstream>
#include "MyVector.h"//ʹ���Լ������vector
#include <string>

using namespace std;

//������
template <typename T>
class Queue {
private:
    int front;//ͷָ��
    int rear;//βָ��
    int size; //��¼���и���
    int capacity;//��¼����
    T* queue;
public:
    Queue(int cap) : front(0), rear(0), size(0), capacity(cap) {
        queue = new T[cap];
    }

    ~Queue() {
        delete[] queue;
    }

    //�ж϶ӿ�
    bool isEmpty() {
        return size == 0;
    }

    //�ж϶���
    bool isFull() {
        return size == capacity;
    }

    //���
    void enqueue(T value) {
        if (!isFull()) {
            queue[rear] = value;
            rear = (rear + 1) % capacity;
            size++;
        }
    }

    //����
    T dequeue() {
        if (!isEmpty()) {
            T temp = queue[front];
            front = (front + 1) % capacity;
            size--;
            return temp;
        }
        return T();  //��ֹ����ֻ�ܷ���һ��Ĭ��ֵ��
    }
};

//��Ԫ������
class PixelGrouper {
private:
    int rows;//��¼����
    int cols;//��¼����
    myVector<myVector<int>> image;
    myVector<myVector<int>> labels;
    float version = 0.1;
    int dx[4] = { -1, 1, 0, 0 };//ʹ�������ʾ����
    int dy[4] = { 0, 0, -1, 1 };

public:
    //�����ļ�ͷ����ȡͼ������
    void loadImage(const string& filename) {
        ifstream inFile(filename);
        string line;

        //��ȡͷ����Ϣ��ֱ����ʾ����
        while (getline(inFile, line)) {
            if (line == "IMAGE_HEADER_END") break;
            //��ȡ����
            if (line.substr(0, 4) == "Rows") rows = stoi(line.substr(6));
            //��ȡ����
            if (line.substr(0, 4) == "Cols") cols = stoi(line.substr(6));
        }

        //���ļ��浽��ά������

        image.resize(rows, myVector<int>(cols));
        labels.resize(rows, myVector<int>(cols, 0)); //��ʼ����Ǿ���

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                inFile >> image[i][j];
                labels[i][j] = 0;
            }
        }
    }

    //����Ƿ�����Ч�߽���
    bool isValid(int x, int y) {
        return (x >= 0 && x < rows && y >= 0 && y < cols);
    }

    //���з���
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

    //��ͼ�������Ԫ����
    void groupPixels() {
        int groupNum = 1;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {

                if (image[i][j] == 1 && labels[i][j] == 0) {//�������1����δ������
                    group(i, j, groupNum);
                    groupNum++;
                }
            }
        }
    }

    // �����������ļ�
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
