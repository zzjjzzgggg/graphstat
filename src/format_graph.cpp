/**
 * Copyright (C) by J.Z. (04/05/2018 09:36)
 * Distributed under terms of the MIT license.
 */

#include "format_graph.h"


void mappingNodes() {
    printf("Mapping graph nodes.\nAssume the input is a text edgelist.\n");
    std::string out_fnm =
        strutils::insertMiddle(FLAGS_graph, "mapped", FLAGS_ext);
    auto po = ioutils::getIOOut(out_fnm);
    int id = 0;
    std::unordered_map<std::string, int> nidmap;
    ioutils::TSVParser ss{FLAGS_graph};
    while (ss.next()) {
        std::string src = ss.get<std::string>(0);
        std::string dst = ss.get<std::string>(1);
        if (nidmap.find(src) == nidmap.end()) nidmap[src] = id++;
        if (nidmap.find(dst) == nidmap.end()) nidmap[dst] = id++;
        po->save("{}\t{}\n"_format(nidmap[src], nidmap[dst]));
    }
    printf("Mapped graph is saved to %s\n", out_fnm.c_str());
    ioutils::saveMap(nidmap, strutils::insertMiddle(FLAGS_graph, "nmap"));
    printf("Node range [0, %d]\n", id - 1);
}

void saveNodes() {
    int num_edges = 0;
    std::unordered_set<int> nodes;
    ioutils::TSVParser ss{FLAGS_graph};
    while (ss.next()) {
        nodes.insert(ss.get<int>(0));
        nodes.insert(ss.get<int>(1));
        num_edges++;
    }
    printf("nodes: %lu, edges: %d\n", nodes.size(), num_edges);
    ioutils::saveSet(nodes, strutils::insertMiddle(FLAGS_graph, "nodes"));
}

void getDegSeq() {
    std::unordered_set<int> nodes;
    ioutils::TSVParser ss{FLAGS_graph};
    if (FLAGS_dir) {
        std::unordered_map<int, int> id_seq, od_seq;
        while (ss.next()) {
            int src = ss.get<int>(0), dst = ss.get<int>(1);
            od_seq[src]++;
            id_seq[dst]++;
            nodes.insert(src);
            nodes.insert(dst);
        }
        for (int v : nodes) {
            if (od_seq.find(v) == od_seq.end()) od_seq[v] = 0;
            if (id_seq.find(v) == id_seq.end()) id_seq[v] = 0;
        }
        ioutils::saveMap(id_seq, strutils::insertMiddle(FLAGS_graph, "idseq"));
        ioutils::saveMap(od_seq, strutils::insertMiddle(FLAGS_graph, "odseq"));
    } else {
        std::unordered_map<int, int> deg_seq;
        while (ss.next()) {
            int src = ss.get<int>(0), dst = ss.get<int>(1);
            deg_seq[src]++;
            deg_seq[dst]++;
        }
        ioutils::saveMap(deg_seq, strutils::insertMiddle(FLAGS_graph, "dseq"));
    }
}
