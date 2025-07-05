// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include "graph/SampleVertex.h"
#include "graph/SampleEdge.h"
#include "graph/SamplePositiveGraph.h"
#include "graph/SampleNegativeGraph.h"
#include "algorithm/SampleDijkstra.h"
#include "algorithm/SampleBellmanFord.h"



SamplePositiveGraph* loadPositiveGraphFromCSV(const std::string& verticesFile, const std::string& distancesFile);
SamplePositiveGraph* createSamplePositiveGraph();
SampleNegativeGraph* createNegativeGraph(SamplePositiveGraph* positiveGraph);
double calculateEuclideanDistance(SampleVertex* v1, SampleVertex* v2);
SampleVertex* findGarageVertex(SamplePositiveGraph* graph);
double calculateCycleProfit(const std::vector<SampleVertex*>& cycle);
double calculateTotalDistance(SamplePositiveGraph* graph, SampleVertex* garage, const std::vector<SampleVertex*>& cycle);
void runSimplePathAnalysis(SamplePositiveGraph* graph, SampleVertex* garage);

int main() {
    std::cout << "=== Delivery Truck Route Optimization System ===" << std::endl;
    std::cout << "Maximizing Delivery Profit by Combining Bellman-Ford and Dijkstra Algorithms" << std::endl;
    
    try {
        // Step 1: Create the positive graph (for Dijkstra)
        std::cout << "\n1. Constructing Positive Weight Map..." << std::endl;
        SamplePositiveGraph* positiveGraph = loadPositiveGraphFromCSV("data/vertices.csv", "data/distances.csv");
        std::cout << "Positive graph created with " << positiveGraph->getAllVertices().size() << " vertices." << std::endl;
        
        // Step 2: Create the negative graph (for Bellman-Ford)
        std::cout << "\n2. Constructing Negative Weight Map for Profit Analysis..." << std::endl;
        SampleNegativeGraph* negativeGraph = createNegativeGraph(positiveGraph);
        std::cout << "Negative graph created for profit calculations." << std::endl;
        
        // Find the garage vertex (central hub)
        SampleVertex* garage = findGarageVertex(positiveGraph);
        if (garage == nullptr) {
            std::cout << "Error: Garage vertex not found in the graph!" << std::endl;
            delete positiveGraph;
            delete negativeGraph;
            return 1;
        }
        
        // Step 3: Run Bellman-Ford to find negative cycles (profitable routes)
        std::cout << "\n3. Running Bellman-Ford to Find Optimal Delivery Sequence..." << std::endl;
        SampleBellmanFord bellmanFord(negativeGraph);
        std::vector<SampleVertex*> profitableCycle = bellmanFord.findNegativeCycle();
        
        if (profitableCycle.empty()) {
            std::cout << "No profitable delivery cycles found!" << std::endl;
            runSimplePathAnalysis(positiveGraph, garage);
        } else {
            // Print the profitable cycle
            std::cout << "\nFound profitable delivery cycle:" << std::endl;
            std::cout << "Cycle: ";
            for (SampleVertex* vertex : profitableCycle) {
                std::cout << vertex->getName() << " -> ";
            }
            std::cout << profitableCycle[0]->getName() << std::endl;
            
            // Calculate the total profit of the cycle
            double profit = calculateCycleProfit(profitableCycle);
            std::cout << "Total profit for this cycle: $" << (-profit) << std::endl;
            
            // Step 4: Use Dijkstra to find shortest paths between vertices in the profitable cycle
            std::cout << "\n4. Using Dijkstra to Find Shortest Paths Between Delivery Points..." << std::endl;
            SampleDijkstra dijkstra(positiveGraph);
            
            // Execute the profitable cycle and print the detailed path using Dijkstra
            dijkstra.executeNegativeCycleAndPrintPath(garage, profitableCycle);
            
            // Calculate total distance and final profit
            double totalDistance = calculateTotalDistance(positiveGraph, garage, profitableCycle);
            double travelCost = totalDistance * 0.1; // Assuming $0.1 per distance unit
            double finalProfit = (-profit) - travelCost;
            
            std::cout << "\n=== Final Analysis ===" << std::endl;
            std::cout << "Total delivery profit: $" << (-profit) << std::endl;
            std::cout << "Total travel distance: " << totalDistance << " units" << std::endl;
            std::cout << "Travel cost: $" << travelCost << std::endl;
            std::cout << "Final profit after travel costs: $" << finalProfit << std::endl;
        }
        
        // Clean up
        delete positiveGraph;
        delete negativeGraph;
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}

SamplePositiveGraph* loadPositiveGraphFromCSV(const std::string& verticesFile, const std::string& distancesFile) {
    SamplePositiveGraph* graph = new SamplePositiveGraph();
    
    try {
        // First, load all vertices
        std::ifstream verticesStream(verticesFile);
        if (!verticesStream.is_open()) {
            throw std::runtime_error("Unable to open vertices file");
        }
        
        std::string line;
        std::getline(verticesStream, line); // Skip header line
        
        while (std::getline(verticesStream, line)) {
            std::stringstream ss(line);
            std::string name, type, temp;
            double latitude, longitude;
            int mapRow, mapCol;
            
            // Parse CSV line
            std::getline(ss, name, ',');
            std::getline(ss, temp, ',');
            latitude = std::stod(temp);
            std::getline(ss, temp, ',');
            longitude = std::stod(temp);
            std::getline(ss, temp, ',');
            mapRow = std::stoi(temp);
            std::getline(ss, temp, ',');
            mapCol = std::stoi(temp);
            std::getline(ss, type, ',');
            
            SampleVertex* vertex = new SampleVertex(name);
            vertex->setLatitude(latitude);
            vertex->setLongitude(longitude);
            vertex->setMapRow(mapRow);
            vertex->setMapCol(mapCol);
            vertex->setType(type);
            
            graph->addVertex(vertex);
        }
        verticesStream.close();
        
        // Then, load all edges/distances
        std::ifstream distancesStream(distancesFile);
        if (!distancesStream.is_open()) {
            throw std::runtime_error("Unable to open distances file");
        }
        
        std::getline(distancesStream, line); // Skip header line
        
        while (std::getline(distancesStream, line)) {
            std::stringstream ss(line);
            std::string fromName, toName, temp;
            double distance;
            
            // Parse CSV line
            std::getline(ss, fromName, ',');
            std::getline(ss, toName, ',');
            std::getline(ss, temp, ',');
            distance = std::stod(temp);
            
            SampleVertex* from = graph->getVertexByName(fromName);
            SampleVertex* to = graph->getVertexByName(toName);
            
            if (from != nullptr && to != nullptr) {
                graph->addEdge(from, to, distance);
            }
        }
        distancesStream.close();
        
    } catch (const std::exception& e) {
        std::cout << "Error reading CSV files: " << e.what() << std::endl;
        
        // If files not found, create a sample graph for demonstration
        std::cout << "Creating sample graph for demonstration instead..." << std::endl;
        delete graph;
        return createSamplePositiveGraph();
    }
    
    return graph;
}

SamplePositiveGraph* createSamplePositiveGraph() {
    SamplePositiveGraph* graph = new SamplePositiveGraph();
    
    // Add vertices (locations)
    SampleVertex* garage = new SampleVertex("Garage");
    garage->setType("garage");
    garage->setLatitude(40.7128);
    garage->setLongitude(-74.0060);
    
    SampleVertex* locationA = new SampleVertex("LocationA");
    locationA->setType("pickup");
    locationA->setLatitude(40.7300);
    locationA->setLongitude(-74.0100);
    
    SampleVertex* locationB = new SampleVertex("LocationB");
    locationB->setType("pickup");
    locationB->setLatitude(40.7200);
    locationB->setLongitude(-73.9900);
    
    SampleVertex* locationC = new SampleVertex("LocationC");
    locationC->setType("pickup");
    locationC->setLatitude(40.7050);
    locationC->setLongitude(-74.0200);
    
    SampleVertex* locationD = new SampleVertex("LocationD");
    locationD->setType("dropoff");
    locationD->setLatitude(40.7400);
    locationD->setLongitude(-73.9800);
    
    SampleVertex* locationE = new SampleVertex("LocationE");
    locationE->setType("dropoff");
    locationE->setLatitude(40.7150);
    locationE->setLongitude(-73.9700);
    
    graph->addVertex(garage);
    graph->addVertex(locationA);
    graph->addVertex(locationB);
    graph->addVertex(locationC);
    graph->addVertex(locationD);
    graph->addVertex(locationE);
    
    // Add edges with positive weights (distances/costs)
    graph->addEdge(garage, locationA, 5.0);
    graph->addEdge(garage, locationB, 7.0);
    graph->addEdge(garage, locationC, 9.0);
    graph->addEdge(locationA, locationB, 3.0);
    graph->addEdge(locationA, locationD, 6.0);
    graph->addEdge(locationB, locationC, 4.0);
    graph->addEdge(locationB, locationD, 5.0);
    graph->addEdge(locationB, locationE, 8.0);
    graph->addEdge(locationC, locationE, 6.0);
    graph->addEdge(locationD, locationE, 4.0);
    graph->addEdge(locationD, garage, 8.0);
    graph->addEdge(locationE, garage, 7.0);
    
    return graph;
}

SampleNegativeGraph* createNegativeGraph(SamplePositiveGraph* positiveGraph) {
    SampleNegativeGraph* graph = new SampleNegativeGraph();
    const double BASE_PROFIT = 15.0;        // Increased base profit
    const double DISTANCE_PROFIT = 2.0;     // Profit per distance unit
    const double MULTI_PICKUP_BONUS = 3.0;  // Bonus for multiple pickups
    
    // 1. Copy all vertices
    for (const auto& pair : positiveGraph->getAllVertices()) {
        SampleVertex* vertex = pair.second;
        SampleVertex* newVertex = new SampleVertex(vertex->getName());
        newVertex->setType(vertex->getType());
        newVertex->setLatitude(vertex->getLatitude());
        newVertex->setLongitude(vertex->getLongitude());
        newVertex->setMapRow(vertex->getMapRow());
        newVertex->setMapCol(vertex->getMapCol());
        graph->addVertex(newVertex);
    }

    // 2. Create profit edges (pickup to dropoff with negative weights representing profit)
    for (const auto& fromPair : positiveGraph->getAllVertices()) {
        SampleVertex* from = fromPair.second;
        if (from->getType() == "pickup") {
            for (const auto& toPair : positiveGraph->getAllVertices()) {
                SampleVertex* to = toPair.second;
                if (to->getType() == "dropoff") {
                    // Find the shortest path distance in the positive graph
                    SampleDijkstra dijkstra(positiveGraph);
                    dijkstra.runDijkstra(from);
                    double distance = to->getDistance();
                    
                    if (distance > 0) {
                        // Calculate profit - make it negative for Bellman-Ford
                        double profit = BASE_PROFIT + (distance * DISTANCE_PROFIT);
                        graph->addEdge(from->getName(), to->getName(), -profit);
                    }
                }
            }
        }
    }

    // 3. Special constraints to ensure valid delivery routes
    
    // a) Between pickups (with small negative weights to encourage multiple pickups)
    for (const auto& fromPair : graph->getAllVertices()) {
        SampleVertex* from = fromPair.second;
        if (from->getType() == "pickup") {
            for (const auto& toPair : graph->getAllVertices()) {
                SampleVertex* to = toPair.second;
                if (to->getType() == "pickup" && from != to) {
                    // Small negative weight to encourage visiting multiple pickups
                    graph->addEdge(from->getName(), to->getName(), -MULTI_PICKUP_BONUS);
                }
            }
        }
    }
    
    // b) REMOVED direct connections between dropoffs to prevent invalid cycles
    
    // c) From dropoffs back to garage ONLY (no direct dropoff-to-dropoff)
    for (const auto& pair : graph->getAllVertices()) {
        SampleVertex* vertex = pair.second;
        if (vertex->getType() == "dropoff") {
            graph->addEdge(vertex->getName(), "Garage", 0.0);
        }
    }
    
    // d) From garage to pickups ONLY (no direct garage-to-dropoff)
    for (const auto& pair : graph->getAllVertices()) {
        SampleVertex* vertex = pair.second;
        if (vertex->getType() == "pickup") {
            graph->addEdge("Garage", vertex->getName(), 0.0);
        }
    }
    
    // 5. Add artificial high-profit paths for certain routes to guide the algorithm
    // This creates a "suggested" multi-stop route pattern
    
    // Example of a multi-pickup, multi-dropoff route with high profit
    if (graph->getVertexByName("RestaurantA") && graph->getVertexByName("RestaurantB") && 
        graph->getVertexByName("OfficeA") && graph->getVertexByName("OfficeB")) {
        
        // Create a high-profit path from RestaurantA to RestaurantB to OfficeA to OfficeB
        graph->addEdge("RestaurantA", "RestaurantB", -MULTI_PICKUP_BONUS * 2);
        graph->addEdge("RestaurantB", "OfficeA", -BASE_PROFIT * 1.5);
        graph->addEdge("OfficeA", "OfficeB", -MULTI_PICKUP_BONUS * 2);
    }

    // Create another potential profitable route
    if (graph->getVertexByName("RestaurantB") && graph->getVertexByName("RestaurantC") && 
        graph->getVertexByName("OfficeB") && graph->getVertexByName("OfficeC")) {
        
        graph->addEdge("RestaurantB", "RestaurantC", -MULTI_PICKUP_BONUS * 2);
        graph->addEdge("RestaurantC", "OfficeB", -BASE_PROFIT * 1.5);
        graph->addEdge("OfficeB", "OfficeC", -MULTI_PICKUP_BONUS * 2);
    }

    return graph;
}
double calculateEuclideanDistance(SampleVertex* v1, SampleVertex* v2) {
        double lat1 = v1->getLatitude();
        double lon1 = v1->getLongitude();
        double lat2 = v2->getLatitude();
        double lon2 = v2->getLongitude();
        
        // Simple Euclidean distance calculation
        return std::sqrt(std::pow(lat2 - lat1, 2) + std::pow(lon2 - lon1, 2)) * 100;
    }
    
    SampleVertex* findGarageVertex(SamplePositiveGraph* graph) {
        for (const auto& pair : graph->getAllVertices()) {
            SampleVertex* vertex = pair.second;
            if (vertex->getType() == "garage") {
                return vertex;
            }
        }
        
        // If no garage is defined, return the first vertex as default
        if (!graph->getAllVertices().empty()) {
            std::cout << "Warning: No garage vertex found. Using first vertex as default." << std::endl;
            return graph->getAllVertices().begin()->second;
        }
        
        return nullptr;
    }
    
    double calculateCycleProfit(const std::vector<SampleVertex*>& cycle) {
        double totalProfit = 0.0;
        
        for (size_t i = 0; i < cycle.size(); i++) {
            SampleVertex* current = cycle[i];
            SampleVertex* next = cycle[(i + 1) % cycle.size()];
            
            // Find the edge between these vertices and add its weight (negative = profit)
            for (SampleEdge* edge : current->getNeighbors()) {
                if ((edge->getVertexF() == current && edge->getVertexT() == next) ||
                    (edge->getVertexF() == next && edge->getVertexT() == current)) {
                    totalProfit += edge->getWeight();
                    break;
                }
            }
        }
        
        return totalProfit;
    }
    
    double calculateTotalDistance(SamplePositiveGraph* graph, SampleVertex* garage, const std::vector<SampleVertex*>& cycle) {
        double totalDistance = 0.0;
        
        // Create Dijkstra instance
        SampleDijkstra dijkstra(graph);
        
        // Get distance from garage to first location
        SampleVertex* firstLocation = graph->getVertexByName(cycle[0]->getName());
        dijkstra.runDijkstra(garage);
        totalDistance += firstLocation->getDistance();
        
        // Get distances between locations in the cycle
        for (size_t i = 0; i < cycle.size() - 1; i++) {
            SampleVertex* current = graph->getVertexByName(cycle[i]->getName());
            SampleVertex* next = graph->getVertexByName(cycle[i + 1]->getName());
            
            dijkstra.runDijkstra(current);
            totalDistance += next->getDistance();
        }
        
        // Get distance from last location back to garage
        SampleVertex* lastLocation = graph->getVertexByName(cycle[cycle.size() - 1]->getName());
        dijkstra.runDijkstra(lastLocation);
        totalDistance += garage->getDistance();
        
        return totalDistance;
    }
    
    void runSimplePathAnalysis(SamplePositiveGraph* graph, SampleVertex* garage) {
        std::cout << "\nRunning simple path analysis between key locations..." << std::endl;
        
        SampleDijkstra dijkstra(graph);
        
        // Find pickup and dropoff vertices
        std::vector<SampleVertex*> pickups;
        std::vector<SampleVertex*> dropoffs;
        
        for (const auto& pair : graph->getAllVertices()) {
            SampleVertex* vertex = pair.second;
            if (vertex->getType() == "pickup") {
                pickups.push_back(vertex);
            } else if (vertex->getType() == "dropoff") {
                dropoffs.push_back(vertex);
            }
        }
        
        // Analyze paths from garage to each pickup point
        std::cout << "\nDistances from Garage to Pickup points:" << std::endl;
        for (SampleVertex* pickup : pickups) {
            dijkstra.runDijkstra(garage);
            std::cout << "To " << pickup->getName() << ": " << pickup->getDistance() << " units" << std::endl;
        }
        
        // Analyze paths between pickup and dropoff points
        std::cout << "\nDistances from Pickup to Dropoff points:" << std::endl;
        for (SampleVertex* pickup : pickups) {
            for (SampleVertex* dropoff : dropoffs) {
                dijkstra.runDijkstra(pickup);
                std::cout << "From " << pickup->getName() << " to " << dropoff->getName() << ": " 
                         << dropoff->getDistance() << " units" << std::endl;
            }
        }
        
        // Analyze paths from dropoff points back to garage
        std::cout << "\nDistances from Dropoff points back to Garage:" << std::endl;
        for (SampleVertex* dropoff : dropoffs) {
            dijkstra.runDijkstra(dropoff);
            std::cout << "From " << dropoff->getName() << ": " << garage->getDistance() << " units" << std::endl;
        }
    }