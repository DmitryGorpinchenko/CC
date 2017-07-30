// *************************************************************************
//  program to compute an MST of an input graph and to perform Monte Carlo 
//  simulation for computing an average shortest path length in a Graph
//
//  author: Dmitry Gorpinchenko
// 
// *************************************************************************

#include <iostream>

#include "graph.h"
#include "graph_utils.h"

void run_sssp_simulation();
void run_mst_test(const std::string& filename);

int main(int argc, char* argv[]) {
    //run_sssp_simulation();
    run_mst_test(argv[1]);
    return 0;
}

void run_sssp_simulation() {
    int n_trials = 1000; //number of trials to perform for each edge density
    int V = 50; //number of verticies
    double lo = 1.0; //lower bound of edge cost
    double hi = 10.0; //upper bound of edge cost
    double densities[] = { 0.2, 0.4 }; //edge densities
    int source = 0; //source node wrt which compute shortest paths
    for (auto density : densities) {
        std::cout << "Vertex number: " << V << "\t|\tEdge cost range: (" << lo << ", " << hi << ")";
        std::cout << "\t|\tEdge density: " << density << "\t|\tAvg SP cost: ";
        double avg = 0.0;
        for (int i = 0; i < n_trials; ++i) {
            Graphs::Graph G(V);
            Graphs::GenerateRandomGraph(density, Graphs::EdgeWeightRange(lo, hi), &G);  
            Graphs::SSSPathFinder pf(G, source);
            avg += pf.CalcAvgCost();
        }
        std::cout << (avg / n_trials) << std::endl;
    }
}

void run_mst_test(const std::string& filename) {
    Graphs::Graph G(filename);
    Graphs::MST mst;
    double mst_cost = Graphs::FindMST(G, &mst);
    std::cout << "MST cost = " << mst_cost << std::endl;
    std::cout << "MST edges:" << std::endl;
    for (auto& e : mst) {
        std::cout << e.i << " " << e.j << " " << e.w << std::endl;
    }
}
