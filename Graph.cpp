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
// Sequential Search
//bool Graph::hasNode(const std::string& node) const {
//    return nodes.count(node) > 0;
//}

// Parallel Search
bool Graph::hasNode(const std::string& node) const {
    std::atomic<bool> found(false);
    std::mutex mtx;
    std::vector<std::thread> threads;
    size_t numThreads = std::max(1u, std::min(std::thread::hardware_concurrency() / 2, static_cast<unsigned int>(nodes.size() / 2)));
    size_t chunkSize = nodes.size() / numThreads;

    auto it = nodes.begin();
    auto searchTask = [&](auto begin, auto end, int threadID) {
        for (auto iter = begin; iter != end && !found.load(); ++iter) {
            //std::cout << "Thread " << threadID << " searching node: " << *iter << std::endl;
            if (*iter == node) {
                found.store(true);
                break;
            }
        }
        };

    for (size_t i = 0; i < numThreads; ++i) {
        auto end = (i == numThreads - 1) ? nodes.end() : std::next(it, chunkSize);
        threads.emplace_back(searchTask, it, end, i);
        it = end;
    }

    for (auto& t : threads) {
        t.join();
    }

    return found.load();
}



// Check if an edge exists
// Sequential Search
//bool Graph::hasEdge(const std::string& source, const std::string& target) const {
//    if (adjacencyList.count(source)) {
//        return adjacencyList.at(source).count(target) > 0;
//    }
//    return false;
//}

// Parallel Search
bool Graph::hasEdge(const std::string& source, const std::string& target) const {
    if (!adjacencyList.count(source)) {
        return false;
    }

    std::atomic<bool> found(false);
    std::mutex mtx;
    std::vector<std::thread> threads;
    size_t numEdges = adjacencyList.at(source).size();
    size_t numThreads = std::max(1u, std::min(std::thread::hardware_concurrency() / 2, static_cast<unsigned int>(numEdges / 2)));
    size_t chunkSize = numEdges / numThreads;

    auto it = adjacencyList.at(source).begin();
    auto searchTask = [&](auto begin, auto end, int threadID) {
        for (auto iter = begin; iter != end && !found.load(); ++iter) {
            //std::cout << "Thread " << threadID << " searching edge: " << *iter << std::endl;
            if (*iter == target) {
                found.store(true);
                break;
            }
        }
        };

    for (size_t i = 0; i < numThreads; ++i) {
        auto end = (i == numThreads - 1) ? adjacencyList.at(source).end() : std::next(it, chunkSize);
        threads.emplace_back(searchTask, it, end, i);
        it = end;
    }

    for (auto& t : threads) {
        t.join();
    }

    return found.load();
}

std::string Graph::hasPath(const std::string& source, const std::string& target) const {
    if (!hasNode(source) || !hasNode(target)) {
        return "No path from " + source + " to " + target;
    }

    std::unordered_map<std::string, std::string> parent;
    std::queue<std::string> q;
    q.push(source);
    parent[source] = "";

    while (!q.empty()) {
        std::string node = q.front();
        q.pop();

        if (node == target) {
            std::vector<std::string> path;
            for (std::string at = target; !at.empty(); at = parent[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            std::string result = "path: ";
            for (size_t i = 0; i < path.size(); ++i) {
                result += path[i] + (i < path.size() - 1 ? " -> " : "");
            }
            return result;
        }

        for (const auto& neighbor : adjacencyList.at(node)) {
            if (!parent.count(neighbor)) {
                parent[neighbor] = node;
                q.push(neighbor);
            }
        }
    }

    return "No path from " + source + " to " + target;
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