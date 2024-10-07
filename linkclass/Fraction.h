//Fraction.h
#pragma once
#include<iostream>

//分数类
template<class T>
class Fraction {
public:
    T numerator;   //分子
    T denominator; //分母
    Fraction(T num=0, T den=1) : numerator(num), denominator(den) {
        //确保分数是最简分数
        simplify();
    }
    //辅助函数，用来约分
    void simplify() {
        T g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
    }
    //最大公约数
    T gcd(T a, T b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

};

//重载输出运算符
template <class T>
std::ostream& operator<<(std::ostream& os, const Fraction<T>& frac) {
    os << frac.numerator << "/" << frac.denominator;
    return os;
}