// SampleVertex.h
#ifndef SAMPLE_VERTEX_H
#define SAMPLE_VERTEX_H

#include <string>
#include <vector>
#include <limits>

class SampleEdge;

class SampleVertex {
private:
    std::string name;
    double latitude;
    double longitude;
    int mapRow;
    int mapCol;
    std::string type; // "normal", "pickup", "dropoff", "garage"
    int status; // 0 = unvisited, 1 = visited
    double distance;
    SampleVertex* parent;
    std::vector<SampleEdge*> neighbors;

public:
    SampleVertex(const std::string& name);
    ~SampleVertex();
    
    void addNeighbor(SampleEdge* edge);
    
    // Getters and setters
    std::string getName() const { return name; }
    double getLatitude() const { return latitude; }
    void setLatitude(double lat) { this->latitude = lat; }
    double getLongitude() const { return longitude; }
    void setLongitude(double lon) { this->longitude = lon; }
    int getMapRow() const { return mapRow; }
    void setMapRow(int row) { this->mapRow = row; }
    int getMapCol() const { return mapCol; }
    void setMapCol(int col) { this->mapCol = col; }
    std::string getType() const { return type; }
    void setType(const std::string& type) { this->type = type; }
    int getStatus() const { return status; }
    void setStatus(int status) { this->status = status; }
    double getDistance() const { return distance; }
    void setDistance(double distance) { this->distance = distance; }
    SampleVertex* getParent() const { return parent; }
    void setParent(SampleVertex* parent) { this->parent = parent; }
    const std::vector<SampleEdge*>& getNeighbors() const { return neighbors; }
    
    // For priority queue comparison
    bool operator>(const SampleVertex& other) const {
        return this->distance > other.distance;
    }
};
#endif