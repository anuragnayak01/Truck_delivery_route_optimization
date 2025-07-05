// SampleNegativeGraph.cpp

#include "graph/SampleEdge.h"
#include "graph/SampleNegativeGraph.h"
SampleNegativeGraph::SampleNegativeGraph() {
}

SampleNegativeGraph::~SampleNegativeGraph() {
    // Clean up all vertices and edges
    for (auto& pair : vertices) {
        SampleVertex* vertex = pair.second;
        // Clean up all edges for this vertex
        for (SampleEdge* edge : vertex->getNeighbors()) {
            delete edge;
        }
        delete vertex;
    }
}

void SampleNegativeGraph::addVertex(SampleVertex* vertex) {
    vertices[vertex->getName()] = vertex;
}

void SampleNegativeGraph::addEdge(const std::string& fromName, const std::string& toName, double weight) {
    SampleVertex* from = getVertexByName(fromName);
    SampleVertex* to = getVertexByName(toName);
    
    if (from != nullptr && to != nullptr) {
        SampleEdge* edge = new SampleEdge(from, to, weight);
        from->addNeighbor(edge);
    }
}

SampleVertex* SampleNegativeGraph::getVertexByName(const std::string& name) const {
    auto it = vertices.find(name);
    if (it != vertices.end()) {
        return it->second;
    }
    return nullptr;
}
