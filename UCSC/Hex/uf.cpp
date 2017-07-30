// ************************************************************************
//  implementation of Union-Find with path compression and union by rank
//  author: Dmitry Gorpinchenko
// 
// ************************************************************************

#include <vector>
#include "uf.h"

struct UF::uf_impl {
    //mutable because we can change it in const method Find()
    mutable std::vector<int> parent;
    std::vector<int> rank;
    int n_components;
    
    uf_impl() : n_components(0) {}
    
    uf_impl(int N) : parent(N), rank(N), n_components(N) {
        for (int i = 0; i < N; ++i) {
            parent[i] = i;
        }
    }
    
    //Finds the number of component containing node i 
    //marked const because it changes state in an invisible to the outside world manner
    //(by applying so called path compression)
    int Find(int i) const {
        if (i != parent[i]) {
            parent[i] = Find(parent[i]);
        }
        return parent[i];
    }
};

UF::UF() : pimpl_(new uf_impl()) {}

UF::UF(int N) : pimpl_(new uf_impl(N)) {}

UF::UF(const UF& other) : pimpl_(new uf_impl(*other.pimpl_)) {}

//required by pimpl idiom
UF::~UF() = default;

UF& UF::operator= (const UF& that) {
    if (this != &that) {
        *this->pimpl_ = *that.pimpl_;      
    }
    return *this;
}

//implements union by rank heuristic
void UF::Union(int i, int j) {
    if ((i = pimpl_->Find(i)) == (j = pimpl_->Find(j))) {
        return;
    }
    if (pimpl_->rank[i] > pimpl_->rank[j]) {
        pimpl_->parent[j] = i;
    } else {
        pimpl_->parent[i] = j;
        if (pimpl_->rank[i] == pimpl_->rank[j]) {
            ++pimpl_->rank[j];
        }
    }
} 

bool UF::IsConnected(int i, int j) const {
    return pimpl_->Find(i) == pimpl_->Find(j);
}
