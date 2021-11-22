#pragma once

#include <cassert>
#include "C:\Users\Asus VivoBook X510UF\Desktop\dev\SD\common\mem_inspect.hpp"

// Uncomment to test this list
#define USER_LIST

// use NEW(Type, count) for allocation
// use DELETE(ptr) for deallocation

// Uncomment if you allocate dynamic dummy node, example: this->dummy = NEW(Node, 1)
// #define LIST_EXTRA_ELEMENT

/// Value result template, true when argument is const qualified
template <typename T>
struct is_const {
	enum {
		value = false,
	};
};

/// Specialization for const
template <typename T>
struct is_const<const T> {
	enum {
		value = true,
	};
};

template <bool test, typename T>
struct enable_if {
	typedef T type;
};

/// Specialization for false case
template <typename T>
struct enable_if<false, T> {
};


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

	template <bool isConst>
	struct base_iterator {

	private:
		Node *ptr;

		base_iterator(Node *n) : ptr(n) {}

		friend struct list;
	public:

		template <bool val = isConst> ///if iterator is not const, return non const deref. operator
		typename enable_if<!val, T>::type& operator*() {
			return ptr->data;
		}

		template <bool val = isConst>
		typename enable_if<val, const T>::type& operator*() {
			return ptr->data;
		}

		/* T& operator*() {

		}

		const T& operator*() {

		} */
	};

	typedef base_iterator<true> const_iterator;
	typedef base_iterator<false> iterator;

	const_iterator cbegin() {
		return const_iterator(dummy.next);
	}

	iterator begin() {
		return iterator(dummy.next);
	}
	/* template <bool val = isConst>
	typename enable_if<!val, base_iterator>::type& begin() {
		return base_iterator(dummy.next);
	}

	template <bool val = isConst>
	typename enable_if<val, const base_iterator>::type& begin() {
		return base_iterator(dummy.next);
	} */

	list();
	list(const size_t);
	list(const size_t, const T&);
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

	Node* ptr = obj.dummy.next;

	while (ptr != &obj.dummy) {
	
		insertBefore(&this->dummy, ptr->data);
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

	this->clear();
}

template<typename T>
list<T>::list(const list<T>& obj) : list() {
	copy(obj);
}

template<typename T>
list<T>::list(const size_t count_) : list() {
	
	while (count_ != this->count) {
		insertBefore(&this->dummy, T());
	}
}

template<typename T>
list<T>::list(const size_t count_, const T& val) : list() {

	while (count_ != this->count) {
		insertBefore(&this->dummy, val);
	}
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

	while (this->count != 0) {
		this->remove(dummy.next);
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