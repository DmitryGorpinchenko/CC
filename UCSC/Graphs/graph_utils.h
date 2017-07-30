// ************************************************************************************
//  file containing useful utility data types and functions for working on graphs
//
//  author: Dmitry Gorpinchenko
//
// ************************************************************************************

#pragma once

#include <vector>
#include <memory>

namespace Graphs {
    
class Graph;
struct Edge;

//alias to repeset path in a graph
using Path = std::vector<int>;

//alias to represent a MST
using MST = std::vector<Edge>;

//strcut to represent an Edge in MST
struct Edge {    
    Edge(int i, int j, double w) : i(i), j(j), w(w) {}
    
    int i, j;
    double w;
};

//struct to encapsulate edge cost range
struct EdgeWeightRange {
    EdgeWeightRange(double lo, double hi) : lo(lo), hi(hi) {}
    
    double lo, hi;
};   

// ***********************************************************************************
// class for computing all Single(S) Source(S) Shortest(S) Paths
//
// ***********************************************************************************
class SSSPathFinder {
public:
    //creates an instance of this class and computes all sssps
    SSSPathFinder(const Graph& G, int s);
    ~SSSPathFinder();

    //gets cost of path to node i from source s
    //thorws std::out_of_range exception on invalid input 
    double GetCost(int i) const;
    
    //returns path to node i from source s
    //thorws std::out_of_range exception on invalid input
    Path GetPath(int i) const;
    
    //calculates and average ssspaths cost
    double CalcAvgCost() const;
    
private:
    struct ssspf_impl;
    std::unique_ptr<ssspf_impl> impl_;    
};

//finds a MST of the graph G
//G - input, mst - output parameter
double FindMST(const Graph& G, MST* mst);

//function for generating random graphs with specified edge density and range for edge costs
void GenerateRandomGraph(double density, EdgeWeightRange range, Graph* G);
    
}
