#pragma once
template<class T>
class myVector {
private:
	T* first;//ͷָ��
	int capacity;//��������
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

	//����ԭ�������鲢�������·���,ָ��Ĭ��ֵ��ʼ��
	void resize(int newCap,const T& val) {
		delete[] first;
		capacity = newCap;
		first = new T[newCap];
		for (int i = 0; i < capacity;i++) {
			*(first + i) = val;//�����������Ұָ���ˣ�ע�ⲻҪҰָ���ˣ�
		}
	}

	//����ԭ�������鲢�������·���
	void resize(int newCap) {
		delete[] first;
		capacity = newCap;
		first = new T[newCap];
	}

	//����ֵ���������Ϊ���
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

	//���������Ų�����
	T& operator[](int index) { return *(first + index); }

	~myVector() { delete[] first; }
};