#pragma once

#include <cassert>
#include <iostream>

// if you comment out the next line, the tests will run on std::vector instead
// Make sure all tests pass on std::vector before trying to test your implementation
#define USER_VECTOR

/// Place your implementation below
/// The tests are testing not only interface but implementation details. For this to work allocation and deallocation happen in a specific way:
/// You must replace all new/delete with the following
/// T *ptr = new T[count]; -> NEW(T, count);
/// delete[] ptr; -> DELETE(ptr);

/// Example:


template <typename T>
struct Vector {
	// Implement me

private:
	T* data;
	int count;
	int cap;

	void ensureSize(size_t size) {
		
		size_t newSize = data ? this->cap : 1;

		while (newSize < size) {
			newSize *= 2;
		}

		if (newSize != this->cap) {
			reserveSpace(newSize);
		}
	}

	void reserveSpace(size_t size) {

		T* newData = new T[size];
		//T* newData = NEW(T, size);

		for (int i = 0; i < this->count && i < size; ++i) {

			newData[i] = this->data[i];
		}

		delete[] data;
		//DELETE(data);
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

	Vector() : data(nullptr), count(0), cap(0) {}

	Vector(const int count_) {

		//data = NEW(T, count_);
		data = new T[count];
		cap = count_;
		
		count = count_;
	}

	Vector(const int count_, const T& value) : Vector(count_) {
		
		for (int i = 0; i < count_; ++i) {
			this->data[i] = value;
		}
	}

	Vector(const Vector<T> &obj) {

		if (obj.data != nullptr) {
			//data = NEW(T, obj.count);
			data = new T[obj.count];
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

	Vector& operator=(const Vector<T> &obj) {
		
		if (this != &obj) {

			if (this->cap < obj.count) {
				
				T* newData = new T[obj.count];
				//T* newData = NEW(T, count);

				for (int i = 0; i < obj.count; ++i) {
					newData[i] = obj.data[i];
				}

				delete[] this->data;
				//DELETE(this->data);
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

	~Vector() {
		//DELETE(data);
		delete[] data;
	};

	int size() const {
		return this->count;
	}

	int capacity() const {
		return this->cap;
	}

	void reserve(int c) {

		if (c > cap) {
			//T* newData = NEW(T, c);
			T* newData = new T[c];

			for (int i = 0; i < this->count; ++i) {
				newData[i] = this->data[i];
			}

			delete[] this->data;
			//DELETE(this->data);
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

			//T *newData = NEW(T, this->count);
			T* newData = new T[this->count];

			for (int i = 0; i < this->count; ++i) {
				newData[i] = this->data[i];
			}

			//DELETE(this->data);
			delete[] data;
			
			this->data = newData;
			this->cap = this->count;
		}
	}

	void push_back(const T& obj) {

		/* if (this->count == this->cap) {

			if (this->cap == 0) this->cap = 1;

			T* newData = new T[this->cap*2];// NEW(T, this->cap * 2);
			
			for (int i = 0; i < this->count; ++i) {
				newData[i] = this->data[i];
			}

			if (this->data) {
				DELETE(this->data);
			}
			
			this->data = newData;
			this->cap *= 2;
		} */

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
		
		friend class Vector;
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

		for (it; it + 1 != this->end(); ++it) {
			*it = *(it + 1);
		}
		count--;

		return it;
		
	}
	
	iterator erase(iterator first, iterator last) {
		int dist = last.getPointer() - first.getPointer();

		for (first; first + dist != this->end(); ++first) {
			*first = *(first + dist);
		}
		count -= dist;

		return last;
	}
};

