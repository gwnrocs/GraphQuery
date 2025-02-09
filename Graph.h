#pragma once
#ifndef Graph_H
#define Graph_H

#include <unordered_map>
#include <unordered_set>
#include <string>

class Graph {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> adjacencyList;
    std::unordered_set<std::string> nodes;

public:
    Graph();
    void addNode(const std::string& node);
    void addEdge(const std::string& source, const std::string& target);
    bool hasNode(const std::string& node) const;
    bool hasEdge(const std::string& source, const std::string& target) const;
    std::vector<std::string> getNodes() const;
    std::vector<std::pair<std::string, std::string>> getEdges() const;
};

#endif