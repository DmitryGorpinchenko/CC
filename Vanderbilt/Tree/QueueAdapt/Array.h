#ifndef ARRAY_H
#define ARRAY_H

#include <memory>

// Solve circular include problem
template <typename T> class Array_Iterator;
template <typename T> class Const_Array_Iterator;

template <typename T> class Array {
public:
    typedef T value_type;

    Array(size_t size);
    Array(size_t size, const T &default_value);
    Array(const Array &s);

    // Throws <std::bad_alloc> if allocation fails. If any exceptions
    // occur, however, the state of the original array remains unchanged
    // (i.e., implement "strong exception guarantee" semantics).
    Array &operator=(const Array &s);

    typedef Array_Iterator<T> iterator;
    typedef Const_Array_Iterator<T> const_iterator;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    // Set an item in the array at location index.  If <index> >=
    // <s.cur_size_> then <resize()> the array so it's big enough.
    // Throws <std::bad_alloc> if resizing the array fails.
    void set(const T &new_item, size_t index);
    void get(T &item, size_t index); // throws <std::out_of_range>

    size_t size() const;

    const T &operator[](size_t index) const;
    T &operator[](size_t index);

    bool operator==(const Array &s) const;
    bool operator!=(const Array &s) const;

    // Change the size of the array to be at least <new_size> elements.
    // If a <default_value> was given in the <Array> constructor then
    // make sure any new elements are initialized accordingly.  Throws
    // <std::bad_alloc> if allocation fails.
    void resize(size_t new_size);
    void swap(Array &s); // does not throw an exception

private:
    bool in_range(size_t index) const;
    
    // Current size of the array.  The purpose of keeping track of both 
    // <cur_size_> and <max_size_> is to avoid reallocating memory if we
    // don't have to in the set() method. 
    size_t cur_size_;
    size_t max_size_;
    
    std::unique_ptr<T> default_value_;
    std::unique_ptr<T[]> array_;
};

template <typename T> class Array_Iterator {
    friend class Array<T>;
    Array_Iterator(Array<T> &array, size_t pos = 0);
public:
    T &operator*();
    T &operator*() const;

    Array_Iterator &operator++();
    Array_Iterator operator++(int);
    Array_Iterator &operator--();
    Array_Iterator operator--(int);

    bool operator==(const Array_Iterator &rhs) const;
    bool operator!=(const Array_Iterator &lhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

private:
    Array<T> &array_;
    size_t pos_;
};

template <typename T> class Const_Array_Iterator {
    friend class Array<T>;
    Const_Array_Iterator(const Array<T> &array, size_t pos = 0);
public:
    const T &operator*() const;

    Const_Array_Iterator &operator++();
    Const_Array_Iterator operator++(int);
    Const_Array_Iterator &operator--();
    Const_Array_Iterator operator--(int);

    bool operator==(const Const_Array_Iterator &rhs) const;
    bool operator!=(const Const_Array_Iterator &rhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

private:
    const Array<T> &array_;
    size_t pos_;
};

#if defined (__INLINE__)
#define INLINE inline
#include "Array.inl"
#endif

#include "Array.cpp"

#endif
