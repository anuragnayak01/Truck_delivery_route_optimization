#include "SampleVertex.h"
#ifndef SAMPLE_EDGE_H
#define SAMPLE_EDGE_H
// SampleEdge.h
class SampleEdge {
    private:
        SampleVertex* vertexF;
        SampleVertex* vertexT;
        double weight;
    
    public:
        SampleEdge(SampleVertex* vertexF, SampleVertex* vertexT, double weight);
        
        // Getters
        SampleVertex* getVertexF() const { return vertexF; }
        SampleVertex* getVertexT() const { return vertexT; }
        double getWeight() const { return weight; }
    };
#endif