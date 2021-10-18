
#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>

template <typename T>
class Queue {

private:

    struct Box {
        Box *prev;
        Box *next;
        T data;
    };

    Box *endOfQueue;
    Box *frontOfQueue;

    int size_;

public:
    
    Queue() {
        endOfQueue = nullptr;
        frontOfQueue = nullptr;
        size_ = 0;
    }

    //Queue(T arr[])

    ~Queue() {
        
        Box *crr = endOfQueue;

        while (crr != nullptr) {
            //crr = crr->next;
            Box *nxt = crr->next;
            delete crr;
            crr = nxt;
        }
    }

    void push(const T& data_) {

        /* Box *last = new Box{nullptr, endOfQueue, data_};
        endOfQueue = last; */
        endOfQueue = new Box{nullptr, endOfQueue, data_};
        
        //endOfQueue->next->prev = endOfQueue;

        if (size_ == 0) frontOfQueue = endOfQueue;
        else {
            Box *temp = endOfQueue->next;
            temp->prev = endOfQueue;
        }
        this->size_++;
    }

    void pop() {
        Box *first = frontOfQueue;
        frontOfQueue = frontOfQueue->prev;
        delete first;
        frontOfQueue->next = nullptr;
        
        this->size_--;
    }

    int size() const {
        return size_;
    }

    T front() const {
        return frontOfQueue->data;
    }

    T back() const {
        return endOfQueue->data;
    }
};

#endif