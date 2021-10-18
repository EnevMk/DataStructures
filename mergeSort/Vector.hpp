#pragma once

template <typename T>
class Vector {

private:
    T *arr;
    int current;
    int capacity;

public:

    using value_type = T;

    Vector();
    //Vector(const T *arr_[], int count);
    Vector(const T *arr_, int count);
    Vector<T>& operator=(const Vector<T> &obj);
    void push(T data);
    //void push(T data, int index);
    void popBack();
    void removeAtIndex(const int index);
    int size() const;
    int maxCapacity() const;
    T& operator[](int index) const;
    void reserve(unsigned n);
    bool elem(const T &data) const;
    ~Vector();

    class Iterator {
    
    private:

        T* ptr;

    public:

        using value_type = T;

        const T* getPtr() const;
        Iterator(T* init);
        bool operator!=(const Iterator &other);
        Iterator& operator++();
        Iterator operator+(int n);
        T operator*();
        std::ptrdiff_t operator-(const Iterator &other);
        //bool Vector<T>::Iterator::operator<(const Iterator& other);
    };

    Iterator begin();
    Iterator end();
    
};
//#include "Vector.cpp"
template <typename T>
const T* Vector<T>::Iterator::getPtr() const {
    return this->ptr;
}

template <typename T>
Vector<T>::Iterator::Iterator(T* init) {
    ptr = init;
}

template <typename T>
bool Vector<T>::Iterator::operator!=(const Iterator &other) {
    return (this->ptr != other.ptr);
}

template <typename T>
typename Vector<T>::Iterator::Iterator& Vector<T>::Iterator::operator++() {
    this->ptr++;
    return *this;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator+(int n) {
    T *crr = ptr;
    for (int i = 0; i < n; ++i) {
        crr++;
    }

    return Iterator(crr);
}

template <typename T>
T Vector<T>::Iterator::operator*() {
    return *ptr;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin() {
    return Iterator(arr);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end() {
    return Iterator(arr + this->current);
}

template <typename T>
std::ptrdiff_t Vector<T>::Iterator::operator-(const Iterator& other) {

    return this->ptr - other.ptr;
}

/* template <typename T>
bool Vector<T>::Iterator::operator<(const Iterator& other) {
    return this->ptr < other;
} */

template <typename T>
Vector<T>::Vector() {
    arr = new T[8];
    current = 0;
    capacity = 8;
}


template <typename T>
Vector<T>::Vector(const T *arr_, int count) {
    arr = new T[count];

    capacity = count;
    current = count;

    for (int i = 0; i < current; ++i) {
        arr[i] = arr_[i];
    }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &obj) {
    if(this != &obj) {
        capacity = obj.capacity;
        current = obj.current;

        delete[] arr;
        arr = new T[capacity];

        for (int i = 0; i < current; ++i) {
            arr[i] = obj.arr[i];
        }
    }
    return *this;
}

template <typename T>
Vector<T>::~Vector() {

    delete[] arr;
    current = 0;
    capacity = 0;
}

template <typename T>
bool Vector<T>::elem(const T &data) const {
    for (int i = 0; i < current; ++i) {

        if (data == arr[i]) return true;
    }
    return false;
}

template <typename T>
void Vector<T>::push(T data) {
    if (current == capacity) {
        T *temp = new T[2 * capacity];

        for (int i = 0; i < current; ++i) {
            temp[i] = arr[i];
        }

        delete[] arr;
        capacity *= 2;
        arr = temp;
    }
    arr[current] = data;
    current++;

}

template <typename T>
void Vector<T>::popBack() {
    current--;
}

template <typename T>
void Vector<T>::removeAtIndex(const int index) {
    if (index < size()) {
        for (int i = index; i < size() - 1; ++i) {
            arr[i] = arr[i + 1];
        }
        current--;
    }
}

template <typename T>
int Vector<T>::size() const{
    return current;
}

template <typename T>
int Vector<T>::maxCapacity() const {
    return capacity;
}

template <typename T>
T& Vector<T>::operator[](int index) const {
    return *(arr + index);
}

template <typename T>
void Vector<T>::reserve(unsigned n) {
    if (n > capacity) {
        T *temp = new T[n];

        for (int i = 0; i < current; ++i) {
            temp[i] = arr[i];
        }
        delete[] arr;
        arr = temp;
        capacity = n;
    }
}