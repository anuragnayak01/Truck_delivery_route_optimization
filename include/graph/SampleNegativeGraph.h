// SampleNegativeGraph.h
#ifndef SAMPLE_NEGATIVE_GRAPH_H
#define SAMPLE_NEGATIVE_GRAPH_H

#include <unordered_map>
#include <string>
#include "SampleVertex.h"
#include "SampleEdge.h"
class SampleNegativeGraph {
private:
    std::unordered_map<std::string, SampleVertex*> vertices;

public:
    SampleNegativeGraph();
    ~SampleNegativeGraph();
    
    void addVertex(SampleVertex* vertex);
    void addEdge(const std::string& fromName, const std::string& toName, double weight);
    
    const std::unordered_map<std::string, SampleVertex*>& getAllVertices() const { return vertices; }
    SampleVertex* getVertexByName(const std::string& name) const;
};
#endif