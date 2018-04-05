/**
 * Copyright (C) by J.Z. (04/05/2018 10:00)
 * Distributed under terms of the MIT license.
 */

#include "convert_graph.h"

void saveBinaryEdgelist() {
    std::string out_fnm = strutils::insertMiddle(FLAGS_graph, "be", FLAGS_ext);
    auto po = ioutils::getIOOut(out_fnm);
    ioutils::TSVParser ss(FLAGS_graph);
    while (ss.next()) {
        int src = ss.get<int>(0), dst = ss.get<int>(1);
        po->save(src);
        po->save(dst);
    }
    printf("binary edgelist saved to %s\n", out_fnm.c_str());
}
