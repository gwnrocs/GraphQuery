#include "Graph.h"

Graph::Graph() {}

// Add a node to the graph
void Graph::addNode(const std::string& node) {
    if (!nodes.count(node)) {
        nodes.insert(node);
        adjacencyList[node] = {};
    }
}

// Add an edge to the graph
void Graph::addEdge(const std::string& source, const std::string& target) {
    addNode(source);
    addNode(target);
    adjacencyList[source].insert(target);
}

// Check if a node exists
bool Graph::hasNode(const std::string& node) const {
    return nodes.count(node) > 0;
}

// Check if an edge exists
bool Graph::hasEdge(const std::string& source, const std::string& target) const {
    if (adjacencyList.count(source)) {
        return adjacencyList.at(source).count(target) > 0;
    }
    return false;
}

// Get all nodes
std::vector<std::string> Graph::getNodes() const {
    return std::vector<std::string>(nodes.begin(), nodes.end());
}

// Get all edges
std::vector<std::pair<std::string, std::string>> Graph::getEdges() const {
    std::vector<std::pair<std::string, std::string>> edges;
    for (const std::pair<const std::string, std::unordered_set<std::string>>& pair : adjacencyList) {
        const std::string& source = pair.first;
        const std::unordered_set<std::string>& targets = pair.second;
        for (const std::string& target : targets) {
            edges.emplace_back(source, target);
        }
    }
    return edges;
}