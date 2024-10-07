#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>//用于格式化输出
//定义双向链表节点
struct Node {
    int data;//存储一位数字
    Node* next;//指向下一个节点
    Node* prev;//指向前一个节点

    Node(int value=0) : data(value), next(nullptr), prev(nullptr) {}
};

class BigNumber {
public:
    Node* head;//链表头指针，存储最低位
    Node* tail;//链表尾指针，存储最高位
    unsigned int size = 0;
    int num;
    BigNumber() {
        head = new Node(1);//初始化为1
        tail = head;//初始时头尾相同
        size = 1;
    }

    //大数乘法：将当前大数与num相乘
    void multiply(int num) {
        Node* curr = tail;//从末位数开始相乘
        int carry = 0;//进位
        while (curr) {
            int temp = curr->data * num + carry;
            curr->data = (temp) % 1000;
            carry = (temp) / 1000;
            curr = curr->prev;
        }
        while (curr == nullptr && carry != 0) {
            head->prev = new Node(0);//到达最高位还有进位就增加节点
            size += 1;
            head->prev->next = head;
            head = head->prev;
            head->data = carry % 1000;
            carry = carry / 1000;
        }
    }

    //大数阶乘
    void factorial(int _num);

    //输出大数
    void print() const;

    //释放链表内存
    ~BigNumber() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};



void BigNumber:: print() const {
    std::string fileName = std::to_string(num) + "的阶乘.txt";
    Node* current = head;
    //先输出最高位，不需要填充0
    std::ofstream outPut;
    outPut.open(fileName);
    outPut << "节点数为：" + std::to_string(size) << std::endl;
    outPut << "前十个节点为：" << current->data << ',';
    current = current->next;

    //输出剩余的部分，需要确保每个节点都输出三位数，不足补0
    int node = 0;
    while (current && node < 10) {
        outPut << std::setw(3) << std::setfill('0') << current->data << ',';
        current = current->next;
        node++;
    }
    outPut << std::endl;
}

void BigNumber::factorial(int _num)
{
    num = _num;
    for (int i = 1; i <= _num; i++) {
        this->multiply(i);
    }
    this->print();
}