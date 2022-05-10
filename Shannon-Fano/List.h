#pragma once
#include <iostream>

template<typename T>
class List
{
public:
	List();
	~List();

private:
	template<typename T>
	struct Node {
		Node* next;
		T data;

		Node(T data = T(), Node* next = nullptr) {
			this->data = data;
			this->next = next;
		}
	};

public:
	Node<T>* head;
	int size;

	void push_back(T data); 
	void push_front(T data);
	void pop_back();
	void pop_front();
	int getSize() { 
		return size;
	}
	T& operator[](const int index); 
	void insert(int index, T data); 
	void sortedInsert(T data);
	void remove(int index); 
	void clear();
	void print();
};

