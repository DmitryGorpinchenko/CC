#ifndef ARRAY_H
#define ARRAY_H

#include <memory>

typedef char T;

class Array {
public:
    typedef T value_type;

    explicit Array(size_t size);
    Array(size_t size, const T &default_value);
    Array(const Array &s);
    
    Array &operator=(const Array &s);

    void set(const T &new_item, size_t index);
    void get(T &item, size_t index) const;

    const T &operator[](size_t index) const;
    T &operator[](size_t index);

    size_t size() const;

    bool operator==(const Array &s) const;
    bool operator!=(const Array &s) const;

private:
    bool in_range(size_t index) const;
    void swap(Array &s);

    size_t max_size_;
    size_t cur_size_;
    std::unique_ptr<T[]> array_;
};

#if defined (__INLINE__)
#define INLINE inline
#include "Array.inl"
#endif

#endif
