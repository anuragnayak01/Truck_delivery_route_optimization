// SampleDijkstra.cpp
#include "algorithm/SampleDijkstra.h"
#include "graph/SampleVertex.h"
#include "graph/SampleEdge.h"
#include "graph/SamplePositiveGraph.h"
#include "graph/SampleNegativeGraph.h"
#include <queue>
#include <algorithm>
#include <iostream>

// Custom comparator for priority queue
struct VertexComparator {
    bool operator()(SampleVertex* a, SampleVertex* b) {
        return a->getDistance() > b->getDistance();
    }
};

SampleDijkstra::SampleDijkstra(SamplePositiveGraph* positiveGraph) {
    this->positiveGraph = positiveGraph;
}

void SampleDijkstra::runDijkstra(SampleVertex* source) {
    // Create a priority queue of vertices
    std::priority_queue<SampleVertex*, std::vector<SampleVertex*>, VertexComparator> priorityQueue;
    
    // Initialize distances and status for all vertices in the positive graph
    for (const auto& pair : positiveGraph->getAllVertices()) {
        SampleVertex* vertex = pair.second;
        vertex->setDistance(std::numeric_limits<double>::max()); // Set initial distance to infinity
        vertex->setStatus(0); // Set vertex status to unvisited (0)
        vertex->setParent(nullptr); // No parent initially
    }
    
    source->setDistance(0); // Set distance for the source vertex to 0
    priorityQueue.push(source); // Add the source vertex to the priority queue
    
    while (!priorityQueue.empty()) {
        SampleVertex* u = priorityQueue.top(); // Get the vertex with the smallest distance
        priorityQueue.pop();
        u->setStatus(1); // Mark the vertex as visited (1)
        
        for (SampleEdge* edge : u->getNeighbors()) {
            SampleVertex* v;
            
            // Determine the target vertex
            if (edge->getVertexT() == u) {
                v = edge->getVertexF();
            } else {
                v = edge->getVertexT();
            }
            
            if (v->getStatus() == 1) continue; // Skip already visited vertices
            
            double newDist = u->getDistance() + edge->getWeight(); // Calculate the new distance
            if (newDist < v->getDistance()) {
                v->setDistance(newDist); // Update the distance
                v->setParent(u); // Set the parent to the current vertex
                priorityQueue.push(v); // Add the vertex to the queue with the updated distance
            }
        }
    }
}

std::vector<SampleVertex*> SampleDijkstra::getShortestPath(SampleVertex* target) {
    std::vector<SampleVertex*> path;
    
    // Check if target is reachable
    if (target->getDistance() == std::numeric_limits<double>::max()) {
        return path; // Return empty path if target is unreachable
    }
    
    for (SampleVertex* at = target; at != nullptr; at = at->getParent()) {
        path.push_back(at); // Add each vertex to the path list
    }
    std::reverse(path.begin(), path.end()); // Reverse the list to get the path from source to target
    return path;
}

void SampleDijkstra::printShortestPath(SampleVertex* source, SampleVertex* target) {
    runDijkstra(source); // Run Dijkstra from the source to calculate all shortest paths
    std::vector<SampleVertex*> path = getShortestPath(target); // Get the shortest path to the target
    
    if (path.empty() || path[0] != source) {
        std::cout << "No path from " << source->getName() << " to " << target->getName() << std::endl;
    } else {
        std::cout << "Shortest path from " << source->getName() << " to " << target->getName() << ": ";
        
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i]->getName();
            if (i < path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
        
        // Calculate and print the total distance
        double totalDistance = target->getDistance();
        std::cout << "Total distance: " << totalDistance << std::endl;
    }
}

void SampleDijkstra::executeNegativeCycleAndPrintPath(SampleVertex* garage, const std::vector<SampleVertex*>& cycle) {
    std::cout << "Shortest path:" << std::endl;
    
    // Find the corresponding vertices in the positive graph
    std::vector<SampleVertex*> positiveCycle;
    for (SampleVertex* vertex : cycle) {
        positiveCycle.push_back(positiveGraph->getVertexByName(vertex->getName()));
    }
    
    double totalDistance = 0;
    
    // Iterate over the cycle in the positive graph and print the shortest paths
    for (size_t i = 0; i < positiveCycle.size(); i++) {
        if (i == 0) { // from the garage to the first pickup vertex
            runDijkstra(garage);
            printShortestPath(garage, positiveCycle[i]);
            totalDistance += positiveCycle[i]->getDistance();
        }
        
        if (i == positiveCycle.size() - 1) { // back to the garage
            runDijkstra(positiveCycle[i]);
            printShortestPath(positiveCycle[i], garage);
            totalDistance += garage->getDistance();
            continue;
        }
        
        // Run Dijkstra again from the new current vertex to find the next shortest path
        runDijkstra(positiveCycle[i]);
        printShortestPath(positiveCycle[i], positiveCycle[i + 1]);
        totalDistance += positiveCycle[i + 1]->getDistance();
    }
    
    std::cout << "Total route distance: " << totalDistance << std::endl;
}