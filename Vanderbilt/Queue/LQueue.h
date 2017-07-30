#ifndef _LQUEUE_H
#define _LQUEUE_H

template <typename T, typename LQUEUE_NODE> class LQueue_Iterator;
template <typename T, typename LQUEUE_NODE> class Const_LQueue_Iterator;
template <typename T, typename LQUEUE_NODE> class LQueue_Reverse_Iterator;
template <typename T, typename LQUEUE_NODE> class Const_LQueue_Reverse_Iterator;

template <typename T> struct LQueue_Node { // defines an element in the <LQueue>
    LQueue_Node(LQueue_Node *next, LQueue_Node *prev);
    LQueue_Node(); // doesn't initialize <item_>, helpful to implement the dummy node in a concise way
    ~LQueue_Node();

    void *operator new(size_t); // allocate a new LQueue_Node trying first from the free_list_ and if that's empty try from the global ::operator new
    void operator delete(void *ptr); // return <ptr> to the <free_list_>.
    
    void unlink() { next_->prev_ = prev_; prev_->next_ = next_; next_ = this; prev_ = this; }

    static void free_list_add(void *ptr);
    static void free_list_allocate(size_t n); // preallocate <n> <LQueue_Nodes> and store them on the <free_list_>
    static void free_list_release(); // returns all dynamic memory on the free list to the free store

    static LQueue_Node *free_list_; // head of the "free list", which is a stack of <LQueue_Nodes> used to speed up allocation.
    
    T item_;
    LQueue_Node *next_;
    LQueue_Node *prev_;
};

/**
 * This queue is a circular linked list.  The <tail_> pointer points to a dummy node which makes implementation much easier
 * (particularly iterator traversal).  When enqueuing an item, the dummy node contains the new item and points to a new dummy node.Therefore the
 * head of the list is always <tail_ -> next_>. Dequeuing an object gets rid of the head node and makes the dummy node point to the new head.
 */
template <typename T, typename LQUEUE_NODE = LQueue_Node<T>> class LQueue {
    friend class LQueue_Iterator<T, LQUEUE_NODE>;
    friend class Const_LQueue_Iterator<T, LQUEUE_NODE>;
    friend class LQueue_Reverse_Iterator<T, LQUEUE_NODE>;
    friend class Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE>;
public:
    class Underflow {};
    class Overflow {};

    LQueue(size_t size_hint = 0);
    LQueue(const LQueue &rhs);
    LQueue &operator=(const LQueue &rhs);
    ~LQueue();

    void enqueue(const T &new_item); // throws the <Overflow> exception if the queue is full, e.g. if memory is exhausted
    void dequeue(); // throws the <Underflow> exception if the queue is empty

    T front() const; // throws the <Underflow> exception if the queue is empty
    bool is_empty() const { return size() == 0; }
    bool is_full() const { return false; }
    size_t size() const { return count_; }

    bool operator==(const LQueue &rhs) const;
    bool operator!=(const LQueue &rhs) const;

    void swap(LQueue &new_queue); // does not throw an exception

    typedef T value_type;
    typedef LQueue_Iterator<T, LQUEUE_NODE> iterator;
    typedef Const_LQueue_Iterator<T, LQUEUE_NODE> const_iterator;
    typedef LQueue_Reverse_Iterator<T, LQUEUE_NODE> reverse_iterator;
    typedef Const_LQueue_Reverse_Iterator<T, LQUEUE_NODE> const_reverse_iterator;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

protected:
    void dequeue_i(); // dequeue without checking if the queue is empty

private:
    LQUEUE_NODE *tail_; // since the list is circular, the head of the queue is always this->tail_->next_
    size_t count_;
};

template <typename T, typename LQUEUE_NODE> class LQueue_Iterator {
public:
    friend class LQueue<T, LQUEUE_NODE>;

    T &operator*() const;

    LQueue_Iterator &operator++();
    LQueue_Iterator operator++(int);
    LQueue_Iterator &operator--();
    LQueue_Iterator operator--(int);

    bool operator==(const LQueue_Iterator &rhs) const;
    bool operator!=(const LQueue_Iterator &lhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

protected:
    LQueue_Iterator(LQUEUE_NODE *pos);

private:
    LQUEUE_NODE *pos_; // the position in the linked list
};

template <typename T, typename LQUEUE_NODE> class Const_LQueue_Iterator {
public:
    friend class LQueue<T, LQUEUE_NODE>;

    const T &operator*() const;

    Const_LQueue_Iterator &operator++();
    Const_LQueue_Iterator operator++(int);
    Const_LQueue_Iterator &operator--();
    Const_LQueue_Iterator operator--(int);

    bool operator==(const Const_LQueue_Iterator &rhs) const;
    bool operator!=(const Const_LQueue_Iterator &lhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

protected:
    Const_LQueue_Iterator(LQUEUE_NODE *pos);

private:
    LQUEUE_NODE *pos_;
};

template <typename T, typename LQUEUE_NODE> class LQueue_Reverse_Iterator {
public:
    friend class LQueue<T, LQUEUE_NODE>;

    T &operator*() const;

    LQueue_Reverse_Iterator &operator++(); // different semantics from a non-reverse iterator
    LQueue_Reverse_Iterator operator++(int);
    LQueue_Reverse_Iterator &operator--();
    LQueue_Reverse_Iterator operator--(int);

    bool operator==(const LQueue_Reverse_Iterator &rhs) const;
    bool operator!=(const LQueue_Reverse_Iterator &lhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

protected:
    LQueue_Reverse_Iterator(LQUEUE_NODE *pos);

private:
    LQUEUE_NODE *pos_;
};

template <typename T, typename LQUEUE_NODE> class Const_LQueue_Reverse_Iterator {
public:
    friend class LQueue<T, LQUEUE_NODE>;
    
    const T &operator*() const;

    Const_LQueue_Reverse_Iterator &operator++();
    Const_LQueue_Reverse_Iterator operator++(int);
    Const_LQueue_Reverse_Iterator &operator--();
    Const_LQueue_Reverse_Iterator operator--(int);

    bool operator==(const Const_LQueue_Reverse_Iterator &rhs) const;
    bool operator!=(const Const_LQueue_Reverse_Iterator &lhs) const;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef int difference_type;

protected:
    Const_LQueue_Reverse_Iterator(LQUEUE_NODE *pos);

private:
    LQUEUE_NODE *pos_;
};

#include "LQueue.cpp"
#endif
