// SamplePositiveGraph.cpp

#include "graph/SampleEdge.h"
#include "graph/SamplePositiveGraph.h"
SamplePositiveGraph::SamplePositiveGraph() {
}

SamplePositiveGraph::~SamplePositiveGraph() {
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

void SamplePositiveGraph::addVertex(SampleVertex* vertex) {
    vertices[vertex->getName()] = vertex;
}

void SamplePositiveGraph::addEdge(SampleVertex* from, SampleVertex* to, double weight) {
    SampleEdge* edge = new SampleEdge(from, to, weight);
    from->addNeighbor(edge);
    to->addNeighbor(edge); // For undirected graph
}

SampleVertex* SamplePositiveGraph::getVertexByName(const std::string& name) const {
    auto it = vertices.find(name);
    if (it != vertices.end()) {
        return it->second;
    }
    return nullptr;
}