// include/algorithm/SampleBellmanFord.h
#ifndef SAMPLE_BELLMAN_FORD_H
#define SAMPLE_BELLMAN_FORD_H

#include "graph/SampleNegativeGraph.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class SampleBellmanFord {
private:
    SampleNegativeGraph* graph;
    
    std::vector<SampleVertex*> reconstructCycle(
        SampleVertex* cycleVertex,
        const std::unordered_map<SampleVertex*, SampleVertex*>& parent);

public:
    SampleBellmanFord(SampleNegativeGraph* graph);
    std::vector<SampleVertex*> findNegativeCycle();
};

#endif // SAMPLE_BELLMAN_FORD_H