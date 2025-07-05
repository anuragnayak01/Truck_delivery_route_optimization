#include "graph/SampleVertex.h"
#include "graph/SampleEdge.h"

SampleVertex::SampleVertex(const std::string& name) {
    this->name = name;
    this->distance = std::numeric_limits<double>::max();
    this->status = 0;
    this->parent = nullptr;
    this->type = "normal";
}

SampleVertex::~SampleVertex() {
    // The edges will be deleted by the graph
}

void SampleVertex::addNeighbor(SampleEdge* edge) {
    neighbors.push_back(edge);
}
