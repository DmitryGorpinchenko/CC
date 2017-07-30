#if !defined (_Queue_H)
#define _Queue_H

#include <stdlib.h>

template <typename T> class Queue {
public:
    typedef T value_type; // trait to use when freeing up memory from LQueue

    virtual ~Queue() = 0;

    class Underflow {};
    class Overflow {};

    virtual void enqueue(const T &new_item) = 0;
    virtual void dequeue() = 0;
    virtual T front() const = 0;

    virtual bool is_empty() const = 0;
    virtual bool is_full() const = 0;
    virtual size_t size() const = 0;
};

template <typename T, typename QUEUE> class Queue_Adapter : public Queue<T> {
public:
    Queue_Adapter(size_t size);

    bool operator==(const Queue_Adapter &rhs) const;
    bool operator!=(const Queue_Adapter &rhs) const;

    void enqueue(const T &new_item) override;
    void dequeue() override;
    T front() const override;
    bool is_empty() const override;
    bool is_full() const override;
    size_t size() const override;

private:
    QUEUE queue_;
};

#include "Queue.cpp"

#endif
