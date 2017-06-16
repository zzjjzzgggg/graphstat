#include "stdafx.h"

#include <gflags/gflags.h>

DEFINE_string(graph, "", "graph filename");
DEFINE_bool(dir, false, "directed or undirected");
DEFINE_int32(type, 0,
             "graph type:\n"
             "0: edgelist\n"
             "1: binary edgelist\n"
             "2: binary\n");
DEFINE_string(format, "",
              "format the graph: (not load into RAM)\n"
              "m: re-mapping graph nodes (node id starts from 0)\n"
              "n: get all nodes\n"
              "d: get degree sequence\n");
DEFINE_string(calcs, "",
              "statistics to calculate: (will load in RAM)\n"
              "b: basic statistics\n"
              "t: count closed triads\n"
              "D: BFS diameter (-testnodes:100)\n"
              "h: hops (10% effective diameter)\n"
              "w: largest weakly connected components\n"
              "s: largest strongly connected components\n");
DEFINE_string(convert, "",
              "conver graph to:\n"
              "e: binary edgelist (not load in RAM)\n"
              "b: binary (load in RAM)\n");
DEFINE_string(ext, "",
              "specify output file format, e.g., lz, gzip, txt.\n"
              "if left empty, then does not change.\n");

void mappingNodes() {
    printf("re-mapping graph nodes.\nassume the input is a text edgelist.\n");
    string out_fnm = strutils::insertMiddle(FLAGS_graph, "mapped", FLAGS_ext);
    auto po = ioutils::getIOOut(out_fnm);
    int id = 0;
    unordered_map<string, int> nidmap;
    ioutils::TSVParser ss{FLAGS_graph};
    while (ss.next()) {
        string src = ss.get<string>(0);
        string dst = ss.get<string>(1);
        if (nidmap.find(src) == nidmap.end()) nidmap[src] = (id++);
        if (nidmap.find(dst) == nidmap.end()) nidmap[dst] = (id++);
        po->save("{}\t{}\n"_format(nidmap[src], nidmap[dst]));
    }
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
        unordered_map<int, int> id_seq, od_seq;
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
        unordered_map<int, int> deg_seq;
        while (ss.next()) {
            int src = ss.get<int>(0), dst = ss.get<int>(1);
            deg_seq[src]++;
            deg_seq[dst]++;
        }
        ioutils::saveMap(deg_seq, strutils::insertMiddle(FLAGS_graph, "dseq"));
    }
}

void saveBinaryEdgelist() {
    string out_fnm = strutils::insertMiddle(FLAGS_graph, "be", FLAGS_ext);
    auto po = ioutils::getIOOut(out_fnm);
    ioutils::TSVParser ss(FLAGS_graph);
    while (ss.next()) {
        int src = ss.get<int>(0), dst = ss.get<int>(1);
        po->save(src);
        po->save(dst);
    }
    printf("binary edgelist saved to %s\n", out_fnm.c_str());
}

template <class Graph>
void calcs(const Graph& graph) {
    if (FLAGS_calcs.find('b') != string::npos) {
        printf("nodes:%d edges:%d\n", graph.getNodes(), graph.getEdges());
    }
}

template <class Graph>
void saveBinary(const Graph& graph) {
    string out_fnm = strutils::insertMiddle(FLAGS_graph, "bin", FLAGS_ext);
    graph.save(out_fnm);
    printf("binary graph saved to %s\n", out_fnm.c_str());
}

template <class Graph>
void proc(const Graph& graph) {
    if (FLAGS_dir)
        printf("Directed ");
    else
        printf("Undirected ");
    printf("graph loaded. Nodes:%d, Edges:%d\n\n", graph.getNodes(),
           graph.getEdges());
    if (!FLAGS_calcs.empty()) calcs(graph);
    if (!FLAGS_convert.empty() && FLAGS_convert.find('b') != string::npos)
        saveBinary(graph);
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("usage:");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    if (!FLAGS_format.empty()) {
        // re-mapping nodes
        if (FLAGS_format.find('m') != string::npos) mappingNodes();
        // save nodes in the graph
        if (FLAGS_format.find('n') != string::npos) saveNodes();
        // degree sequence
        if (FLAGS_format.find('d') != string::npos) getDegSeq();
        return 0;
    }

    if (!FLAGS_convert.empty() && FLAGS_convert.find('e') != string::npos) {
        // save binary edgelist
        saveBinaryEdgelist();
        return 0;
    }

    if (FLAGS_dir) {
        graph::DGraph graph;
        switch (FLAGS_type) {
            case 0:
                graph = graph::loadEdgeList<graph::DGraph>(FLAGS_graph);
                break;
            case 1:
                graph = graph::loadBinEdgeList<graph::DGraph>(FLAGS_graph);
                break;
            case 2:
                graph = graph::loadBinary<graph::DGraph>(FLAGS_graph);
                break;
            default:
                break;
        }
        proc(graph);
    } else {
        graph::UGraph graph;
        switch (FLAGS_type) {
            case 0:
                graph = graph::loadEdgeList<graph::UGraph>(FLAGS_graph);
                break;
            case 1:
                graph = graph::loadBinEdgeList<graph::UGraph>(FLAGS_graph);
                break;
            case 2:
                graph = graph::loadBinary<graph::UGraph>(FLAGS_graph);
                break;
            default:
                break;
        }
        printf("Undirected graph loaded. Nodes:%d, Edges:%d\n\n",
               graph.getNodes(), graph.getEdges());
        proc(graph);
    }

    printf("\ncost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
