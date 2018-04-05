/**
 * Copyright (C) by J.Z. (04/05/2018 09:50)
 * Distributed under terms of the MIT license.
 */

#include "global_variables.h"

DEFINE_string(graph, "", "graph filename");
DEFINE_bool(dir, false, "directed or undirected");
DEFINE_int32(type, 0,
             "graph type:\n"
             "0: edgelist\n"
             "1: binary edgelist\n"
             "2: binary\n");
DEFINE_string(format, "",
              "format the graph: (not load into RAM)\n"
              "m: mapping graph nodes (node id starts from 0)\n"
              "n: get all nodes\n"
              "d: get degree sequence\n");
DEFINE_string(calcs, "",
              "statistics to calculate: (will load in RAM)\n"
              "b: basic statistics\n"
              "t: count closed triads\n"
              "D: BFS diameter (-testnodes:100)\n"
              "h: hops (10% effective diameter)\n"
              "W: weakly connected components\n"
              "S: strongly connected components\n");
DEFINE_string(convert, "",
              "conver graph to:\n"
              "e: binary edgelist (not load in RAM)\n"
              "b: binary (load in RAM)\n");
DEFINE_string(ext, "",
              "specify output file format, e.g., lz, gzip, txt.\n"
              "if left empty, then does not change.\n");
