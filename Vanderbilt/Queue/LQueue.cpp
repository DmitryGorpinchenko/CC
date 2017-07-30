#if !defined (_LQUEUE_CPP)
#define _LQUEUE_CPP

#include <iostream>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <memory>

#include "LQueue.h"

template <typename T> LQueue_Node<T> *LQueue_Node<T>::free_list_ = nullptr;

template <typename T> void *LQueue_Node<T>::operator new(size_t size)
{
    if (free_list_) {
        auto tmp = free_list_;
        free_list_ = (free_list_->next_ == free_list_
                      ? nullptr
                      : free_list_->next_);
        tmp->unlink();
        return tmp;
    }
    return ::operator new(size);
}

template <typename T> void LQueue_Node<T>::operator delete(void *ptr)
{
    free_list_add(ptr);
}

template <typename T> void LQueue_Node<T>::free_list_add(void *ptr)
{
    if (ptr == nullptr) {
        return; 
    }
    auto tmp = static_cast<LQueue_Node *>(ptr);
    if (free_list_ == nullptr) {
        free_list_ = tmp->next_ = tmp->prev_ = tmp;
    } else {
        tmp->next_ = free_list_;
        tmp->prev_ = free_list_->prev_;
        free_list_->prev_->next_ = tmp;
        free_list_->prev_ = tmp;
        free_list_ = tmp;
    }
}

template <typename T> void LQueue_Node<T>::free_list_allocate(size_t n)
{
    while (n--) {
        free_list_add(::operator new(sizeof(LQueue_Node)));
    }
}

template <typename T> void LQueue_Node<T>::free_list_release()
{
    if (free_list_ == nullptr) {
        return;
    }
    while (free_list_->next_ != free_list_) {
        auto tmp = free_list_->next_;
        free_list_->unlink();
        ::operator delete(free_list_);
        free_list_ = tmp;
    }
    free_list_->unlink();
    ::operator delete(free_list_);
    free_list_ = nullptr;
}

template <typename T>
LQueue_Node<T>::LQueue_Node(LQueue_Node *next, LQueue_Node *prev)
    : next_(next)
    , prev_(prev)
{
    next_->prev_ = this;
    prev_->next_ = this;
}

template <typename T>
LQueue_Node<T>::LQueue_Node()
    : next_(this)
    , prev_(this)
{}

template <typename T>
LQueue_Node<T>::~LQueue_Node()
{
    unlink();
}

template<typename T, typename LQUEUE_NODE>
LQueue<T, LQUEUE_NODE>::LQueue(size_t size_hint)
    : tail_(new LQUEUE_NODE())
    , count_(0)
{
    LQUEUE_NODE::free_list_allocate(size_hint);
}

template<typename T, typename LQUEUE_NODE>
void LQueue<T, LQUEUE_NODE>::swap(LQueue &new_queue)
{
    std::swap(tail_, new_queue.tail_);
    std::swap(count_, new_queue.count_);
}

template <typename T, typename LQUEUE_NODE>
LQueue<T, LQUEUE_NODE>::LQueue(const LQueue &rhs)
    : tail_(nullptr) // tail_ will be set correctly by swap()
    , count_(0) // count_ will be set correctly by swap()
{
    LQueue tmp(rhs.size());
    std::for_each(rhs.begin(), rhs.end(), [&tmp](const auto& item) { tmp.enqueue(item); });
    swap(tmp);
}

template <typename T, typename LQUEUE_NODE>
LQueue<T, LQUEUE_NODE> &LQueue<T, LQUEUE_NODE>::operator=(const LQueue &rhs)
{
    if (this != &rhs) {
        LQueue tmp(rhs); swap(tmp);
    }
    return *this;
}

template <typename T, typename LQUEUE_NODE>
LQueue<T, LQUEUE_NODE>::~LQueue()
{
    while (!is_empty()) {
        dequeue_i();
    }
    delete tail_;
}

template <typename T, typename LQUEUE_NODE>
bool LQueue<T, LQUEUE_NODE>::operator==(const LQueue &rhs) const {
    return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
}

template <typename T, typename LQUEUE_NODE>
bool LQueue<T, LQUEUE_NODE>::operator!=(const LQueue &rhs) const {
    return !(*this == rhs);
}

template <typename T, typename LQUEUE_NODE> void LQueue<T, LQUEUE_NODE>::enqueue(const T &new_item)
{
    try {
        auto tmp = new LQUEUE_NODE(tail_, tail_->prev_);
        try {
            tmp->item_ = new_item;
            ++count_;
        } catch (...) {
            delete tmp;
            throw;
        }
    } catch (...) {
        throw Overflow();
    }
}

template <typename T, typename LQUEUE_NODE> void LQueue<T, LQUEUE_NODE>::dequeue_i()
{
    delete tail_->next_;
    --count_;
}

template <typename T, typename LQUEUE_NODE> void LQueue<T, LQUEUE_NODE>::dequeue()
{
    if (is_empty()) {
        throw Underflow();
    }
    dequeue_i();
}

template <typename T, typename LQUEUE_NODE> T LQueue<T, LQUEUE_NODE>::front() const
{
    if (is_empty()) {
        throw Underflow();
    }
    return tail_->next_->item_;
}

template <typename T, typename LQUEUE_NODE> 
typename LQueue<T, LQUEUE_NODE>::iterator LQueue<T, LQUEUE_NODE>::begin()
{
    return iterator(tail_->next_);
}

template <typename T, typename LQUEUE_NODE>
typename LQueue<T, LQUEUE_NODE>::iterator LQueue<T, LQUEUE_NODE>::end()
{
    return iterator(tail_);
}

template <typename T, typename LQUEUE_NODE>
typename LQueue<T, LQUEUE_NODE>::const_iterator LQueue<T, LQUEUE_NODE>::begin() const
{
    return const_iterator(tail_->next_);
}

template <typename T, typename LQUEUE_NODE>
typename LQueue<T, LQUEUE_NODE>::const_iterator LQueue<T, LQUEUE_NODE>::end() const
{
    return const_iterator(tail_);
}

template <typename T, typename LQUEUE_NODE>
typename LQueue<T, LQUEUE_NODE>::reverse_iterator LQueue<T, LQUEUE_NODE>::rbegin()
{
    return reverse_iterator(tail_->prev_);
}

template <typename T, typename LQUEUE_NODE>
typename LQueue<T, LQUEUE_NODE>::reverse_iterator LQueue<T, LQUEUE_NODE>::rend()
{
    return reverse_iterator(tail_);
}

template <typename T, typename LQUEUE_NODE>
typename LQueue<T, LQUEUE_NODE>::const_reverse_iterator LQueue<T, LQUEUE_NODE>::rbegin() const
{
    return const_reverse_iterator(tail_->prev_);
}

template <typename T, typename LQUEUE_NODE>
typename LQueue<T, LQUEUE_NODE>::const_reverse_iterator LQueue<T, LQUEUE_NODE>::rend() const
{
    return const_reverse_iterator(tail_);
}

// LQueue_Iterator

template <typename T, typename LQUEUE_NODE> T &LQueue_Iterator<T, LQUEUE_NODE>::operator*() const
{
    return pos_->item_;
}

template <typename T, typename LQUEUE_NODE> LQueue_Iterator<T, LQUEUE_NODE> &LQueue_Iterator<T, LQUEUE_NODE>::operator++()
{
    pos_ = pos_->next_;
    return *this;
}

template <typename T, typename LQUEUE_NODE> LQueue_Iterator<T, LQUEUE_NODE> LQueue_Iterator<T, LQUEUE_NODE>::operator++(int)
{
    auto tmp = *this;
    pos_ = pos_->next_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE> LQueue_Iterator<T, LQUEUE_NODE> &LQueue_Iterator<T, LQUEUE_NODE>::operator--()
{
    pos_ = pos_->prev_;
    return *this;
}

template <typename T, typename LQUEUE_NODE> LQueue_Iterator<T, LQUEUE_NODE> LQueue_Iterator<T, LQUEUE_NODE>::operator--(int)
{
    auto tmp = *this;
    pos_ = pos_->prev_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE> bool LQueue_Iterator<T, LQUEUE_NODE>::operator==(const LQueue_Iterator &rhs) const
{
    return pos_ == rhs.pos_;
}

template <typename T, typename LQUEUE_NODE> bool LQueue_Iterator<T, LQUEUE_NODE>::operator!=(const LQueue_Iterator &rhs) const
{
    return !(*this == rhs);
}

template <typename T, typename LQUEUE_NODE>
LQueue_Iterator<T, LQUEUE_NODE>::LQueue_Iterator(LQUEUE_NODE *pos)
    : pos_(pos)
{}

// Const_LQueue_Iterator

template <typename T, typename LQUEUE_NODE> const T &Const_LQueue_Iterator<T, LQUEUE_NODE>::operator*() const
{
    return pos_->item_;
}

template <typename T, typename LQUEUE_NODE> Const_LQueue_Iterator<T, LQUEUE_NODE> &Const_LQueue_Iterator<T, LQUEUE_NODE>::operator++()
{
    pos_ = pos_->next_;
    return *this;
}

template <typename T, typename LQUEUE_NODE> Const_LQueue_Iterator<T, LQUEUE_NODE> Const_LQueue_Iterator<T, LQUEUE_NODE>::operator++(int)
{
    auto tmp = *this;
    pos_ = pos_->next_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE> Const_LQueue_Iterator<T, LQUEUE_NODE> &Const_LQueue_Iterator<T, LQUEUE_NODE>::operator--()
{
    pos_ = pos_->prev_;
    return *this;
}

template <typename T, typename LQUEUE_NODE> Const_LQueue_Iterator<T, LQUEUE_NODE> Const_LQueue_Iterator<T, LQUEUE_NODE>::operator--(int)
{
    auto tmp = *this;
    pos_ = pos_->prev_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE>
bool Const_LQueue_Iterator<T, LQUEUE_NODE>::operator==(const Const_LQueue_Iterator &rhs) const
{
    return pos_ == rhs.pos_;
}

template <typename T, typename LQUEUE_NODE>
bool Const_LQueue_Iterator<T, LQUEUE_NODE>::operator!=(const Const_LQueue_Iterator &rhs) const
{
    return !(*this == rhs);
}

template <typename T, typename LQUEUE_NODE>
Const_LQueue_Iterator<T, LQUEUE_NODE>::Const_LQueue_Iterator(LQUEUE_NODE *pos)
    : pos_(pos)
{}

// LQueue_Reverse_Iterator

template <typename T, typename LQUEUE_NODE> T &LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator*() const
{
    return pos_->item_;
}

template <typename T, typename LQUEUE_NODE> LQueue_Reverse_Iterator<T, LQUEUE_NODE> &LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator++() 
{
    pos_ = pos_->prev_;
    return *this;
}

template <typename T, typename LQUEUE_NODE> LQueue_Reverse_Iterator<T, LQUEUE_NODE> LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator++(int)
{
    auto tmp = *this;
    pos_ = pos_->prev_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE> LQueue_Reverse_Iterator<T, LQUEUE_NODE> &LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator--()
{
    pos_ = pos_->next_;
    return *this;
}

template <typename T, typename LQUEUE_NODE> LQueue_Reverse_Iterator<T, LQUEUE_NODE> LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator--(int)
{
    auto tmp = *this;
    pos_ = pos_->next_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE>
bool LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator==(const LQueue_Reverse_Iterator &rhs) const
{
    return pos_ == rhs.pos_;
}

template <typename T, typename LQUEUE_NODE>
bool LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator!=(const LQueue_Reverse_Iterator &rhs) const
{
    return !(*this == rhs);
}

template <typename T, typename LQUEUE_NODE>
LQueue_Reverse_Iterator<T, LQUEUE_NODE>::LQueue_Reverse_Iterator(LQUEUE_NODE *pos)
    : pos_(pos)
{}

// Const_LQueue_Reverse_Iterator

template <typename T, typename LQUEUE_NODE> const T &Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator*() const
{
    return pos_->item_;
}

template <typename T, typename LQUEUE_NODE>
Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE> &Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator++()
{
    pos_ = pos_->prev_;
    return *this;
}

template <typename T, typename LQUEUE_NODE>
Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE> Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator++(int)
{
    auto tmp = *this;
    pos_ = pos_->prev_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE>
Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE> &Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator--()
{
    pos_ = pos_->next_;
    return *this;
}

template <typename T, typename LQUEUE_NODE>
Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE> Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator--(int)
{
    auto tmp = *this;
    pos_ = pos_->next_;
    return tmp;
}

template <typename T, typename LQUEUE_NODE>
bool Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator==(const Const_LQueue_Reverse_Iterator &rhs) const
{
    return pos_ == rhs.pos_;
}

template <typename T, typename LQUEUE_NODE>
bool Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::operator!=(const Const_LQueue_Reverse_Iterator &rhs) const
{
    return !(*this == rhs);
}

template <typename T, typename LQUEUE_NODE>
Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>::Const_LQueue_Reverse_Iterator(LQUEUE_NODE *pos)
    : pos_(pos)
{}

#endif
