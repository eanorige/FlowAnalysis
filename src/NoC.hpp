#pragma once

#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct Edge_key {
    std::string from;
    std::string to;

    Edge_key(const std::string& f, const std::string& t) : from(f), to(t) {}
    bool operator<(const Edge_key& other) const {
        return std::tie(from, to) < std::tie(other.from, other.to);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Edge_key& ek) {
    return os << ek.from << " -> " << ek.to;
}

struct Flow {
    double weight;
    std::vector<Edge_key> path;
};

class NoC {
   public:
    void load_from_json(const std::string& filename);
    std::map<Edge_key, double> calculate_edge_loads() const;
    static void print_edge_loads(const std::map<Edge_key, double>& edge_loads);

   private:
    int width = 0;
    int height = 0;
    std::vector<std::string> nodes;
    // Adjacency list: node -> list of neighbors
    std::map<std::string, std::vector<std::string>> adj;

    // Traffic flows within this NoC
    std::vector<Flow> flows;

    std::string get_node_name(int x, int y) const;
};
