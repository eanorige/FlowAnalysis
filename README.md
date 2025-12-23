# FlowAnalysis

A Network-on-Chip (NoC) flow analysis tool written in C++. This tool calculates edge loads on a 2D mesh network based on specified traffic flows and network topology.

## Features

-   **2D Mesh Topology**: Automatically generates a grid-based network structure.
-   **Broken Link Support**: Ability to specify broken links between routers to simulate faults.
-   **Explicit Flow Paths**: Define traffic flows with specific routing paths.
-   **Load Calculation**: Computes the accumulated weight/load on each link in the network.
-   **JSON Configuration**: Flexible input format using JSON.

## Prerequisites

-   C++17 compatible compiler
-   CMake (version 3.14 or higher)

## Building

1.  Clone the repository:
    ```bash
    git clone https://github.com/eanorige/FlowAnalysis.git
    cd FlowAnalysis
    ```

2.  Configure and build using CMake:
    ```bash
    cmake -B build -S .
    cmake --build build
    ```

## Running

Run the executable with the path to your configuration file:

```bash
./build/FlowAnalysis data/noc_config.json
```

## Configuration Format

The configuration file is a JSON file with the following structure:

```json
{
  "grid": {
    "width": 3,
    "height": 3
  },
  "broken_links": [
    {"node1": "0,0", "node2": "0,1"}
  ],
  "flows": [
    {
      "weight": 10.0,
      "path": ["0,0", "1,0", "1,1", "1,2", "0,2"]
    }
  ]
}
```

-   **grid**: Defines the dimensions of the NoC mesh.
-   **broken_links**: A list of adjacent node pairs that are not connected. Nodes are identified by "x,y" coordinates.
-   **flows**: A list of traffic flows.
    -   **weight**: The load or bandwidth of the flow.
    -   **path**: An ordered list of router coordinates ("x,y") representing the path the flow takes.

## License

[MIT](LICENSE)
