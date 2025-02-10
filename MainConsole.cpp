#include "MainConsole.h"

MainConsole::MainConsole() {}

bool MainConsole::parseGraphFromFile(Graph& graph, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "> " << Colors::Red << "Error: " << filename << " not found\n\n" << Colors::Reset;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char type;
        iss >> type;
        if (type == '*') {
            std::string node;
            iss >> node;
            graph.addNode(node);
        }
        else if (type == '-') {
            std::string source, target;
            iss >> source >> target;
            graph.addEdge(source, target);
        }
    }

    file.close();
    return true;
}


void MainConsole::handleQueries(Graph& graph) {
    std::string query;
    while (true) {
        std::cout << "> Graph Query: ";
        std::getline(std::cin, query);
        std::istringstream iss(query);
        std::string command;
        iss >> command;

        if (command == "nodes") {
            std::vector<std::string> nodes = graph.getNodes();
            std::cout << "> Nodes:";
            for (const std::string& node : nodes) {
                std::cout << " " << node;
            }
            std::cout << "\n\n";
        }
        else if (command == "node") {
            std::string node;
            iss >> node;
            if (graph.hasNode(node)) {
                std::cout << "> " << Colors::Green << "Node " << node << " exists in the graph.\n\n" << Colors::Reset;
            }
            else {
                std::cout << "> " << Colors::Red << "Node " << node << " does not exist in the graph.\n\n" << Colors::Reset;
            }
        }
        else if (command == "edges") {
            std::vector<std::pair<std::string, std::string>> edges = graph.getEdges();
            std::cout << "> Edges:";
            for (const std::pair<std::string, std::string>& pair : edges) {
                const std::string& source = pair.first;
                const std::string& target = pair.second;
                std::cout << " (" << source << ", " << target << ")";
            }
            std::cout << "\n\n";
        }
        else if (command == "edge") {
            std::string source, target;
            iss >> source >> target;
            if (graph.hasEdge(source, target)) {
                std::cout << "> " << Colors::Green << "Edge(" << source << ", " << target << ") exists in the graph.\n\n" << Colors::Reset;
            }
            else {
                std::cout << "> " << Colors::Red << "Edge(" << source << ", " << target << ") does not exist in the graph.\n\n" << Colors::Reset;
            }
        }
        else if (command == "path") {
            std::string source, target;
            iss >> source >> target;
            std::string result = graph.hasPath(source, target);
            std::cout << "> " << result << "\n\n";
        }

        else if (command == "exit") {
            std::cout << "> " << Colors::Yellow << "Exiting . . . \n\n\n" << Colors::Reset;
            system("wmic process where \"name='python.exe' and commandline like '%%GraphVisualizer.py%%'\" call terminate");

            break;
        }
        else {
            std::cout << "> " << Colors::Red << "Error: Invalid query.\n\n" << Colors::Reset;
        }
    }
}


void MainConsole::drawConsole() {
    Graph* graph = new Graph();
    std::string filename;
    bool graphParsed = false;

    Utils::printAscii();

    while (!graphParsed) {
        std::cout << "> Graph File: ";
        std::getline(std::cin, filename);

        if (parseGraphFromFile(*graph, filename)) {
            std::cout << "> " << Colors::Green << "Graph File: " << filename << " has been loaded.\n\n" << Colors::Reset;

            // Run Graph Visualizer Python script in background
#ifdef _WIN32
            std::string command = "start /B python GraphVisualizer.py " + filename;
#else
            std::string command = "python GraphVisualizer.py " + filename + " &";
#endif

            int result = system(command.c_str());
            if (result != 0) {
                std::cerr << "> " << Colors::Red << "Error: Failed to run visualization script\n" << Colors::Reset;
                std::cerr << "> " << Colors::Yellow << "Please ensure Python and required libraries are installed\n\n" << Colors::Reset;
            }

            graphParsed = true;
            handleQueries(*graph);
        }
    }
}