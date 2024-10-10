//RearrangingrailroadCars

#include <iostream>
#include "Stack.h"

//����������ĺ���
void rearrangeCars(int carList[], int carNum, int holdNum/*=3*/) {
    Stack<int> cars (carList,carNum);
    //������������ջ
    Stack<int>* holdingTracks=new Stack<int>[holdNum];//������H1,H2,H3
    Stack<int> outputTrack;//������
    int nextCarToOutput = 1;//��������ĳ�����

    //����ÿ���������ϵĳ���
    while (!cars.isEmpty()) {
        int car = cars.pop();//��������

        //�����ȴӻ���ջ��������������ĳ���
        for (int i = 0; i < holdNum; ++i) {
            while (!holdingTracks[i].isEmpty() && holdingTracks[i].getTop() == nextCarToOutput) {
                outputTrack.push(holdingTracks[i].pop()); 
                std::cout << nextCarToOutput <<"�ų�" << "��H" << (i + 1) << "�Ƶ�����" << std::endl;
                nextCarToOutput++;
            }
        }

        //����������ֱ�����
        if (car == nextCarToOutput) {
            outputTrack.push(car);
            std::cout << car<<"�ų�" << "ֱ���Ƶ�����" << std::endl;
            nextCarToOutput++;
        }
        else {
            //���Խ����������ʵĻ���ջ
            int bestTrack = -1;  //��¼����ʵĻ���ջ���
            for (int i = 0; i < holdNum; ++i) {
                //�������ջΪ�գ���¼�յĻ���ջλ��
                if (holdingTracks[i].isEmpty()) {
                    if (bestTrack == -1) {
                        bestTrack = i;  //�ҵ�һ���ջ���ջ
                    }
                }
                //���ջ��Ԫ�رȵ�ǰ�����Ŵ�������С�ĺ���ջ��
                else if (holdingTracks[i].getTop() > car) {
                    if (!holdingTracks[bestTrack].isEmpty()&&(bestTrack == -1 || holdingTracks[i].getTop() < holdingTracks[bestTrack].getTop())) {
                        bestTrack = i;  //�ҵ������ʵĻ���ջ
                    }
                }
            }

            //����ҵ����ʵĻ���ջ������û���ջ
            if (bestTrack != -1) {
                holdingTracks[bestTrack].push(car);
                std::cout  << car <<"�ų�" << "�Ƶ������H" << (bestTrack + 1) << "." << std::endl;
            }
            else {
                //û���ҵ����ʵĻ���ջ��˵������ʧ��
                std::cout  << car<<"�ų��޷��Ƶ����ʵĻ���죬˵��"<<holdNum<<"�������޷����������е�����" << std::endl;
                return rearrangeCars(carList, carNum, holdNum+1);
                delete holdingTracks;
                break;
            }
        }
    }

    //��������л���ջ���Ƿ��з��������ĳ���������
    for (int i = 0; i < holdNum; ++i) {
        while (!holdingTracks[i].isEmpty() && holdingTracks[i].getTop() == nextCarToOutput) {
            outputTrack.push(holdingTracks[i].pop());
            std::cout << nextCarToOutput<<"�ų�" << "�ӻ����H" << (i + 1) << "�Ƶ�����" << std::endl;
            nextCarToOutput++;
        }
    }

    //��������Ƿ����
    if (nextCarToOutput > outputTrack.getSize()) {
        std::cout << "������ɣ�" << std::endl;
    }
    else {
        std::cout << "����ʧ��" << std::endl;
    }
}

