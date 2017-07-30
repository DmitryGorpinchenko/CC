#if !defined (_AQUEUE_C)
#define _AQUEUE_C

#include "AQueue.h"

#include <algorithm>

// AQueue

template <typename T, typename ARRAY> AQueue<T, ARRAY>::AQueue(size_t size)
    : queue_(size + 1)
    , head_(0)
    , tail_(0)
    , count_(0)
{}

template <typename T, typename ARRAY> size_t AQueue<T, ARRAY>::size() const { return count_; }
template <typename T, typename ARRAY> void AQueue<T, ARRAY>::swap(AQueue<T, ARRAY> &new_queue)
{
    queue_.swap(new_queue.queue_);
    std::swap(head_, new_queue.head_);
    std::swap(tail_, new_queue.tail_); 
    std::swap(count_, new_queue.count_);
}
template <typename T, typename ARRAY> bool AQueue<T, ARRAY>::operator==(const AQueue<T, ARRAY> &rhs) const
{
    return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
}
template <typename T, typename ARRAY> bool AQueue<T, ARRAY>::operator!=(const AQueue<T, ARRAY> &rhs) const
{
    return !(*this == rhs);
}
template <typename T, typename ARRAY> void AQueue<T, ARRAY>::enqueue(const T &new_item)
{
    if (is_full()) { throw Overflow(); } queue_[tail_] = new_item; tail_ = increment(tail_); ++count_;
}
template <typename T, typename ARRAY> void AQueue<T, ARRAY>::dequeue()
{
    if (is_empty()) { throw Underflow(); } head_ = increment(head_); --count_;
}
template <typename T, typename ARRAY> T AQueue<T, ARRAY>::front() const { if (is_empty()) { throw Underflow(); } return queue_[head_]; }
template <typename T, typename ARRAY> bool AQueue<T, ARRAY>::is_empty() const { return count_ == 0; }
template <typename T, typename ARRAY> bool AQueue<T, ARRAY>::is_full() const { return count_ == queue_.size() - 1; }

template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::iterator AQueue<T, ARRAY>::begin()
{
    return iterator(*this, head_);
}
template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::iterator AQueue<T, ARRAY>::end()
{
    return iterator(*this, tail_);
}
template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::const_iterator AQueue<T, ARRAY>::begin() const
{
    return const_iterator(*this, head_);
}
template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::const_iterator AQueue<T, ARRAY>::end() const
{
    return const_iterator(*this, tail_);
}
template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::reverse_iterator AQueue<T, ARRAY>::rbegin()
{
    return reverse_iterator(*this, tail_ - 1); 
}
template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::reverse_iterator AQueue<T, ARRAY>::rend()
{
    return reverse_iterator(*this, tail_);
}
template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::const_reverse_iterator AQueue<T, ARRAY>::rbegin() const
{
    return const_reverse_iterator(*this, tail_ - 1); 
}
template <typename T, typename ARRAY> typename AQueue<T, ARRAY>::const_reverse_iterator AQueue<T, ARRAY>::rend() const
{
    return const_reverse_iterator(*this, tail_); 
}
template <typename T, typename ARRAY> size_t AQueue<T, ARRAY>::increment(size_t index) const
{
    return index == queue_.size() - 1 ? 0 : index + 1; 
}
template <typename T, typename ARRAY> size_t AQueue<T, ARRAY>::decrement(size_t index) const
{
    return index == 0 ? queue_.size() - 1 : index - 1;
}

// AQueue_Iterator

template <typename T, typename ARRAY> T &AQueue_Iterator<T, ARRAY>::operator*() const
{
    return aqueue_ref_.queue_[pos_];
}
template <typename T, typename ARRAY> AQueue_Iterator<T, ARRAY> &AQueue_Iterator<T, ARRAY>::operator++()
{
    pos_ = aqueue_ref_.increment(pos_); return *this;
}
template <typename T, typename ARRAY> AQueue_Iterator<T, ARRAY> AQueue_Iterator<T, ARRAY>::operator++(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.increment(pos_); return tmp;
}
template <typename T, typename ARRAY> AQueue_Iterator<T, ARRAY> &AQueue_Iterator<T, ARRAY>::operator--()
{
    pos_ = aqueue_ref_.decrement(pos_); return *this;
}
template <typename T, typename ARRAY> AQueue_Iterator<T, ARRAY> AQueue_Iterator<T, ARRAY>::operator--(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.decrement(pos_); return tmp;
}
template <typename T, typename ARRAY> bool AQueue_Iterator<T, ARRAY>::operator==(const AQueue_Iterator<T, ARRAY> &rhs) const
{
    return &aqueue_ref_ == &rhs.aqueue_ref_ && pos_ == rhs.pos_;
}
template <typename T, typename ARRAY> bool AQueue_Iterator<T, ARRAY>::operator!=(const AQueue_Iterator<T, ARRAY> &rhs) const
{ 
    return !(*this == rhs);
}
template <typename T, typename ARRAY> AQueue_Iterator<T, ARRAY>::AQueue_Iterator(AQueue<T, ARRAY> &queue, size_t pos) 
    : aqueue_ref_(queue), pos_(pos)
{}

// Const_AQueue_Iterator

template <typename T, typename ARRAY> const T &Const_AQueue_Iterator<T, ARRAY>::operator*() const
{
    return aqueue_ref_.queue_[pos_];
}
template <typename T, typename ARRAY> Const_AQueue_Iterator<T, ARRAY> &Const_AQueue_Iterator<T, ARRAY>::operator++()
{
    pos_ = aqueue_ref_.increment(pos_); return *this;
}
template <typename T, typename ARRAY> Const_AQueue_Iterator<T, ARRAY> Const_AQueue_Iterator<T, ARRAY>::operator++(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.increment(pos_); return tmp;
}
template <typename T, typename ARRAY> Const_AQueue_Iterator<T, ARRAY> &Const_AQueue_Iterator<T, ARRAY>::operator--()
{
    pos_ = aqueue_ref_.decrement(pos_); return *this;
}
template <typename T, typename ARRAY> Const_AQueue_Iterator<T, ARRAY> Const_AQueue_Iterator<T, ARRAY>::operator--(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.decrement(pos_); return tmp;
}
template <typename T, typename ARRAY> bool Const_AQueue_Iterator<T, ARRAY>::operator==(const Const_AQueue_Iterator<T, ARRAY> &rhs) const
{
    return &aqueue_ref_ == &rhs.aqueue_ref_ && pos_ == rhs.pos_;
}
template <typename T, typename ARRAY> bool Const_AQueue_Iterator<T, ARRAY>::operator!= (const Const_AQueue_Iterator<T, ARRAY> &rhs) const
{
    return !(*this == rhs);
}
template <typename T, typename ARRAY> Const_AQueue_Iterator<T, ARRAY>::Const_AQueue_Iterator(const AQueue<T, ARRAY> &queue, size_t pos)
    : aqueue_ref_(queue), pos_(pos)
{}

// AQueue_Reverse_Iterator

template <typename T, typename ARRAY> T &AQueue_Reverse_Iterator<T, ARRAY>::operator*() const
{
    return aqueue_ref_.queue_[pos_];
}
template <typename T, typename ARRAY> AQueue_Reverse_Iterator<T, ARRAY> &AQueue_Reverse_Iterator<T, ARRAY>::operator++()
{
    pos_ = aqueue_ref_.decrement(pos_); return *this;
}
template <typename T, typename ARRAY> AQueue_Reverse_Iterator<T, ARRAY> AQueue_Reverse_Iterator<T, ARRAY>::operator++(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.decrement(pos_); return tmp;
}
template <typename T, typename ARRAY> AQueue_Reverse_Iterator<T, ARRAY> &AQueue_Reverse_Iterator<T, ARRAY>::operator--()
{
    pos_ = aqueue_ref_.increment(pos_); return *this;
}
template <typename T, typename ARRAY> AQueue_Reverse_Iterator<T, ARRAY> AQueue_Reverse_Iterator<T, ARRAY>::operator--(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.increment(pos_); return tmp;
}
template <typename T, typename ARRAY> bool AQueue_Reverse_Iterator<T, ARRAY>::operator==(const AQueue_Reverse_Iterator<T, ARRAY> &rhs) const
{
    return &aqueue_ref_ == &rhs.aqueue_ref_ && pos_ == rhs.pos_;
}
template <typename T, typename ARRAY> bool AQueue_Reverse_Iterator<T, ARRAY>::operator!=(const AQueue_Reverse_Iterator<T, ARRAY> &rhs) const
{
    return !(*this == rhs);
}
template <typename T, typename ARRAY> AQueue_Reverse_Iterator<T, ARRAY>::AQueue_Reverse_Iterator(AQueue<T, ARRAY> &queue, size_t pos)
    : aqueue_ref_(queue), pos_(pos)
{}

// Const_AQueue_Reverse_Iterator 

template <typename T, typename ARRAY> const T &Const_AQueue_Reverse_Iterator<T, ARRAY>::operator*() const
{
    return aqueue_ref_.queue_[pos_];
} 
template <typename T, typename ARRAY> Const_AQueue_Reverse_Iterator<T, ARRAY> &Const_AQueue_Reverse_Iterator<T, ARRAY>::operator++()
{
    pos_ = aqueue_ref_.decrement(pos_); return *this;
}
template <typename T, typename ARRAY> Const_AQueue_Reverse_Iterator<T, ARRAY> Const_AQueue_Reverse_Iterator<T, ARRAY>::operator++(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.decrement(pos_); return tmp;
}
template <typename T, typename ARRAY> Const_AQueue_Reverse_Iterator<T, ARRAY> &Const_AQueue_Reverse_Iterator<T, ARRAY>::operator--()
{
    pos_ = aqueue_ref_.increment(pos_); return *this;
}
template <typename T, typename ARRAY> Const_AQueue_Reverse_Iterator<T, ARRAY> Const_AQueue_Reverse_Iterator<T, ARRAY>::operator--(int)
{
    auto tmp = *this; pos_ = aqueue_ref_.increment(pos_); return tmp;
}
template <typename T, typename ARRAY> bool Const_AQueue_Reverse_Iterator<T, ARRAY>::operator==(const Const_AQueue_Reverse_Iterator<T, ARRAY> &rhs) const
{
    return &aqueue_ref_ == &rhs.aqueue_ref_ && pos_ == rhs.pos_;
}
template <typename T, typename ARRAY> bool Const_AQueue_Reverse_Iterator<T, ARRAY>::operator!=(const Const_AQueue_Reverse_Iterator<T, ARRAY> &rhs) const
{
    return !(*this == rhs);
}
template <typename T, typename ARRAY> Const_AQueue_Reverse_Iterator<T, ARRAY>::Const_AQueue_Reverse_Iterator(const AQueue<T, ARRAY> &queue, size_t pos)
    : aqueue_ref_(queue), pos_(pos)
{}

#endif
