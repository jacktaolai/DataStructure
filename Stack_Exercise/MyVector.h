#pragma once
template<class T>
class myVector {
private:
	T* first;//头指针
	int capacity;//数组容量
public:
	myVector() : first(nullptr), capacity(0){}
	
	myVector(int cap) {
		capacity = cap;
		first = new T[cap];
	}

	myVector(int cap,T val) {
		capacity = cap;
		first = new T[cap];
		for (int i = 0; i < capacity;i++) {
			*(first + i) = val;
		}
	}

	//销毁原来的数组并且再重新分配,指定默认值初始化
	void resize(int newCap,const T& val) {
		delete[] first;
		capacity = newCap;
		first = new T[newCap];
		for (int i = 0; i < capacity;i++) {
			*(first + i) = val;//不用深拷贝就是野指针了！注意不要野指针了！
		}
	}

	//销毁原来的数组并且再重新分配
	void resize(int newCap) {
		delete[] first;
		capacity = newCap;
		first = new T[newCap];
	}

	//将赋值运算符重载为深拷贝
	myVector& operator=(const myVector& other) {
		if (this != &other) {
			delete[] first;
			capacity = other.capacity;
			first = new T[capacity];
			for (int i = 0; i < capacity; i++) {
				first[i] = other.first[i];
			}
		}
		return *this;
	}

	//重载中括号操作符
	T& operator[](int index) { return *(first + index); }

	~myVector() { delete[] first; }
};