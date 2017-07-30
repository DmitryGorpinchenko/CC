#if !defined (_STLQUEUE_H)
#define _STLQUEUE_H

#include <queue>
#include <stdlib.h>

template <typename T, typename QUEUE = std::queue<T>> class STLQueue_Adapter {
public:
    class Underflow {};
    class Overflow {};

    STLQueue_Adapter(size_t) {}

    void enqueue(const T &new_item);
    void dequeue();
    T front() const;
    bool is_empty() const;
    bool is_full() const;
    size_t size() const;

private:
    QUEUE queue_;
};

#include "STLQueue.cpp"

#endif
