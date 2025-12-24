import json
import random
import collections
import argparse

def get_node_name(x, y):
    return f"{x},{y}"

def generate_noc(width, height, num_flows, num_broken_links, seed=None):
    if seed is not None:
        random.seed(seed)

    adj = collections.defaultdict(list)
    edges = []

    # Build full grid
    for y in range(height):
        for x in range(width):
            u = get_node_name(x, y)
            # East
            if x + 1 < width:
                v = get_node_name(x + 1, y)
                edges.append((u, v))
            # South
            if y + 1 < height:
                v = get_node_name(x, y + 1)
                edges.append((u, v))

    # Select broken links
    broken_links_data = []
    if num_broken_links > 0 and len(edges) > 0:
        random.shuffle(edges)
        broken_edges = set(edges[:num_broken_links])
        for u, v in broken_edges:
            broken_links_data.append({"node1": u, "node2": v})
    else:
        broken_edges = set()

    # Build adjacency list excluding broken links
    # Re-iterate to build adj correctly (undirected)
    # The edges list above only had one direction for generation purposes
    # But we need to check if (u,v) or (v,u) is in broken_edges
    
    # Helper to check if edge is broken
    def is_broken(u, v):
        return (u, v) in broken_edges or (v, u) in broken_edges

    for y in range(height):
        for x in range(width):
            u = get_node_name(x, y)
            neighbors = []
            # Check 4 directions
            for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
                nx, ny = x + dx, y + dy
                if 0 <= nx < width and 0 <= ny < height:
                    v = get_node_name(nx, ny)
                    if not is_broken(u, v):
                        neighbors.append(v)
            adj[u] = neighbors

    # BFS for shortest path
    def bfs(start, end):
        queue = collections.deque([[start]])
        visited = {start}
        while queue:
            path = queue.popleft()
            node = path[-1]
            if node == end:
                return path
            for neighbor in adj[node]:
                if neighbor not in visited:
                    visited.add(neighbor)
                    new_path = list(path)
                    new_path.append(neighbor)
                    queue.append(new_path)
        return None

    nodes = list(adj.keys())
    flows = []
    
    count = 0
    while count < num_flows:
        u = random.choice(nodes)
        v = random.choice(nodes)
        if u == v:
            continue
            
        path = bfs(u, v)
        if path:
            weight = round(random.uniform(1.0, 100.0), 2)
            flows.append({
                "weight": weight,
                "path": path
            })
            count += 1

    data = {
        "grid": {
            "width": width,
            "height": height
        },
        "broken_links": broken_links_data,
        "flows": flows
    }
    
    return data

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate NoC configuration")
    parser.add_argument("--width", type=int, default=10, help="Grid width")
    parser.add_argument("--height", type=int, default=10, help="Grid height")
    parser.add_argument("--flows", type=int, default=1000, help="Number of flows")
    parser.add_argument("--broken", type=int, default=50, help="Number of broken links")
    parser.add_argument("--seed", type=int, help="Random seed")
    parser.add_argument("--output", type=str, default="data/large_noc_config.json", help="Output file path")
    
    args = parser.parse_args()

    data = generate_noc(args.width, args.height, args.flows, args.broken, args.seed)
    with open(args.output, "w") as f:
        json.dump(data, f, indent=2)
    print(f"Generated {args.output}")
