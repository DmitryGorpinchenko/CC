#ifndef ARRAY_C
#define ARRAY_C

#include <stdexcept>
#include <algorithm>

#include "Array.h"

#if !defined (__INLINE__)
#define INLINE 
#include "Array.inl"
#endif

Array::Array(size_t size) : cur_size_(size), max_size_(size), array_(new T[size]) {}
Array::Array(size_t size, const T &default_value) : Array(size) { std::fill_n(array_.get(), size, default_value); }
Array::Array(const Array &s) : Array(s.size()) { std::copy_n(s.array_.get(), s.size(), array_.get()); }

Array &Array::operator=(const Array &s)
{
    if (this != &s) {
        if (max_size_ >= s.size()) {
            std::copy_n(s.array_.get(), s.size(), array_.get());
            cur_size_ = s.size();
        } else {
            auto tmp = s;
            swap(tmp);
        }
    }
    return *this;
}

void Array::set(const T &new_item, size_t index)
{
    if (!in_range(index)) {
        throw std::out_of_range("invalid index");
    }
    array_[index] = new_item;
}

void Array::get(T &item, size_t index) const
{
    if (!in_range(index)) {
        throw std::out_of_range("invalid index");
    }
    item = array_[index];
}

bool Array::operator==(const Array &s) const { return size() == s.size() && std::equal(array_.get(), array_.get() + size(), s.array_.get()); }
bool Array::operator!=(const Array &s) const { return !(*this == s); }

void Array::swap(Array &s)
{
    std::swap(cur_size_, s.cur_size_);
    std::swap(max_size_, s.max_size_);
    std::swap(array_, s.array_);
}

#endif
