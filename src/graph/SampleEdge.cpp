// SampleEdge.cpp
#include "graph/SampleEdge.h"

SampleEdge::SampleEdge(SampleVertex* vertexF, SampleVertex* vertexT, double weight) {
    this->vertexF = vertexF;
    this->vertexT = vertexT;
    this->weight = weight;
}