//*********************************************************
// class representing Union-Find data structure
//
// author: Dmitry Gorpinchenko
//
//*********************************************************

#ifndef UF_H
#define UF_H

#include <memory>

class UF {
public:
    UF();
    explicit UF(int N);
    UF(const UF& other);
    ~UF();
    
    UF& operator= (const UF& that);
    
    //connects components containing nodes i and j into one
    void Union(int i, int j);
    
    //checks whether nodes i and j are in one component 
    bool IsConnected(int i, int j) const;

private:  
    struct uf_impl;
    std::unique_ptr<uf_impl> pimpl_;
};

#endif
