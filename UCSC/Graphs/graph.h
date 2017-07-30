// *****************************************************
//  class representing Graph ADT
//
//  author: Dmitry Gorpinchenko
// 
// *****************************************************

#pragma once

#include <unordered_map>
#include <memory>
#include <limits>

namespace Graphs {

//useful type alias to represent neighbours of node
using edge_list = std::unordered_map<int, double>;
 
//infinite distance representing broken direct or inderect connections between nodes
extern const double c_max_limit;
 
class Graph {
public:
    //creates graph with V nodes and no edges
    explicit Graph(int V);
    
    //copies other graph into graph under construction
    Graph(const Graph& other);
    
    //constructs a graph from the input file
    Graph(const std::string& filename);
    
    //frees resources allocated by graph
    ~Graph();
    
    //gets number of nodes in a graph
    int V() const;
    
    //gets number of edges in a graph
    int E() const;
    
    //tests for existence of edge between nodes i and j
    //throws std::out_of_range exception on invalid input
    bool HasEdge(int i, int j) const;
    
    //gets heighbours of node i
    const edge_list& Adj(int i) const;
    
    //adds an edge between nodes i and j
    //no-op if edge already in graph
    //throws std::out_of_range exception on invalid input
    void Add(int i, int j, double weight);
    
    //deletes an edge between nodes i and j if any
    //throws std::out_of_range exception on invalid input
    void Del(int i, int j);
    
    //gets cost of edge between nodes i and j
    //returns c_max_limit if no edge between i and j exists
    //throws std::out_of_range exception on invalid input
    double GetWeight(int i, int j) const;
    
    //removes edge if weight is infinite
    //adds edge if it is not already there
    //sets an edge cost between i and j to weight 
    //throws std::out_of_range on invalid nodes or negative weight
    void SetWeight(int i, int j, double weight);
        
private:
    struct graph_impl; 
    std::unique_ptr<graph_impl> impl_;
};    
    
}
