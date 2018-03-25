#ifndef QUEUE_H
#define QUEUE_H

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

template <class T>
class Queue {
public:
    Queue();
    Queue(std::size_t);

    Queue(const Queue &) = delete;
    Queue(Queue &&) = default;
    ~Queue() = default;

    void enqueue(T);

    T dequeue();
private:
    bool full() const {
        return head == increment(tail);
    }

    bool empty() const {
        return head == tail;
    }

    std::size_t increment(std::size_t i) const {
        return (i + 1)%size;
    }

    std::mutex mutex;
    std::condition_variable notfull;
    std::condition_variable notempty;

    const std::size_t size;
    std::size_t head = 0;
    std::size_t tail = 0;
    std::vector<T> vec;
};

template <class T>
Queue<T>::Queue() :
    size(100),
    vec(size)
{
}

template <class T>
Queue<T>::Queue(std::size_t size) :
    size(size),
    vec(size)
{
}

template <class T>
void Queue<T>::enqueue(T elem)
{
    std::unique_lock<std::mutex> lock(mutex);

    while (full()) {
        notfull.wait(lock);
    }

    vec[tail] = std::move(elem);
    tail = increment(tail);
    notempty.notify_one();
}

template <class T>
T Queue<T>::dequeue()
{
    std::unique_lock<std::mutex> lock(mutex);

    while (empty()) {
        notempty.wait(lock);
    }

    T elem = std::move(vec[head]);
    head = increment(head);
    notfull.notify_one();
    return elem;
}

#endif
