// ************************************************************************************
//  file containing implementation of useful utility data types and functions 
//  for working with graphs
//
//  author: Dmitry Gorpinchenko
//
// ************************************************************************************

#include <random>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>

#include "graph.h"
#include "graph_utils.h"

namespace Graphs {

//type aliases for making code more self-documenting
using search_node = std::pair<double, int>; //'first' in pair is a distance and 'second' is the corresponding vertex
using open_set = std::priority_queue<search_node, std::vector<search_node>, std::greater<search_node> >; 

//constant representing an absence of path to the node
static const int no_path = -1;
//constant representing a path's root ('parent' of source node, so to speak)
static const int no_parent = -2;

// ******************************************************************************************************************
//  struct representing private implementation details of path finder
//
// ******************************************************************************************************************
struct SSSPathFinder::ssspf_impl {
    std::vector<double> costs;
    std::vector<int> parent_tree;
    
    ssspf_impl(const Graph& G, int s) : costs(G.V(), c_max_limit), parent_tree(G.V(), no_path) {
        costs[s] = 0.0;
	    parent_tree[s] = no_parent;
    }
};

//implementation of Dijkstra's single source shortest paths algorithm
SSSPathFinder::SSSPathFinder(const Graph& G, int s) : impl_(new ssspf_impl(G, s)) {
    //initialize open set
    open_set os;
    os.push(search_node(0.0, s));
    //iterate until there are nodes in open set
    while (!os.empty()) {
        search_node curr = os.top();
        os.pop();
        for (const auto& neigh : G.Adj(curr.second)) {
            if (impl_->costs[neigh.first] > curr.first + neigh.second) {
                impl_->costs[neigh.first] = curr.first + neigh.second;
                os.push(search_node(impl_->costs[neigh.first], neigh.first));
                impl_->parent_tree[neigh.first] = curr.second;
            }
        }
    }
}

//it is nedded here due to usage of pimpl idiom
SSSPathFinder::~SSSPathFinder() = default;

double SSSPathFinder::CalcAvgCost() const {
    double total_len = std::accumulate(impl_->costs.begin(), impl_->costs.end(), 0.0, [](double acc, double next) {
        return next == c_max_limit ? acc : acc + next;            
    }); // <- adds all finite paths costs
    int paths_count = std::count_if(impl_->costs.begin(), impl_->costs.end(), [](double c) { 
        return c > 0 && c < c_max_limit; 
    }); // <- finds the number of finite length non-trivial paths
    return total_len / paths_count;
}

double SSSPathFinder::GetCost(int i) const {
    if(i < 0 || i >= impl_->costs.size()) {
        throw std::out_of_range("Node does not exist");
    }
    return impl_->costs[i];
}

Path SSSPathFinder::GetPath(int i) const {
    if (i < 0 || i >= impl_->costs.size()) {
        throw std::out_of_range("Node does not exist");
    }
    if (impl_->parent_tree[i] == no_path) {
        return Path();
    }
    Path p;
    while (i != no_parent) {
        p.push_back(i);
        i = impl_->parent_tree[i];
    }
    //reverse node order because they was inserted backwards
    std::reverse(p.begin(), p.end());
    return p;
}

double FindMST(const Graph& G, MST* mst) {
    //construct an array of distinct edges
    std::vector<Edge> edges;
    for (int i = 0; i < G.V(); ++i) {
        for (const auto& neigh : G.Adj(i)) {
            if (i < neigh.first) {
                edges.push_back(Edge(i, neigh.first, neigh.second));
            }
        }
    }
    //sort edges according to their cost
    std::sort(edges.begin(), edges.end(), [](Edge e1, Edge e2) {
        return e1.w < e2.w;
    });
    //simulate union-find data structure
    int components_num = G.V();
    std::vector<int> component_id(components_num);
    for (int i = 0; i < G.V(); ++i) {
        component_id[i] = i;
    }
    for (const auto& e : edges) {
        if (components_num == 1) {
            break; //stops if all nodes are connected
        }
        if (component_id[e.i] != component_id[e.j]) { //avoids loops in MST
            int union_id = component_id[e.i];
            for (int i = 0; i < G.V(); ++i) { //performs union of two components
                if (component_id[i] == component_id[e.j]) {
                    component_id[i] = union_id;
                }
            }
            --components_num;
            mst->push_back(e);
        }
    }
    return std::accumulate(mst->begin(), mst->end(), 0.0, [](double acc, Edge next) { return acc + next.w; });
}

void GenerateRandomGraph(double density, EdgeWeightRange range, Graph* G) {
    std::default_random_engine edge_gen(time(nullptr)), weight_gen(time(nullptr));
    std::uniform_real_distribution<double> edge_distr(0.0, 1.0), weight_distr(range.lo, range.hi);
    for (int i = 0; i < G->V(); ++i) {
        for (int j = i + 1; j < G->V(); ++j) {
            if (edge_distr(edge_gen) < density) {
                G->Add(i, j, weight_distr(weight_gen));
            }
        }
    } 
}
    
}
