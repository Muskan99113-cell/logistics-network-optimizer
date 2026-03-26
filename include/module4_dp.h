#pragma once
#include "logistics.h"

class DPModule {
public:
    struct Item {
        int id, weight, value;
        string name;
    };

    static pair<int, vector<int>> knapsack01(vector<Item>& items, int capacity) {
        int n = items.size();
        vector<vector<int>> dp(n+1, vector<int>(capacity+1, 0));
        for (int i = 1; i <= n; i++)
            for (int w = 0; w <= capacity; w++) {
                dp[i][w] = dp[i-1][w];
                if (items[i-1].weight <= w)
                    dp[i][w] = max(dp[i][w], dp[i-1][w-items[i-1].weight]+items[i-1].value);
            }
        vector<int> taken;
        int w = capacity;
        for (int i = n; i >= 1; i--)
            if (dp[i][w] != dp[i-1][w]) { taken.push_back(items[i-1].id); w -= items[i-1].weight; }
        return {dp[n][capacity], taken};
    }

    static pair<int, vector<int>> multiStageDP(vector<vector<int>>& cost, int n) {
        vector<int> dp(n, INT_MAX), nxt(n, -1);
        dp[n-1] = 0;
        for (int i = n-2; i >= 0; i--)
            for (int j = i+1; j < n; j++)
                if (cost[i][j] != INT_MAX && dp[j] != INT_MAX && cost[i][j]+dp[j] < dp[i]) {
                    dp[i] = cost[i][j]+dp[j]; nxt[i] = j;
                }
        vector<int> path;
        for (int i = 0; i != -1; i = nxt[i]) path.push_back(i);
        return {dp[0], path};
    }

    static vector<vector<int>> floydWarshall(vector<vector<int>> dist, int V) {
        for (int k = 0; k < V; k++)
            for (int i = 0; i < V; i++)
                for (int j = 0; j < V; j++)
                    if (dist[i][k] != INT_MAX/2 && dist[k][j] != INT_MAX/2)
                        dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
        return dist;
    }

    vector<PerformanceResult> run() {
        printBanner("MODULE 4: DYNAMIC PROGRAMMING -- Optimal Cost Minimization", Color::BLUE);
        vector<PerformanceResult> results;

        // 0/1 Knapsack
        printSection("0/1 Knapsack -- Optimal Vehicle Loading");
        vector<Item> items = {
            {1,2,6,"Electronics"},{2,2,10,"Pharma"},{3,3,12,"Fresh Food"},
            {4,5,15,"Machinery"},{5,3,8,"Clothing"},{6,4,14,"Electronics-B"},
            {7,2,9,"Documents"},{8,1,4,"Parcels"}
        };
        int capacity = 10;
        long long t0 = timeUs();
        auto [maxVal, taken] = knapsack01(items, capacity);
        long long dt = timeUs() - t0;
        cout << "    Capacity: " << capacity << "kg | Items: " << items.size() << "\n";
        cout << "    Optimal items loaded:\n";
        int usedW = 0;
        for (int id : taken) {
            usedW += items[id-1].weight;
            cout << "      [OK] #" << id << " " << items[id-1].name
                 << " (" << items[id-1].weight << "kg, $" << items[id-1].value << ")\n";
        }
        cout << "    " << Color::GREEN << "Max Value: $" << maxVal << " | Weight: " << usedW << "/" << capacity << "kg" << Color::RESET << "\n";

        // Greedy vs DP
        auto si = items;
        sort(si.begin(), si.end(), [](const Item& a, const Item& b){ return (double)a.value/a.weight > (double)b.value/b.weight; });
        int gcap = capacity, gval = 0;
        for (auto& it : si) if (gcap >= it.weight) { gval += it.value; gcap -= it.weight; }
        cout << "\n    Greedy: $" << Color::YELLOW << gval << Color::RESET << "  |  DP: $" << Color::GREEN << maxVal << Color::RESET;
        if (maxVal > gval) cout << "  <- DP wins by $" << (maxVal-gval) << "!";
        cout << "\n";
        results.push_back({"0/1 Knapsack", dt, (double)maxVal, "O(n x W)"});
        cout << "    Time: " << dt << " us\n\n";

        // Multi-Stage
        printSection("Multi-Stage Delivery Route Planning");
        int stages = 5;
        const int INF = INT_MAX;
        vector<vector<int>> cost(stages, vector<int>(stages, INF));
        cost[0][1]=2; cost[0][2]=3; cost[1][2]=1; cost[1][3]=4;
        cost[2][3]=2; cost[2][4]=5; cost[3][4]=1;
        t0 = timeUs();
        auto [minCost, path] = multiStageDP(cost, stages);
        dt = timeUs() - t0;
        cout << "    Optimal path: ";
        for (int i = 0; i < (int)path.size(); i++) { if(i) cout << " -> "; cout << "Stop" << path[i]; }
        cout << "\n    " << Color::GREEN << "Min Cost: " << minCost << Color::RESET << "\n";
        results.push_back({"Multi-Stage DP", dt, (double)minCost, "O(k x n^2)"});
        cout << "    Time: " << dt << " us\n\n";

        // Floyd-Warshall
        printSection("Floyd-Warshall -- All-Pairs Shortest Path");
        int V = 6; const int H = INT_MAX/2;
        vector<vector<int>> dist(V, vector<int>(V, H));
        for (int i = 0; i < V; i++) dist[i][i] = 0;
        dist[0][1]=4; dist[0][2]=2; dist[1][2]=1; dist[1][3]=5;
        dist[2][4]=10; dist[3][5]=3; dist[4][3]=4; dist[4][5]=1;
        t0 = timeUs();
        auto fw = floydWarshall(dist, V);
        dt = timeUs() - t0;
        cout << "    Distance matrix (" << V << "x" << V << "):\n    ";
        for (int j = 0; j < V; j++) printf("  C%-2d", j);
        cout << "\n";
        for (int i = 0; i < V; i++) {
            printf("  C%d: ", i);
            for (int j = 0; j < V; j++) {
                if (fw[i][j] >= H) printf(" INF");
                else printf("%4d", fw[i][j]);
            }
            cout << "\n";
        }
        results.push_back({"Floyd-Warshall", dt, 0, "O(V^3)"});
        cout << "    Time: " << dt << " us\n\n";

        // Table
        cout << Color::BOLD;
        cout << "    +------------------+-------------+------------+\n";
        cout << "    | Algorithm        | Complexity  | Time (us)  |\n";
        cout << "    +------------------+-------------+------------+\n";
        for (auto& r : results)
            printf("    | %-16s | %-11s | %-10lld |\n",
                   r.algorithm.c_str(), r.complexity.c_str(), r.timeUs);
        cout << "    +------------------+-------------+------------+\n" << Color::RESET;
        cout << "\n  [OK] DP guarantees global optimum that greedy cannot achieve.\n";
        cout << "       Floyd-Warshall enables complete network dispatch planning.\n";

        return results;
    }
};
