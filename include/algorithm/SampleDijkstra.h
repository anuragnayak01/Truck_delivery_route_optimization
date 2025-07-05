// SampleDijkstra.h
#ifndef SAMPLE_DIJKSTRA_H
#define SAMPLE_DIJKSTRA_H

#include <vector>
#include "graph/SamplePositiveGraph.h"

class SampleDijkstra {
private:
    SamplePositiveGraph* positiveGraph;

public:
    SampleDijkstra(SamplePositiveGraph* positiveGraph);
    
    void runDijkstra(SampleVertex* source);
    std::vector<SampleVertex*> getShortestPath(SampleVertex* target);
    void printShortestPath(SampleVertex* source, SampleVertex* target);
    void executeNegativeCycleAndPrintPath(SampleVertex* garage, const std::vector<SampleVertex*>& cycle);
};
#endif