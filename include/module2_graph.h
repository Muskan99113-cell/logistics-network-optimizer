#pragma once
#include "logistics.h"

// ═══════════════════════════════════════════════════════════════════════════
//  MODULE 2: GRAPH ALGORITHMS — Route Optimization Core
//  Algorithms: Dijkstra, Bellman-Ford, Prim MST, Kruskal MST, BFS, DFS
// ═══════════════════════════════════════════════════════════════════════════

class GraphModule {
public:
    int V;
    vector<vector<Edge>> adj;
    vector<string> cityNames;

    GraphModule(int v) : V(v), adj(v) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void setCityNames(vector<string> names) { cityNames = names; }

    string cityName(int i) {
        return (i < (int)cityNames.size()) ? cityNames[i] : "City" + to_string(i);
    }

    // ── Dijkstra ──────────────────────────────────────────────────────────
    pair<vector<int>, vector<int>> dijkstra(int src) {
        vector<int> dist(V, INT_MAX), prev(V, -1);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        dist[src] = 0;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto& e : adj[u]) {
                if (dist[u] + e.weight < dist[e.to]) {
                    dist[e.to] = dist[u] + e.weight;
                    prev[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        return {dist, prev};
    }

    vector<int> getPath(vector<int>& prev, int dst) {
        vector<int> path;
        for (int v = dst; v != -1; v = prev[v]) path.push_back(v);
        reverse(path.begin(), path.end());
        return path;
    }

    // ── Bellman-Ford (handles dynamic traffic penalties / negative weights) ─
    pair<vector<int>, bool> bellmanFord(int src) {
        vector<int> dist(V, INT_MAX);
        dist[src] = 0;
        bool negCycle = false;
        for (int i = 0; i < V-1; i++) {
            for (int u = 0; u < V; u++) {
                if (dist[u] == INT_MAX) continue;
                for (auto& e : adj[u]) {
                    if (dist[u] + e.weight < dist[e.to])
                        dist[e.to] = dist[u] + e.weight;
                }
            }
        }
        // Check negative cycle
        for (int u = 0; u < V; u++) {
            if (dist[u] == INT_MAX) continue;
            for (auto& e : adj[u])
                if (dist[u] + e.weight < dist[e.to]) { negCycle = true; break; }
        }
        return {dist, negCycle};
    }

    // ── Prim's MST ────────────────────────────────────────────────────────
    pair<int, vector<pair<int,int>>> primMST() {
        vector<int> key(V, INT_MAX), parent(V, -1);
        vector<bool> inMST(V, false);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        key[0] = 0;
        pq.push({0, 0});
        int totalCost = 0;
        vector<pair<int,int>> mstEdges;
        while (!pq.empty()) {
            auto [k, u] = pq.top(); pq.pop();
            if (inMST[u]) continue;
            inMST[u] = true;
            totalCost += k;
            if (parent[u] != -1) mstEdges.push_back({parent[u], u});
            for (auto& e : adj[u]) {
                if (!inMST[e.to] && e.weight < key[e.to]) {
                    key[e.to] = e.weight;
                    parent[e.to] = u;
                    pq.push({key[e.to], e.to});
                }
            }
        }
        return {totalCost, mstEdges};
    }

    // ── Kruskal's MST (Union-Find) ─────────────────────────────────────────
    struct DSU {
        vector<int> parent, rank_;
        DSU(int n) : parent(n), rank_(n, 0) { iota(parent.begin(), parent.end(), 0); }
        int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
        bool unite(int a, int b) {
            a = find(a); b = find(b);
            if (a == b) return false;
            if (rank_[a] < rank_[b]) swap(a, b);
            parent[b] = a;
            if (rank_[a] == rank_[b]) rank_[a]++;
            return true;
        }
    };

    pair<int, vector<GraphEdge>> kruskalMST() {
        vector<GraphEdge> edges;
        for (int u = 0; u < V; u++)
            for (auto& e : adj[u])
                if (u < e.to) edges.push_back({u, e.to, e.weight});
        sort(edges.begin(), edges.end());
        DSU dsu(V);
        int totalCost = 0;
        vector<GraphEdge> mstEdges;
        for (auto& e : edges) {
            if (dsu.unite(e.u, e.v)) {
                totalCost += e.w;
                mstEdges.push_back(e);
            }
        }
        return {totalCost, mstEdges};
    }

    // ── BFS: Connectivity & Reachability ─────────────────────────────────
    vector<int> bfs(int src) {
        vector<bool> visited(V, false);
        vector<int> order;
        queue<int> q;
        visited[src] = true;
        q.push(src);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (auto& e : adj[u])
                if (!visited[e.to]) { visited[e.to] = true; q.push(e.to); }
        }
        return order;
    }

    // ── DFS: Zone discovery ───────────────────────────────────────────────
    void dfsHelper(int u, vector<bool>& vis, vector<int>& order) {
        vis[u] = true; order.push_back(u);
        for (auto& e : adj[u]) if (!vis[e.to]) dfsHelper(e.to, vis, order);
    }

    vector<int> dfs(int src) {
        vector<bool> vis(V, false);
        vector<int> order;
        dfsHelper(src, vis, order);
        return order;
    }

    // ── Floyd-Warshall (All-Pairs Shortest Path) ───────────────────────────
    vector<vector<int>> floydWarshall() {
        vector<vector<int>> dist(V, vector<int>(V, INT_MAX/2));
        for (int i = 0; i < V; i++) dist[i][i] = 0;
        for (int u = 0; u < V; u++)
            for (auto& e : adj[u]) dist[u][e.to] = min(dist[u][e.to], e.weight);
        for (int k = 0; k < V; k++)
            for (int i = 0; i < V; i++)
                for (int j = 0; j < V; j++)
                    if (dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];
        return dist;
    }

    // ── Run Demo ──────────────────────────────────────────────────────────
    vector<PerformanceResult> run(int warehouse = 0) {
        printBanner("MODULE 2: GRAPH ALGORITHMS — Route Optimization Core", Color::GREEN);

        vector<PerformanceResult> results;

        // ── Dijkstra ──
        printSection("Dijkstra's Shortest Path from Warehouse");
        long long t0 = timeUs();
        auto [dDist, dPrev] = dijkstra(warehouse);
        long long dt = timeUs() - t0;
        cout << "    From: " << Color::CYAN << cityName(warehouse) << Color::RESET << "\n";
        for (int i = 1; i < min(V, 8); i++) {
            auto path = getPath(dPrev, i);
            cout << "    → " << cityName(i) << ": " << Color::GREEN << dDist[i] << " km" << Color::RESET << "  [";
            for (int j = 0; j < (int)path.size(); j++) {
                if (j) cout << "→";
                cout << cityName(path[j]);
            }
            cout << "]\n";
        }
        results.push_back({"Dijkstra", dt, 0, "O((V+E) log V)"});
        cout << "    ⏱  Time: " << dt << " µs\n\n";

        // ── Bellman-Ford ──
        printSection("Bellman-Ford (handles dynamic traffic penalties)");
        t0 = timeUs();
        auto [bfDist, negCycle] = bellmanFord(warehouse);
        dt = timeUs() - t0;
        cout << "    Negative cycle detected: " << (negCycle ? "YES (rerouting needed)" : "NO (safe)") << "\n";
        cout << "    Sample distances: ";
        for (int i = 1; i < min(V, 5); i++) cout << cityName(i) << "=" << bfDist[i] << "km  ";
        cout << "\n";
        results.push_back({"Bellman-Ford", dt, 0, "O(V*E)"});
        cout << "    ⏱  Time: " << dt << " µs  ← " << Color::YELLOW << "slower, but handles penalties\n" << Color::RESET << "\n";

        // ── Prim MST ──
        printSection("Prim's MST — Minimum Infrastructure Cost");
        t0 = timeUs();
        auto [primCost, primEdges] = primMST();
        dt = timeUs() - t0;
        cout << "    MST Total Cost: " << Color::GREEN << primCost << " km" << Color::RESET << "\n";
        cout << "    MST Edges (first 5): ";
        for (int i = 0; i < min(5,(int)primEdges.size()); i++)
            cout << cityName(primEdges[i].first) << "-" << cityName(primEdges[i].second) << "  ";
        cout << "\n";
        results.push_back({"Prim MST", dt, (double)primCost, "O(E log V)"});
        cout << "    ⏱  Time: " << dt << " µs\n\n";

        // ── Kruskal MST ──
        printSection("Kruskal's MST — Union-Find approach");
        t0 = timeUs();
        auto [kCost, kEdges] = kruskalMST();
        dt = timeUs() - t0;
        cout << "    MST Total Cost: " << Color::GREEN << kCost << " km" << Color::RESET << "\n";
        results.push_back({"Kruskal MST", dt, (double)kCost, "O(E log E)"});
        cout << "    ⏱  Time: " << dt << " µs\n\n";

        // ── BFS/DFS ──
        printSection("BFS — Connectivity & Reachability Check");
        t0 = timeUs();
        auto bfsOrder = bfs(warehouse);
        dt = timeUs() - t0;
        cout << "    Reachable cities: " << bfsOrder.size() << "/" << V << "\n";
        cout << "    BFS order (first 8): ";
        for (int i = 0; i < min(8,(int)bfsOrder.size()); i++) cout << cityName(bfsOrder[i]) << " ";
        cout << "\n";
        results.push_back({"BFS", dt, (double)bfsOrder.size(), "O(V+E)"});
        cout << "    ⏱  Time: " << dt << " µs\n\n";

        // ── Complexity Table ──
        cout << Color::BOLD;
        cout << "    ┌───────────────┬───────────────┬──────────────┐\n";
        cout << "    │ Algorithm     │ Complexity    │ Time (µs)    │\n";
        cout << "    ├───────────────┼───────────────┼──────────────┤\n";
        for (auto& r : results)
            printf("    │ %-13s │ %-13s │ %-12lld │\n",
                   r.algorithm.c_str(), r.complexity.c_str(), r.timeUs);
        cout << "    └───────────────┴───────────────┴──────────────┘\n" << Color::RESET;

        cout << "\n  ✅ Dijkstra is best for static road networks.\n";
        cout << "     Bellman-Ford handles real-time traffic penalty adjustments.\n";
        cout << "     MST minimizes total infrastructure / backbone route cost.\n";

        return results;
    }
};
