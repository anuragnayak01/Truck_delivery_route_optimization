#include "algorithm/SampleBellmanFord.h"
#include <limits>
#include <algorithm>

// Constructor implementation
SampleBellmanFord::SampleBellmanFord(SampleNegativeGraph* graph) : graph(graph) {}

// reconstructCycle implementation
std::vector<SampleVertex*> SampleBellmanFord::reconstructCycle(
    SampleVertex* cycleVertex, 
    const std::unordered_map<SampleVertex*, SampleVertex*>& parent) 
{
    std::vector<SampleVertex*> cycle;
    SampleVertex* current = cycleVertex;
    
    // Find a node in the cycle
    std::unordered_set<SampleVertex*> visited;
    for (int i = 0; i < parent.size(); ++i) {
        if (!current || parent.find(current) == parent.end()) return {};
        current = parent.at(current);
        if (visited.count(current)) break;
        visited.insert(current);
    }
    if (!current) return {};
    
    // Reconstruct cycle
    do {
        cycle.push_back(current);
        current = parent.at(current);
        if (cycle.size() > parent.size()) return {};
    } while (current != cycleVertex && current != nullptr);
    
    std::reverse(cycle.begin(), cycle.end());
    return cycle;
}


std::vector<SampleVertex*> SampleBellmanFord::findNegativeCycle() {
    // Get all vertices
    std::vector<SampleVertex*> vertices;
    for (const auto& pair : graph->getAllVertices()) {
        vertices.push_back(pair.second);
    }
    int n = vertices.size();
    
    // Choose an arbitrary source vertex
    SampleVertex* source = vertices[0];
    
    // Initialize distances and parent pointers
    std::unordered_map<SampleVertex*, double> dist;
    std::unordered_map<SampleVertex*, SampleVertex*> parent;
    
    for (SampleVertex* v : vertices) {
        dist[v] = std::numeric_limits<double>::max();
        parent[v] = nullptr;
    }
    dist[source] = 0.0;
    
    // Relax all edges n-1 times
    for (int i = 0; i < n - 1; i++) {
        for (SampleVertex* u : vertices) {
            for (SampleEdge* edge : u->getNeighbors()) {
                SampleVertex* v = edge->getVertexT();
                double weight = edge->getWeight();
                
                if (dist[u] != std::numeric_limits<double>::max() && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }
    
    // Check for negative cycles
    SampleVertex* cycleVertex = nullptr;
    for (SampleVertex* u : vertices) {
        for (SampleEdge* edge : u->getNeighbors()) {
            SampleVertex* v = edge->getVertexT();
            double weight = edge->getWeight();
            
            if (dist[u] != std::numeric_limits<double>::max() && dist[u] + weight < dist[v]) {
                // Found a negative cycle
                cycleVertex = v;
                break;
            }
        }
        if (cycleVertex != nullptr) break;
    }
    
    // If no negative cycle found
    if (cycleVertex == nullptr) {
        return std::vector<SampleVertex*>();
    }
    
    // Find the cycle
    std::unordered_set<SampleVertex*> visited;
    
    // Go back n steps to ensure we're in the cycle
    for (int i = 0; i < n; i++) {
        cycleVertex = parent[cycleVertex];
        if (cycleVertex == nullptr) return std::vector<SampleVertex*>(); // Handle potential null pointer
    }
    
    // Extract the cycle
    std::vector<SampleVertex*> cycle;
    SampleVertex* current = cycleVertex;
    
    do {
        cycle.push_back(current);
        current = parent[current];
        if (current == nullptr) break; // Safety check
    } while (current != cycleVertex && std::find(cycle.begin(), cycle.end(), current) == cycle.end());
    
    // Reverse to get correct order
    std::reverse(cycle.begin(), cycle.end());
    
    return cycle;
}