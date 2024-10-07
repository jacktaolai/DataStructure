//author: 未名湖居士
//e-mail: jacktaolai@foxmail.com
//Style: Google Style Guide
//Licence: MIT
//version: 0.3
/*************************/
/*    十万阶用时：2分56秒  */

#include <iostream>

#include "Link.h"
#include"Fraction.h"
#include"FreSequence.h"
#include "BigNumber.h"
//法雷序列
static void fre(unsigned int n) {
    FreSequence a=n;
    std::cout << n << "阶的法雷序列已经生成" << std::endl;
}
//大数阶乘
static void fac(int m) {
    BigNumber a;
    a.factorial(m);
    std::cout << m << "阶的阶乘已经生成"<<std::endl;

}
int main() {
    //基本功能测试
  
    //创建链表
    Link<int> list1;
    list1.append(5);
    list1.append(3);
    list1.append(1);

    Link<int> list2;
    list2.append(6);
    list2.append(4);
    list2.append(2);

    //打印链表内容
    std::cout << "List 1 in order: ";
    list1.printInOrderRecursive();
    std::cout << "List 1 in reverse order: ";
    list1.printInReverseOrderRecursive();
    std::cout << std::endl;

    std::cout << "List 2 in order: ";
    list2.printInOrderRecursive();
    std::cout << "List 2 in reverse order: ";
    list2.printInReverseOrderRecursive();
    std::cout << std::endl;

    //合并链表
    Link<int> mergedList = mergeSortedLink(list1, list2);
    std::cout << "Merged list in order: ";
    mergedList.printInOrderRecursive();
    std::cout << "Merged list in reverse order: ";
    mergedList.printInReverseOrderRecursive();
    std::cout << std::endl;

    //逆置链表
    mergedList.reverse();
    std::cout << "Reversed merged list in order: ";
    mergedList.printInOrderRecursive();
    std::cout << "Reversed merged list in reverse order: ";
    mergedList.printInReverseOrderRecursive();
    std::cout << std::endl;
    


    std::thread event[12];
    event[0] = std::thread(fre, 1000);
    event[1] = std::thread(fre, 5000);
    event[2] = std::thread(fre, 10000);
    event[3] = std::thread(fre, 20000);
    event[4] = std::thread(fre, 50000);
    event[5] = std::thread(fre, 100000);
    event[6] = std::thread(fac, 1000);
    event[7] = std::thread(fac, 5000);
    event[8] = std::thread(fac, 10000);
    event[9] = std::thread(fac, 20000);
    event[10] = std::thread(fac, 50000);
    event[11] = std::thread(fac, 100000);
    for (int i = 0; i < 12; i++) {
        if (event[i].joinable()) {
            event[i].join();
        }
    }
    return 0;
}

