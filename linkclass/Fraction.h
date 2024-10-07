//Fraction.h
#pragma once
#include<iostream>

//������
template<class T>
class Fraction {
public:
    T numerator;   //����
    T denominator; //��ĸ
    Fraction(T num=0, T den=1) : numerator(num), denominator(den) {
        //ȷ��������������
        simplify();
    }
    //��������������Լ��
    void simplify() {
        T g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
    }
    //���Լ��
    T gcd(T a, T b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

};

//������������
template <class T>
std::ostream& operator<<(std::ostream& os, const Fraction<T>& frac) {
    os << frac.numerator << "/" << frac.denominator;
    return os;
}