// SamplePositiveGraph.h
#ifndef SAMPLE_POSITIVE_GRAPH_H
#define SAMPLE_POSITIVE_GRAPH_H

#include <unordered_map>
#include <string>
#include "SampleVertex.h"
#include "SampleEdge.h"

class SamplePositiveGraph {
private:
    std::unordered_map<std::string, SampleVertex*> vertices;

public:
    SamplePositiveGraph();
    ~SamplePositiveGraph();
    
    void addVertex(SampleVertex* vertex);
    void addEdge(SampleVertex* from, SampleVertex* to, double weight);
    
    const std::unordered_map<std::string, SampleVertex*>& getAllVertices() const { return vertices; }
    SampleVertex* getVertexByName(const std::string& name) const;
};
#endif