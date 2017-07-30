/**
 * File: KDTree.h
 * Author: Dmitry Gorpinchenko
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include "Point.h"
#include "BoundedPQueue.h"
#include <stdexcept>
#include <cmath>
#include <map>
#include <algorithm>

// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.
using namespace std;

template <size_t N, typename ElemType>
class KDTree {
public:
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();

    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();

    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);
      
    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;

    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;

    //makes the tree empty
    void clear();

    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;

    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);

    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);
      
    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;
      
    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;

private:
    struct Node {
        ElemType value;
        Point<N> pt;
        Node* l;
        Node* r;

        Node(const Point<N>& pt, const ElemType& value, Node* l, Node* r) : l(l), r(r), pt(pt), value(value) {}
    };

    Node* findNode(const Point<N>& pt) const;
    Node* findNode(Node* x, const Point<N>& pt, int level) const;
    Node* insert(Node* x, const Point<N>& pt, const ElemType& value, int level);

    void clear(Node* x);
    void copyOther(Node* root);

    void kNNFill(Node* x, const Point<N>& pt, int level, BoundedPQueue<ElemType>* bpq) const;

    int size_;
    Node* root;
};

/** KDTree class implementation details */

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() : size_(0), root(NULL) {}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree()
{
    clear();
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const KDTree& other) : size_(0), root(NULL)
{    
    copyOther(other.root);
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>& KDTree<N, ElemType>::operator=(const KDTree& other)
{   
    if (this != &other) {
        clear();
        copyOther(other.root);
    }
    return *this;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::copyOther(Node* root)
{	
    if (root == NULL) {
        return;
    }
	
    insert(root->pt, root->value);
    copyOther(root->l);
    copyOther(root->r);
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::clear()
{	
    clear(root);
	
    //make tree empty 
    root = NULL;
    size_ = 0;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::clear(Node* x)
{
    if (x == NULL) {
        return;
    }
    clear(x->l);
    clear(x->r);
    delete x;
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::kNNValue(const Point<N>& key, size_t k) const
{	
    BoundedPQueue<ElemType> bpq(k);	
	
    kNNFill(root, key, 0, &bpq);
	
    map<ElemType, int> counts;
    while (!bpq.empty()) {
        ++counts[bpq.dequeueMin()];
    }		
    typedef const pair<ElemType, int>& KVConstRef;
    auto itr = max_element(counts.begin(), counts.end(), [](KVConstRef p1, KVConstRef p2)->bool { return p1.second < p2.second; });

    return itr->first;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::kNNFill(Node* x, const Point<N>& key, int level, BoundedPQueue<ElemType>* bpq) const
{	
    if (x == NULL) {
        return;
    }
	
    bpq->enqueue(x->value, Distance(x->pt, key));
	
    Node* other = NULL;
	
    if (key[level] < x->pt[level]) {
        kNNFill(x->l, key, (level+1) % N, bpq);
        other = x->r;
    } else {
        kNNFill(x->r, key, (level+1) % N, bpq);
        other = x->l;
    }
	
    if (bpq->size() != bpq->maxSize() || fabs(x->pt[level] - key[level]) < bpq->worst()) {
        kNNFill(other, key, (level+1) % N, bpq);
    }
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const
{
    return N;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const
{
    return size_;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const
{
    return size() == 0;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value)
{
    root = insert(root, pt, value, 0);
}

template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node*
KDTree<N, ElemType>::insert(Node* x, const Point<N>& pt, const ElemType& value, int level)
{
    if (x == NULL) {
        x = new Node(pt, value, NULL, NULL);
        ++size_;
    } else if (pt == x->pt) {
        x->value = value;
    } else if (pt[level] < x->pt[level]) {
        x->l = insert(x->l, pt, value, (level+1) % N);
    } else {
        x->r = insert(x->r, pt, value, (level+1) % N);
    }
	return x;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const
{
    return findNode(pt) != NULL;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[] (const Point<N>& pt)
{
    Node* tmp = findNode(pt);
    if (tmp != NULL) {
        return tmp->value;
    }

    insert(pt, ElemType());
    return findNode(pt)->value;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at(const Point<N>& pt)
{
    const KDTree& tmp = *this;
    return const_cast<ElemType&>(tmp.at(pt));
}

template <size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at(const Point<N>& pt) const
{
    Node* tmp = findNode(pt);
    if (tmp == NULL) {
        throw out_of_range("Point does not exist in the tree!");
    }
    return tmp->value;
}

template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node*
KDTree<N, ElemType>::findNode(const Point<N>& pt) const
{
    return findNode(root, pt, 0);
}

template <size_t N, typename ElemType>
typename KDTree<N, ElemType>::Node*
KDTree<N, ElemType>::findNode(Node* x, const Point<N>& pt, int level) const
{
    if (x == NULL) {
        return NULL;
    } else if (x->pt == pt) {
        return x;
    } else if (pt[level] < x->pt[level]) {
        return findNode(x->l, pt, (level + 1) % N);
    } else {
        return findNode(x->r, pt, (level + 1) % N);
    }
}

#endif // KDTREE_INCLUDED
