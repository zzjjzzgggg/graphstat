/**
 * Copyright (C) by J.Z. (04/05/2018 09:59)
 * Distributed under terms of the MIT license.
 */

#ifndef __CONVERT_GRAPH_H__
#define __CONVERT_GRAPH_H__

#include "stdafx.h"

// Declariations
void saveBinaryEdgelist();

template <class Graph>
void saveBinary(const Graph& graph);

// Implementations
template <class Graph>
void saveBinary(const Graph& graph) {
    std::string out_fnm = strutils::insertMiddle(FLAGS_graph, "bin", FLAGS_ext);
    graph.save(out_fnm);
    printf("binary graph saved to %s\n", out_fnm.c_str());
}

#endif /* __CONVERT_GRAPH_H__ */
