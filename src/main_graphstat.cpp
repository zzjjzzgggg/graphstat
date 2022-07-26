#include "format_graph.h"
#include "calcs_graph.h"
#include "convert_graph.h"

template <class Graph>
void proc(const Graph& graph) {
    if (FLAGS_dir)
        printf("Directed ");
    else
        printf("Undirected ");
    printf("graph loaded. Nodes:%d, Edges:%d\n\n", graph.getNodes(),
           graph.getEdges());

    if (!FLAGS_calcs.empty()) calcs(graph);
    if (!FLAGS_convert.empty() && FLAGS_convert.find('b') != std::string::npos)
        saveBinary(graph);
}

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage("{} {}"_format(__DATE__, __TIME__));
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    osutils::Timer tm;

    if (!FLAGS_format.empty()) {
        // re-mapping nodes
        if (FLAGS_format.find('m') != std::string::npos) mappingNodes();
        // save nodes in the graph
        if (FLAGS_format.find('n') != std::string::npos) saveNodes();
        // degree sequence
        if (FLAGS_format.find('d') != std::string::npos) getDegSeq();
        return 0;
    }

    if (!FLAGS_convert.empty() &&
        FLAGS_convert.find('e') != std::string::npos) {
        // save binary edgelist
        saveBinaryEdgelist();
        return 0;
    }

    if (FLAGS_dir) {
        dir::DGraph graph;
        switch (FLAGS_type) {
            case 0:
                graph = loadEdgeList<dir::DGraph>(FLAGS_graph);
                break;
            case 1:
                graph = loadBinEdgeList<dir::DGraph>(FLAGS_graph);
                break;
            case 2:
                graph = loadBinary<dir::DGraph>(FLAGS_graph);
                break;
            default:
                break;
        }
        proc(graph);
    } else {
        undir::UGraph graph;
        switch (FLAGS_type) {
            case 0:
                graph = loadEdgeList<undir::UGraph>(FLAGS_graph);
                break;
            case 1:
                graph = loadBinEdgeList<undir::UGraph>(FLAGS_graph);
                break;
            case 2:
                graph = loadBinary<undir::UGraph>(FLAGS_graph);
                break;
            default:
                break;
        }
        proc(graph);
    }

    printf("\ncost time %s\n", tm.getStr().c_str());
    gflags::ShutDownCommandLineFlags();
    return 0;
}
