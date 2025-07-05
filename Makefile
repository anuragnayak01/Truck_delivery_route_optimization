# Makefile for Delivery Truck Route Optimization System
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -I include

# Source directories
SRC_DIR = src
GRAPH_DIR = $(SRC_DIR)/graph
ALGO_DIR = $(SRC_DIR)/algorithm

# Object files
OBJS = $(SRC_DIR)/main.o \
       $(GRAPH_DIR)/SampleVertex.o \
       $(GRAPH_DIR)/SampleEdge.o \
       $(GRAPH_DIR)/SamplePositiveGraph.o \
       $(GRAPH_DIR)/SampleNegativeGraph.o \
       $(ALGO_DIR)/SampleDijkstra.o \
       $(ALGO_DIR)/SampleBellmanFord.o

# Main target
all: directories delivery_optimizer

directories:
	mkdir -p $(SRC_DIR) $(GRAPH_DIR) $(ALGO_DIR) include/graph include/algorithm data

delivery_optimizer: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Object file dependencies
$(SRC_DIR)/main.o: $(SRC_DIR)/main.cpp include/graph/SamplePositiveGraph.h include/graph/SampleNegativeGraph.h include/algorithm/SampleDijkstra.h include/algorithm/SampleBellmanFord.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GRAPH_DIR)/SampleVertex.o: $(GRAPH_DIR)/SampleVertex.cpp include/graph/SampleVertex.h include/graph/SampleEdge.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GRAPH_DIR)/SampleEdge.o: $(GRAPH_DIR)/SampleEdge.cpp include/graph/SampleEdge.h include/graph/SampleVertex.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GRAPH_DIR)/SamplePositiveGraph.o: $(GRAPH_DIR)/SamplePositiveGraph.cpp include/graph/SamplePositiveGraph.h include/graph/SampleVertex.h include/graph/SampleEdge.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GRAPH_DIR)/SampleNegativeGraph.o: $(GRAPH_DIR)/SampleNegativeGraph.cpp include/graph/SampleNegativeGraph.h include/graph/SampleVertex.h include/graph/SampleEdge.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ALGO_DIR)/SampleDijkstra.o: $(ALGO_DIR)/SampleDijkstra.cpp include/algorithm/SampleDijkstra.h include/graph/SamplePositiveGraph.h include/graph/SampleVertex.h include/graph/SampleEdge.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ALGO_DIR)/SampleBellmanFord.o: $(ALGO_DIR)/SampleBellmanFord.cpp include/algorithm/SampleBellmanFord.h include/graph/SampleNegativeGraph.h include/graph/SampleVertex.h include/graph/SampleEdge.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Data directory already part of directories target

# Clean up
clean:
	rm -f $(SRC_DIR)/*.o $(GRAPH_DIR)/*.o $(ALGO_DIR)/*.o delivery_optimizer

.PHONY: all clean directories