#pragma once
#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include "Graph.h"
#include "Utils.h"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


class MainConsole {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> adjacencyList;
    std::unordered_set<std::string> nodes;

public:
    MainConsole();
    bool parseGraphFromFile(Graph& graph, const std::string& filename);
    void handleQueries(Graph& graph);
    void drawConsole();
};

#endif