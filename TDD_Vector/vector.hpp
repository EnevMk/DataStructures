#pragma once

#include "mem_inspect.hpp"
#include <cassert>

// if you comment out the next line, the tests will run on std::vector instead
// Make sure all tests pass on std::vector before trying to test your implementation
#define USER_VECTOR

/// Place your implementation below
/// The tests are testing not only interface but implementation details. For this to work allocation and deallocation happen in a specific way:
/// You must replace all new/delete with the following
/// T *ptr = new T[count]; -> NEW(T, count);
/// delete[] ptr; -> DELETE(ptr);

/// Example:

inline void test_allocation() {
	int *arr = NEW(int, 12);
	for (int c = 0; c < 12; c++) {
		arr[c] = c;
	}
	delete[] arr;
}

template <typename T>
struct vector {
	// Implement me

private:
	T* data;
	int count;
	int cap;

	void ensureSize(size_t size) {
		//std::cout << "\nbefore";
		assert(!this->data || this->cap != 0);
		//std::cout << "\nSled";
		size_t newSize = data ? this->cap : 1;

		while (newSize < size) {
			newSize *= 2;
		}

		if (newSize != this->cap) {
			reserveSpace(newSize);
		}
	}

	void reserveSpace(size_t size) {

		//T* newData = new T[size];
		T* newData = NEW(T, size);

		for (int i = 0; i < this->count && i < size; ++i) {

			newData[i] = this->data[i];
		}

		//delete[] data;
		DELETE(data);
		data = newData;
		this->cap = size;

		this->count = this->count < this->cap ? count : cap;
	}

public:
	void resize(size_t size, const T& val = T()) {

		//ensureSize(size);
		if (size < this->count) {
			this->count -= count - size;
			return;
		}

		if (size > this->cap) {
			reserve(size);
		}

		for (int i = this->count; i < size; ++i) {
			data[i] = val;
		}
		this->count = size;
	}

	vector() : data(nullptr), count(0), cap(0) {}

	vector(const int count_) {

		data = NEW(T, count_);
		cap = count_;
		

		/* for (int i = 0; i < count; ++) {
			data[i] = T();
		} */
		count = count_;
	}

	vector(const int count_, const T& value) : vector(count_) {
		
		for (int i = 0; i < count_; ++i) {
			this->data[i] = value;
		}
	}

	vector(const vector<T> &obj) {
		std::cout << "copy vecrtorr\n";
		if (obj.data != nullptr) {
			data = NEW(T, obj.count);
			cap = obj.count;
			count = obj.count;

			for (int i = 0; i < count; ++i) {

				data[i] = obj.data[i];
			}
		}
		else {
			data = nullptr;
			count = cap = 0;
		}
		
	}

	vector(vector<T>&& obj) : data(obj.data), cap(obj.cap), count(obj.count) {
		obj.data = nullptr;
		std::cout << "mouve vector\n";
	}

	vector& operator=(const vector<T> &obj) {
		std::cout << "copy oper= vecrtorr\n";
		if (this != &obj) {

			if (this->cap < obj.count) {
				
				//T* newData = new T[obj.count];
				T* newData = NEW(T, count);

				for (int i = 0; i < obj.count; ++i) {
					newData[i] = obj.data[i];
				}

				//delete[] this->data;
				DELETE(this->data);
				this->data = newData;

				this->count = obj.count;
				this->cap = obj.count;
			}

			else {

				for (int i = 0; i < obj.count; ++i) {
					data[i] = obj.data[i];
				}

				count = obj.count;
			}
		}
		return *this;
	}

	vector& operator=(vector<T> &&obj) {
		std::cout << "move oper= vectorr\n";
		if (this != &obj) {
			this->data = obj.data;
			this->count = obj.count;
			this->cap = 0;

			obj.count = 0;
			obj.data = nullptr;
		}

		return *this;
	}

	~vector() {
		DELETE(data);
		//delete[] data;
	};

	int size() const {
		return this->count;
	}

	int capacity() const {
		return this->cap;
	}

	void reserve(int c) {

		if (c > cap) {
			T* newData = NEW(T, c);

			for (int i = 0; i < this->count; ++i) {
				newData[i] = this->data[i];
			}

			//delete[] this->data;
			DELETE(this->data);
			this->data = newData;

			cap = c;
		}
	}

	void clear() {
		this->count = 0;
	}

	bool empty() const {
		return (this->count == 0);
	}

	void shrink_to_fit() {
		if (this->count <  this->cap) {

			T *newData = NEW(T, this->count);

			for (int i = 0; i < this->count; ++i) {
				newData[i] = this->data[i];
			}

			DELETE(this->data);
			
			this->data = newData;
			this->cap = this->count;
		}
	}

	void push_back(const T& obj) {

		if (this->count == this->cap) {

			if (this->cap == 0) this->cap = 1;

			T* newData = NEW(T, this->cap * 2);

			for (int i = 0; i < this->count; ++i) {
				newData[i] = this->data[i];
			}

			if (this->data) {
				DELETE(this->data);
			}
			
			this->data = newData;
			this->cap *= 2;
		}

		ensureSize(this->count + 1);

		this->data[count] = obj;
		count++;
	}

	void pop_back() {
		this->count--;
	}

	T back() {
		return this->data[count - 1];
	}

	T front() {
		return this->data[0];
	}

	T& operator[](const int index) {
		

		return this->data[index];
	}

	const T& operator[](const int index) const {
		

		return this->data[index];
	}

	const T& at(const int index) {
		if (index < 0 || index >= count) {
			throw "Invalid index passed";
		}

		return this->data[index];
	}
	
	struct iterator {

	private:
		T* ptr;

		const T* getPointer() const {
			return ptr;
		}
		
		friend class vector;
	public:
		
		iterator(T* init) : ptr(init) {};

		iterator& operator++() {
			this->ptr++;
			return *this;
		};

		iterator& operator--() {
			this->ptr--;
			return *this;
		}

		iterator operator+(const int i) {

			iterator it(this->ptr + i);

			//if (it < end()) {
			return it;
			//}

			//throw "Iterator beyond allocated memory";
		}

		iterator operator-(const int i) {
			iterator it(this->ptr - i);

			//if (it > begin() || it == begin()) {
			return it;
			//}

			//throw "Iterator beyond allocated memory";
		}
		
		bool operator!=(const iterator& obj) const {
			return (this->ptr != obj.ptr);
		};

		bool operator<(const iterator& obj) const {
			return this->ptr < obj.ptr;
		}

		bool operator==(const iterator& obj) const {
			return (this->ptr == obj.ptr);
		}

		T& operator*() {
			return *ptr;
		};
	};

	iterator begin() const {
		return iterator(data);
	}

	iterator end() const {
		return iterator(data + count);
	}

	iterator insert(iterator pos, const T& val) {

		int distFromBegin = pos.getPointer() - data;
		//std::cout << "dist: " << distFromBegin << '\n';
		
		ensureSize(count + 1);

		iterator newIter = this->begin() + distFromBegin;

		for (iterator iter = end(); iter != newIter; --iter) {
			*iter = *(iter - 1);
			
		}
		*newIter = val;

		this->count++;

		return newIter;

	}

	iterator erase(iterator it) {

		try {

			for (it; it + 1 != this->end(); ++it) {
				*it = *(it + 1);
			}
			count--;
		}
		catch(const char* s) {
			std::cout << "Operation failed. Error message:\n" << s << '\n';
			
		}

		return it;
		
	}
	///TODO
	iterator erase(iterator first, iterator last) {
		int dist = last.getPointer() - first.getPointer();

		for (first; first + dist != this->end(); ++first) {
			*first = *(first + dist);
		}
		count -= dist;

		return last;
	}
};

