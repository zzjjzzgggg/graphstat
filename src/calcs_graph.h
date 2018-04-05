/**
 * Copyright (C) by J.Z. (04/05/2018 09:48)
 * Distributed under terms of the MIT license.
 */

#ifndef __CALCS_GRAPH_H__
#define __CALCS_GRAPH_H__

#include "stdafx.h"

// Declariations
template <class Graph>
void SCCAnalysis(const Graph& graph);

template <class Graph>
void calcs(const Graph& graph);


// Implementations
template <class Graph>
void SCCAnalysis(const Graph& graph) {
    printf("Strongly Connected Components Analysis:\n\n");

    double nodes = graph.getNodes();

    SCCVisitor dfs(graph);
    dfs.performDFS();

    std::unordered_map<int, int> cc_sz;
    for (auto& pr : dfs.getCNEdges()) cc_sz[pr.first]++;

    std::vector<int> sz_vec;
    for (auto& pr : cc_sz) sz_vec.push_back(pr.second);

    std::sort(sz_vec.begin(), sz_vec.end(), std::greater<int>());

    int sz = sz_vec.size();
    printf("Totally found %d strongly connected componets.\n", sz);
    int k = std::min(10, sz);
    printf("Sizes of top %d largest SCC are:\n", k);
    for (int i = 0; i < k; i++)
        printf("\t%d\t\t%.2f%%\n", sz_vec[i], sz_vec[i] / nodes * 100);
}

template <class Graph>
void calcs(const Graph& graph) {
    if (FLAGS_calcs.find('b') != std::string::npos) {
        printf("nodes:%d edges:%d\n", graph.getNodes(), graph.getEdges());
    }
    if (FLAGS_calcs.find('S') != std::string::npos) {
        if (FLAGS_dir) SCCAnalysis(graph);
    }
}

#endif /* __CALCS_GRAPH_H__ */
