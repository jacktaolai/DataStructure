//RearrangingrailroadCars

#include <iostream>
#include "Stack.h"

//处理车厢排序的函数
void rearrangeCars(int carList[], int carNum, int holdNum/*=3*/) {
    Stack<int> cars (carList,carNum);
    //创建三个缓冲栈
    Stack<int>* holdingTracks=new Stack<int>[holdNum];//缓冲轨道H1,H2,H3
    Stack<int> outputTrack;//输出轨道
    int nextCarToOutput = 1;//期望输出的车厢编号

    //处理每个输入轨道上的车厢
    while (!cars.isEmpty()) {
        int car = cars.pop();//弹出车厢

        //尝试先从缓冲栈中输出符合条件的车厢
        for (int i = 0; i < holdNum; ++i) {
            while (!holdingTracks[i].isEmpty() && holdingTracks[i].getTop() == nextCarToOutput) {
                outputTrack.push(holdingTracks[i].pop()); 
                std::cout << nextCarToOutput <<"号车" << "从H" << (i + 1) << "移到出轨" << std::endl;
                nextCarToOutput++;
            }
        }

        //如果车厢可以直接输出
        if (car == nextCarToOutput) {
            outputTrack.push(car);
            std::cout << car<<"号车" << "直接移到出轨" << std::endl;
            nextCarToOutput++;
        }
        else {
            //尝试将车厢放入合适的缓冲栈
            int bestTrack = -1;  //记录最合适的缓冲栈编号
            for (int i = 0; i < holdNum; ++i) {
                //如果缓冲栈为空，记录空的缓冲栈位置
                if (holdingTracks[i].isEmpty()) {
                    if (bestTrack == -1) {
                        bestTrack = i;  //找到一个空缓冲栈
                    }
                }
                //如果栈顶元素比当前车厢编号大，且是最小的合适栈顶
                else if (holdingTracks[i].getTop() > car) {
                    if (!holdingTracks[bestTrack].isEmpty()&&(bestTrack == -1 || holdingTracks[i].getTop() < holdingTracks[bestTrack].getTop())) {
                        bestTrack = i;  //找到更合适的缓冲栈
                    }
                }
            }

            //如果找到合适的缓冲栈，放入该缓冲栈
            if (bestTrack != -1) {
                holdingTracks[bestTrack].push(car);
                std::cout  << car <<"号车" << "移到缓冲轨H" << (bestTrack + 1) << "." << std::endl;
            }
            else {
                //没有找到合适的缓冲栈，说明排序失败
                std::cout  << car<<"号车无法移到合适的缓冲轨，说明"<<holdNum<<"条铁轨无法完成这个序列的重排" << std::endl;
                return rearrangeCars(carList, carNum, holdNum+1);
                delete holdingTracks;
                break;
            }
        }
    }

    //最后检查所有缓冲栈中是否还有符合条件的车厢可以输出
    for (int i = 0; i < holdNum; ++i) {
        while (!holdingTracks[i].isEmpty() && holdingTracks[i].getTop() == nextCarToOutput) {
            outputTrack.push(holdingTracks[i].pop());
            std::cout << nextCarToOutput<<"号车" << "从缓冲轨H" << (i + 1) << "移到出轨" << std::endl;
            nextCarToOutput++;
        }
    }

    //检查排序是否完成
    if (nextCarToOutput > outputTrack.getSize()) {
        std::cout << "排序完成！" << std::endl;
    }
    else {
        std::cout << "排序失败" << std::endl;
    }
}

