#ifndef ARRAY_CPP
#define ARRAY_CPP

#include <stdexcept>
#include <algorithm>

#include "Array.h"

#if !defined (__INLINE__)
#define INLINE
#include "Array.inl"
#endif

// it is important to use value-initialization below to prevent valgrind memory errors
template <typename T> Array<T>::Array(size_t size) : cur_size_(size), max_size_(size), array_(new T[size]{}) {}
template <typename T> Array<T>::Array(size_t size, const T &default_value) : Array(size)
{
    std::fill_n(array_.get(), size, default_value); default_value_.reset(new T(default_value));
}
template <typename T> Array<T>::Array(const Array &s) : Array(s.size())
{
    std::copy_n(s.array_.get(), s.size(), array_.get()); if (s.default_value_) { default_value_.reset(new T(*s.default_value_)); }
}
template <typename T> Array<T> &Array<T>::operator=(const Array &s) { if (this != &s) { auto tmp = s; swap(tmp); } return *this; }
template <typename T> void Array<T>::resize(size_t new_size)
{
    if (new_size <= max_size_) {
        if (cur_size_ < new_size && default_value_) {
            std::fill(array_.get() + cur_size_, array_.get() + new_size, *default_value_);
        }
        cur_size_ = new_size;
    } else {
        auto tmp = default_value_ ? Array(new_size, *default_value_) : Array(new_size);
        std::copy_n(array_.get(), size(), tmp.array_.get());
        swap(tmp);
    }
}
template <typename T> void Array<T>::swap(Array &s) 
{
    std::swap(cur_size_, s.cur_size_); std::swap(max_size_, s.max_size_); std::swap(default_value_, s.default_value_); std::swap(array_, s.array_);
}
template <typename T> void Array<T>::set(const T &new_item, size_t index)
{
    if (!in_range(index)) { resize(index + 1); } array_[index] = new_item;
}
template <typename T> void Array<T>::get(T &item, size_t index)
{
    if (!in_range(index)) { throw std::out_of_range("invalid index"); } item = array_[index];
}
template <typename T> bool Array<T>::operator==(const Array &s) const
{ 
    return size() == s.size() && std::equal(array_.get(), array_.get() + size(), s.array_.get());
}
template <typename T> bool Array<T>::operator!=(const Array &s) const { return !(*this == s); }

template <typename T> Array_Iterator<T>::Array_Iterator(Array<T> &array, size_t pos) : array_(array), pos_(pos) {}
template <typename T> Const_Array_Iterator<T>::Const_Array_Iterator(const Array<T> &array, size_t pos) : array_(array), pos_(pos) {}

#endif
