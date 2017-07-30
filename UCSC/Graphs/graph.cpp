// ***********************************************************************
//  implementation of Graph ADT using pimpl idiom
//
//  author: Dmitry Gorpinchenko
//
// ***********************************************************************

#include <vector>
#include <fstream>

#include "graph.h"

namespace Graphs {

const double c_max_limit = std::numeric_limits<double>::max();

// ************************************************************************
//  struct representing hidden private state of Graph ADT
//
// ************************************************************************
struct Graph::graph_impl {
    std::vector<edge_list> adj;
    int E;

    graph_impl(int V) : adj(V), E(0) {}
    
    //assumes edge does not already exist and nodes are valid
    //no-op if weight == c_max_limit
    void add_edge(int i, int j, double weight) {
        if (weight >= 0.0 && weight < c_max_limit) {
            adj[i][j] = adj[j][i] = weight;
            ++E;
        } else if (weight < 0.0) {
            throw std::out_of_range("Edge cost cannot be negative");
        }
    }
    
    //throws std::out_of_range exception if node does not exist in graph
    void validate_node(int i, const std::string& err) const {
        if (i < 0 || i >= adj.size()) {
            throw std::out_of_range(err);
        }
    }
    
    //checks if edge between nodes i and j possible
    void validate_edge(int i, int j) const {
        validate_node(i, "Wrong starting node");
        validate_node(j, "Wrong ending node");
    }
};

Graph::Graph(int V) : impl_(new graph_impl(V)) {}

Graph::Graph(const Graph& other) : impl_(new graph_impl(*other.impl_)) {}

Graph::Graph(const std::string& filename) {
    int V, i, j;
    double w;
    std::ifstream in(filename);
    in >> V;
    impl_.reset(new graph_impl(V));
    while (in >> i >> j >> w) {
        Add(i, j, w);
    }
}

// ***********************************************************************
//  pimpl idiom requires explicit definition of even default constructor 
//  and destructor placed after full definition of impl struct because 
//  Graph needs to know how to delete the impl object 
//
// ***********************************************************************
Graph::~Graph() = default; 

int Graph::V() const {
    return impl_->adj.size();
}

int Graph::E() const {
    return impl_->E;
}

bool Graph::HasEdge(int i, int j) const {
    impl_->validate_edge(i, j);

    return impl_->adj[i].find(j) != impl_->adj[i].end();
}

const edge_list& Graph::Adj(int i) const {
    impl_->validate_node(i, "You are trying to get neighbours for non-existing node");
    
    return impl_->adj[i];
}

void Graph::Add(int i, int j, double weight) {
    if (!HasEdge(i, j)) {
        impl_->add_edge(i, j, weight);
    }
}

void Graph::Del(int i, int j) {
    if(HasEdge(i, j)) {
        impl_->adj[i].erase(j);
        impl_->adj[j].erase(i);
        --impl_->E; //count every undirected edge once
    }
}

double Graph::GetWeight(int i, int j) const {
    if (HasEdge(i, j)) {
        return impl_->adj[i][j];
    }
    return c_max_limit;
}

void Graph::SetWeight(int i, int j, double weight) {
    if (weight == c_max_limit) {
        Del(i, j);
    } else if (!HasEdge(i, j)) {
        impl_->add_edge(i, j, weight);
    } else if (weight >= 0.0) {
        impl_->adj[i][j] = impl_->adj[j][i] = weight;
    } else {
        throw std::out_of_range("Cannot set negative edge cost");
    }
}

}
