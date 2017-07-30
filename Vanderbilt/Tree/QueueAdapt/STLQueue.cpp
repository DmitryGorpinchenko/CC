#if !defined (_STLQUEUE_CPP)
#define _STLQUEUE_CPP

#include "STLQueue.h"

template <typename T, typename QUEUE> void STLQueue_Adapter<T, QUEUE>::enqueue(const T &new_item) { queue_.push(new_item); }
template <typename T, typename QUEUE> void STLQueue_Adapter<T, QUEUE>::dequeue() { queue_.pop(); }  
template <typename T, typename QUEUE> T STLQueue_Adapter<T, QUEUE>::front() const { return queue_.front(); }
template <typename T, typename QUEUE> bool STLQueue_Adapter<T, QUEUE>::is_empty() const { return queue_.empty(); }
template <typename T, typename QUEUE> bool STLQueue_Adapter<T, QUEUE>::is_full() const { return false; }
template <typename T, typename QUEUE> size_t STLQueue_Adapter<T, QUEUE>::size() const { return queue_.size(); }

#endif
