#if !defined (_Queue_CPP)
#define _Queue_CPP

#include "Queue.h"
#include <iostream>

template <typename T> Queue<T>::~Queue() = default;

template <typename T, typename QUEUE> Queue_Adapter<T, QUEUE>::Queue_Adapter(size_t size) : queue_(size) {}
template <typename T, typename QUEUE> bool Queue_Adapter<T, QUEUE>::operator==(const Queue_Adapter &rhs) const { return queue_ == rhs.queue_; }
template <typename T, typename QUEUE> bool Queue_Adapter<T, QUEUE>::operator!=(const Queue_Adapter &rhs) const { return !(*this == rhs); }

template <typename T, typename QUEUE> void Queue_Adapter<T, QUEUE>::enqueue(const T &new_item)
{
    if (is_full()) { throw typename Queue<T>::Overflow(); } queue_.enqueue(new_item);
}
template <typename T, typename QUEUE> void Queue_Adapter<T, QUEUE>::dequeue()
{
    if (is_empty()) { throw typename Queue<T>::Underflow(); } queue_.dequeue();
}
template <typename T, typename QUEUE> T Queue_Adapter<T, QUEUE>::front() const
{
    if (is_empty()) { throw typename Queue<T>::Underflow(); } return queue_.front();
}

template <typename T, typename QUEUE> bool Queue_Adapter<T, QUEUE>::is_empty() const { return queue_.is_empty(); }
template <typename T, typename QUEUE> bool Queue_Adapter<T, QUEUE>::is_full() const { return queue_.is_full(); }
template <typename T, typename QUEUE> size_t Queue_Adapter<T, QUEUE>::size() const { return queue_.size(); }

#endif
