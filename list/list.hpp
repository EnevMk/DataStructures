#pragma once

#include <cassert>
#include "C:\Users\Asus VivoBook X510UF\Desktop\dev\SD\common\mem_inspect.hpp"

// Uncomment to test this list
#define USER_LIST

// use NEW(Type, count) for allocation
// use DELETE(ptr) for deallocation

// Uncomment if you allocate dynamic dummy node, example: this->dummy = NEW(Node, 1)
// #define LIST_EXTRA_ELEMENT

template <typename T>
struct list {
	struct Node {
		Node *next = nullptr;
		Node *prev = nullptr;
		T data;
	};
	Node dummy;

	size_t count;
private:

	void copy(const list<T>& obj);
	void insertBefore(Node* n, const T& val);
	void remove(Node *target);

public:
	list();
	list(size_t);
	list(size_t, const T&);
	list(const list&);
	list& operator=(const list&);
	~list();

	void clear();

	void push_front(const T&);
	void push_back(const T&);

	void pop_back();
	void pop_front();

	size_t size() const;
	T front() const;
	T back() const;
};

template <typename T>
void list<T>::copy(const list<T>& obj) {

	Node const* ptr = obj.dummy.next;

	while (ptr != &obj.dummy) {
	
		insertBefore(&this->dummy,  ptr->data);
		ptr = ptr->next;
	}
}

template <typename T>
void list<T>::insertBefore(Node* n, const T& val) {

	Node *newNode = NEW(Node, 1);
	newNode->data = val;

	newNode->next = n;
	newNode->prev = n->prev;

	newNode->prev->next = newNode;
	newNode->next->prev = newNode;

	this->count++;
}

template <typename T>
void list<T>::remove(Node *target) {
	
	target->prev->next = target->next;
	target->next->prev = target->prev;

	DELETE(target);

	this->count--;
}

template<typename T>
list<T>::list() {
	dummy.next = &dummy;
	dummy.prev = &dummy;

	count = 0;
}

template<typename T>
list<T>::~list() {
	/* Node* ptr = dummy.next;

	while(ptr != &dummy) {

		Node* temp = ptr;
		ptr = ptr->next;

		DELETE(temp);
	} */

	this->clear();
}

template<typename T>
list<T>::list(const list<T>& obj) : count(0) {
	copy(obj);
}

template<typename T>
list<T>& list<T>::operator=(const list<T>& obj) {

	if (this != &obj) {
		
		clear();
		copy(obj);
	}
	return *this;
}

template <typename T>
void list<T>::clear() {

	Node const* ptr = dummy.next;

	while (ptr != &dummy) {
		
		Node const* temp = ptr;
		ptr = ptr->next;

		DELETE(temp);
	}
}

template<typename T>
void list<T>::push_back(const T& val) {
	insertBefore(&this->dummy, val);
}

template<typename T>
void list<T>::push_front(const T& val) {
	insertBefore(this->dummy.next, val);
}

template<typename T>
void list<T>::pop_back() {
	remove(dummy.prev);
}

template<typename T>
void list<T>::pop_front() {
	remove(dummy.next);
}

template<typename T>
size_t list<T>::size() const {
	return this->count;
}

template<typename T>
T list<T>::front() const {
	return dummy.next->data;
}

template<typename T>
T list<T>::back() const {
	return dummy.prev->data;
}