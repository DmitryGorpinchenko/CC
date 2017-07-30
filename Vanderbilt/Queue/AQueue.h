#if !defined (_AQUEUE_H)
#define _AQUEUE_H

#include "Array.h"

template <typename T, typename ARRAY> class AQueue_Iterator;
template <typename T, typename ARRAY> class Const_AQueue_Iterator;
template <typename T, typename ARRAY> class AQueue_Reverse_Iterator;
template <typename T, typename ARRAY> class Const_AQueue_Reverse_Iterator;

template <typename T, typename ARRAY = Array<T>> class AQueue {
    friend class AQueue_Iterator<T, ARRAY>;
    friend class Const_AQueue_Iterator<T, ARRAY>;
    friend class AQueue_Reverse_Iterator<T, ARRAY>;
    friend class Const_AQueue_Reverse_Iterator<T, ARRAY>;
public:
    typedef T value_type;

    class Underflow {};
    class Overflow {};

    AQueue(size_t size);
    AQueue(const AQueue<T, ARRAY> &rhs);
    AQueue<T, ARRAY> &operator=(const AQueue<T, ARRAY> &rhs);

    void enqueue(const T &new_item); // throws the <Overflow> exception if the queue is full
    void dequeue(void); // throws the <Underflow> exception if the queue is empty
    T front() const; // throws the <Underflow> exception if the queue is empty
    
    bool is_empty() const;
    bool is_full() const;
    size_t size() const;

    bool operator==(const AQueue<T, ARRAY> &rhs) const; // true if sizes and elements of queues are equal
    bool operator!=(const AQueue<T, ARRAY> &s) const;
    
    void swap(AQueue<T, ARRAY> &new_aqueue);

    typedef AQueue_Iterator<T, ARRAY> iterator;
    typedef Const_AQueue_Iterator<T, ARRAY> const_iterator;
    typedef AQueue_Reverse_Iterator<T, ARRAY> reverse_iterator;
    typedef Const_AQueue_Reverse_Iterator<T, ARRAY> const_reverse_iterator;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

protected:
    size_t increment(size_t index) const; // calc the appropriate index when incrementing 
    size_t decrement(size_t index) const; // calc the appropriate index when decrementing

private:
    ARRAY queue_;
    size_t head_; // index location of the head (front) item 
    size_t tail_; // index location of the tail (dummy) item 
    size_t count_;
};

template <typename T, typename ARRAY> class AQueue_Iterator {
public:
    AQueue_Iterator(AQueue<T, ARRAY> &queue, size_t pos = 0);

    T &operator*() const;

    AQueue_Iterator<T, ARRAY> &operator++();
    AQueue_Iterator<T, ARRAY> operator++(int);
    AQueue_Iterator<T, ARRAY> &operator--();
    AQueue_Iterator<T, ARRAY> operator--(int);

    bool operator==(const AQueue_Iterator<T, ARRAY> &rhs) const;
    bool operator!=(const AQueue_Iterator<T, ARRAY> &lhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

private:
    AQueue<T, ARRAY> &aqueue_ref_;
    size_t pos_;
};

template <typename T, typename ARRAY> class Const_AQueue_Iterator {
public:
    Const_AQueue_Iterator(const AQueue<T, ARRAY> &queue, size_t pos = 0);

    const T &operator*() const;

    Const_AQueue_Iterator<T, ARRAY> &operator++();
    Const_AQueue_Iterator<T, ARRAY> operator++(int);
    Const_AQueue_Iterator<T, ARRAY> &operator--();
    Const_AQueue_Iterator<T, ARRAY> operator--(int);

    bool operator==(const Const_AQueue_Iterator<T, ARRAY> &rhs) const;
    bool operator!=(const Const_AQueue_Iterator<T, ARRAY> &lhs) const;
    
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

private:
    const AQueue<T, ARRAY> &aqueue_ref_;
    size_t pos_;
};

template <typename T, typename ARRAY> class AQueue_Reverse_Iterator {
public:
    AQueue_Reverse_Iterator(AQueue<T, ARRAY> &queue, size_t pos = 0);

    T& operator*() const;
    
    AQueue_Reverse_Iterator<T, ARRAY> &operator++(); // different semantics from a non-reverse iterator
    AQueue_Reverse_Iterator<T, ARRAY> operator++(int);
    AQueue_Reverse_Iterator<T, ARRAY> &operator--();
    AQueue_Reverse_Iterator<T, ARRAY> operator--(int);

    bool operator==(const AQueue_Reverse_Iterator<T, ARRAY> &rhs) const;
    bool operator!=(const AQueue_Reverse_Iterator<T, ARRAY> &lhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

private:
    AQueue<T, ARRAY> &aqueue_ref_;
    size_t pos_;
};

template <typename T, typename ARRAY> class Const_AQueue_Reverse_Iterator {
public:
    Const_AQueue_Reverse_Iterator(const AQueue<T, ARRAY> &queue, size_t pos = 0);

    const T& operator*() const;

    Const_AQueue_Reverse_Iterator<T, ARRAY> &operator++(); // different semantics from a non-reverse iterator
    Const_AQueue_Reverse_Iterator<T, ARRAY> operator++(int);
    Const_AQueue_Reverse_Iterator<T, ARRAY> &operator--();
    Const_AQueue_Reverse_Iterator<T, ARRAY> operator--(int);

    bool operator==(const Const_AQueue_Reverse_Iterator<T, ARRAY> &rhs) const;
    bool operator!=(const Const_AQueue_Reverse_Iterator<T, ARRAY> &lhs) const;
    
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

private:
    const AQueue<T, ARRAY> &aqueue_ref_;
    size_t pos_;
};

#include "AQueue.cpp"
#endif
