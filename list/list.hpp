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

	enum IteratorDirection {
		Forward,
		Backward,
	};
	template <bool isConst, IteratorDirection direction = Forward>
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
		typename enable_if<val, const T>::type& operator*() const {
			return ptr->data;
		}

		template <bool val = isConst>
		typename enable_if<!val, T*>::type operator->() {
			return ptr->data;
		}

		template <bool val = isConst>
		typename enable_if<val, const T*>::type operator->() const {
			return &ptr->data;
		}
		//unneccessary 
		template <bool val>
		bool operator!=(base_iterator<val> other) {
			return this->ptr != other.ptr;
		}

		template <bool val, IteratorDirection dir>
		bool operator!=(base_iterator<val, dir> other) {
			return this->ptr != other.ptr;
		}

		template <bool val, IteratorDirection dir>
		bool operator==(base_iterator<val, dir> other) const {
			return this->ptr == other.ptr;
		}

		/* base_iterator& operator++() {
			ptr = ptr->next;
			return *this;
		}

		base_iterator& operator--() {
			ptr = ptr->prev;
			return *this;
		} */

		template<IteratorDirection dir = direction>
		typename enable_if<!dir, base_iterator&>::type operator++() {
			ptr = ptr->next;
			return *this;
		}

		template<IteratorDirection dir = direction>
		typename enable_if<dir, base_iterator&>::type operator++() {
			ptr = ptr->prev;
			return *this;
		}

		template<IteratorDirection dir = direction>
		typename enable_if<!dir, base_iterator&>::type operator--() {
			ptr = ptr->prev;
			return *this;
		}
		
		template<IteratorDirection dir = direction>
		typename enable_if<dir, base_iterator&>::type operator--() {
			ptr = ptr->next;
			return *this;
		}
	};

	typedef base_iterator<true> const_iterator;
	typedef base_iterator<false> iterator;
	typedef base_iterator<false, IteratorDirection::Backward> reversed_iterator;

	const_iterator cbegin() {
		return const_iterator(dummy.next);
	}

	const_iterator cend() {
		return const_iterator(&dummy);
	}
	iterator begin() {
		return iterator(dummy.next);
	}

	iterator end() {
		return iterator(&dummy);
	}

	reversed_iterator rbegin() {
		return reversed_iterator(dummy.prev);
	}

	reversed_iterator rend() {
		return reversed_iterator(&dummy);
	}

	list();
	list(const size_t);
	list(const size_t, const T&);
	list(const list&);
	list& operator=(const list&);
	~list();

	void clear();

	void push_front(const T&);
	void push_back(const T&);

	//base_iterator insert(base_iterator position, const T& val);
	template <bool val, IteratorDirection dir>
	base_iterator<val, dir> insert(base_iterator<val, dir> position, const T& value);

	template <bool val, IteratorDirection dir>
	base_iterator<val, dir> insert(base_iterator<val, dir> position, const int n, const T& value);

	template <bool val, IteratorDirection dir, typename InputIterator>
	base_iterator<val, dir> insert(base_iterator<val, dir> position, InputIterator first, InputIterator last);

	template <bool val, IteratorDirection dir>
	base_iterator<val, dir> erase(base_iterator<val, dir> position);

	void pop_back();
	void pop_front();

	bool empty() const;
	size_t size() const;
	T front() const;
	T back() const;
};

template <typename T>
using IteratorDirection = typename list<T>::IteratorDirection;

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

template <typename T>
list<T>::list() {
	dummy.next = &dummy;
	dummy.prev = &dummy;

	count = 0;
}

template <typename T>
list<T>::~list() {

	this->clear();
}

template <typename T>
list<T>::list(const list<T>& obj) : list() {
	copy(obj);
}

template <typename T>
list<T>::list(const size_t count_) : list() {
	
	while (count_ != this->count) {
		insertBefore(&this->dummy, T());
	}
}

template <typename T>
list<T>::list(const size_t count_, const T& val) : list() {

	while (count_ != this->count) {
		insertBefore(&this->dummy, val);
	}
}

template <typename T>
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

template <typename T>
void list<T>::push_back(const T& val) {
	insertBefore(&this->dummy, val);
}

template <typename T>
void list<T>::push_front(const T& val) {
	insertBefore(this->dummy.next, val);
}

template <typename T>
void list<T>::pop_back() {
	remove(dummy.prev);
}

template <typename T>
void list<T>::pop_front() {
	remove(dummy.next);
}

template <typename T>
template <bool val, IteratorDirection<T> dir>
list<T>::base_iterator<val, dir> list<T>::insert(list<T>::base_iterator<val, dir> position, const T& value) {

	insertBefore(position.ptr, value);
	return position;
}

template <typename T>
template <bool val, IteratorDirection<T> dir>
list<T>::base_iterator<val, dir> list<T>::insert(list<T>::base_iterator<val, dir> position, const int n, const T& value) {

	for (int i = 0; i < n; ++i) {

		insertBefore(position.ptr, value);
	}
	return position;
}

template <typename T>
template <bool val, IteratorDirection<T> dir, typename InputIterator>
list<T>::base_iterator<val, dir> list<T>::insert(list<T>::base_iterator<val, dir> position, InputIterator first, InputIterator last) {

	for (auto it = first; it != last; ++it) {
		insertBefore(position.ptr, *it);
	}

	return position;
}

template <typename T>
template <bool val, IteratorDirection<T> dir>
list<T>::base_iterator<val, dir> list<T>::erase(base_iterator<val, dir> position) {

	++position;
	remove(iterator(position.ptr->prev).ptr);
	
	return position;
}

template <typename T>
bool list<T>::empty() const {
	return this->count == 0;
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

//try to separate definitions from implementations later

/* 
template <typename T>
template <bool isConst>
list<T>::base_iterator<isConst>

template <bool val = isConst>
typename enable_if<!val, T>::type& base_iterator::operator*() {
	return ptr->data;
}

template <bool val = isConst>
typename enable_if<val, const T>::type& operator*() const {
	return ptr->data;
}

template <bool val = isConst>
typename enable_if<!val, T*>::type operator->() {
	return ptr->data;
}

template <bool val = isConst>
typename enable_if<val, const T*>::type operator->() const {
	return &ptr->data;
}

template <bool val>
bool operator!=(base_iterator<val> other) {
	return this->ptr != other.ptr;
}

template <bool val, IteratorDirection dir>
bool operator!=(base_iterator<val, dir> other) {
	return this->ptr != other.ptr;
}

/* base_iterator& operator++() {
	ptr = ptr->next;
	return *this;
}

base_iterator& operator--() {
	ptr = ptr->prev;
	return *this;
} 
template<IteratorDirection dir = direction>
typename enable_if<!dir, base_iterator&>::type operator++() {
	ptr = ptr->next;
	return *this;
}

template<IteratorDirection dir = direction>
typename enable_if<dir, base_iterator&>::type operator++() {
	ptr = ptr->prev;
	return *this;
} */