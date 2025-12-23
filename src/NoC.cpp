#include "NoC.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>

using json = nlohmann::json;

std::string NoC::get_node_name(int x, int y) const {
    return std::to_string(x) + "," + std::to_string(y);
}

std::pair<int, int> parse_node_name(const std::string& name) {
    size_t comma_pos = name.find(',');
    int x = std::stoi(name.substr(0, comma_pos));
    int y = std::stoi(name.substr(comma_pos + 1));
    return {x, y};
}

void NoC::load_from_json(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    json data = json::parse(f);

    // Load Grid Dimensions
    width = data["grid"]["width"];
    height = data["grid"]["height"];

    // Generate Nodes
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            nodes.push_back(get_node_name(x, y));
        }
    }

    std::set<Edge_key> broken;
    for (const auto& link : data["broken_links"]) {
        const std::string& u = link["node1"];
        const std::string& v = link["node2"];
        broken.insert(Edge_key(u, v));
        broken.insert(Edge_key(v, u));  // Assuming bidirectional breakage
    }

    // Generate Edges (Mesh Topology)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::string u = get_node_name(x, y);

            // Neighbors: North, South, East, West
            int dx[] = {0, 0, 1, -1};
            int dy[] = {1, -1, 0, 0};

            for (int i = 0; i < 4; ++i) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    std::string v = get_node_name(nx, ny);
                    // Skip broken edge
                    if (broken.find(Edge_key(u, v)) != broken.end()) continue;
                    adj[u].push_back(v);
                }
            }
        }
    }

    // Load Flows
    for (const auto& flow : data["flows"]) {
        Flow f;
        f.weight = flow["weight"];
        std::vector<std::string> path_nodes;
        for (const auto& node : flow["path"]) {
            path_nodes.push_back(node.get<std::string>());
        }
        for (size_t i = 0; i < path_nodes.size() - 1; ++i) {
            f.path.push_back(Edge_key(path_nodes[i], path_nodes[i + 1]));
        }
        flows.push_back(std::move(f));
    }
}

std::map<Edge_key, double> NoC::calculate_edge_loads() const {
    std::map<Edge_key, double> edge_loads;
    for (const auto& flow : flows) {
        if (flow.path.size() < 2) continue;

        // Add flow weight to each edge in the path
        for (auto& e_key : flow.path) {
            auto& [u, v] = e_key;
            auto& nbrs = adj.at(u);
            if (std::find(nbrs.begin(), nbrs.end(), v) == nbrs.end()) {
                std::cerr << "Error: Edge " << e_key
                          << " used in path but not in graph definition (or is broken)."
                          << std::endl;
            }
            edge_loads[e_key] += flow.weight;
        }
    }
    return edge_loads;}

void NoC::print_edge_loads(const std::map<Edge_key, double>& edge_loads) {
    std::cout << "Edge Loads:" << std::endl;
    std::cout << "-----------" << std::endl;
    for (const auto& [e, load] : edge_loads) {
        std::cout << e.from << " -> " << e.to << " : " << load << std::endl;
    }
}
