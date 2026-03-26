#pragma once
#include "logistics.h"

// ═══════════════════════════════════════════════════════════════════════════
//  MODULE 3: GREEDY ALGORITHMS — Vehicle Assignment
//  Algorithms: Activity Selection, Fractional Knapsack, Job Scheduling
//  Shows WHERE greedy fails (0/1 knapsack comparison)
// ═══════════════════════════════════════════════════════════════════════════

class GreedyModule {
public:

    // ── Activity Selection: Max deliveries per vehicle ─────────────────────
    // Sort by finish time, greedily pick non-overlapping deliveries
    struct Delivery {
        int id, start, finish, priority;
        double value;
    };

    static vector<int> activitySelection(vector<Delivery> deliveries) {
        sort(deliveries.begin(), deliveries.end(),
             [](const Delivery& a, const Delivery& b){ return a.finish < b.finish; });
        vector<int> selected;
        int lastFinish = -1;
        for (auto& d : deliveries) {
            if (d.start >= lastFinish) {
                selected.push_back(d.id);
                lastFinish = d.finish;
            }
        }
        return selected;
    }

    // ── Fractional Knapsack: Maximize value per trip ──────────────────────
    struct Item {
        int id;
        double weight, value;
        double ratio() const { return value / weight; }
    };

    static pair<double, vector<pair<int,double>>> fractionalKnapsack(
            vector<Item> items, double capacity) {
        sort(items.begin(), items.end(),
             [](const Item& a, const Item& b){ return a.ratio() > b.ratio(); });
        double totalValue = 0;
        vector<pair<int,double>> taken; // {id, fraction}
        for (auto& item : items) {
            if (capacity <= 0) break;
            double take = min(item.weight, capacity);
            taken.push_back({item.id, take / item.weight});
            totalValue += take * item.ratio();
            capacity -= take;
        }
        return {totalValue, taken};
    }

    // ── Job Scheduling with Deadlines ────────────────────────────────────
    struct Job {
        int id, deadline, profit;
    };

    static pair<int, vector<int>> jobScheduling(vector<Job> jobs) {
        sort(jobs.begin(), jobs.end(),
             [](const Job& a, const Job& b){ return a.profit > b.profit; });
        int maxDeadline = 0;
        for (auto& j : jobs) maxDeadline = max(maxDeadline, j.deadline);
        vector<int> slot(maxDeadline + 1, -1);
        vector<int> result;
        int totalProfit = 0;
        for (auto& j : jobs) {
            for (int t = min(j.deadline, maxDeadline); t >= 1; t--) {
                if (slot[t] == -1) {
                    slot[t] = j.id;
                    result.push_back(j.id);
                    totalProfit += j.profit;
                    break;
                }
            }
        }
        return {totalProfit, result};
    }

    // ── Greedy 0/1 Knapsack attempt (shows failure) ───────────────────────
    static double greedyKnapsack01(vector<Item> items, double capacity) {
        sort(items.begin(), items.end(),
             [](const Item& a, const Item& b){ return a.ratio() > b.ratio(); });
        double totalValue = 0;
        for (auto& item : items) {
            if (capacity >= item.weight) {
                totalValue += item.value;
                capacity -= item.weight;
            }
            // Cannot take fraction — greedy may miss better combinations
        }
        return totalValue;
    }

    // ── Run Demo ──────────────────────────────────────────────────────────
    vector<PerformanceResult> run() {
        printBanner("MODULE 3: GREEDY ALGORITHMS — Vehicle Assignment", Color::MAGENTA);

        vector<PerformanceResult> results;

        // ── Activity Selection ──
        printSection("Activity Selection — Max Deliveries Per Vehicle");
        vector<Delivery> deliveries = {
            {1, 0,  6,  3, 150}, {2, 1,  4,  2,  80},
            {3, 3,  5,  4, 200}, {4, 5,  9,  1,  60},
            {5, 5,  7,  3, 120}, {6, 8, 12,  2,  90},
            {7, 9, 11,  4, 180}, {8, 11,14,  2,  70},
        };
        long long t0 = timeUs();
        auto selected = activitySelection(deliveries);
        long long dt = timeUs() - t0;
        cout << "    " << deliveries.size() << " delivery windows → " 
             << Color::GREEN << selected.size() << " non-overlapping selected" << Color::RESET << "\n";
        cout << "    Selected IDs: ";
        for (int id : selected) cout << "#" << id << " ";
        cout << "\n";
        results.push_back({"Activity Selection", dt, (double)selected.size(), "O(n log n)"});
        cout << "    ⏱  Time: " << dt << " µs\n\n";

        // ── Fractional Knapsack ──
        printSection("Fractional Knapsack — Maximize Value per Trip");
        vector<Item> items = {
            {1, 10, 60}, {2, 20, 100}, {3, 30, 120},
            {4, 15,  90}, {5, 25, 80},  {6,  5,  40}
        };
        double capacity = 50;
        t0 = timeUs();
        auto [fkValue, taken] = fractionalKnapsack(items, capacity);
        dt = timeUs() - t0;
        cout << "    Vehicle capacity: " << capacity << " kg\n";
        cout << "    Optimal load:\n";
        for (auto [id, frac] : taken)
            printf("      Item#%d — %.0f%% loaded  (+$%.1f)\n",
                   id, frac*100, frac * items[id-1].value);
        cout << "    " << Color::GREEN << "Total Value: $" << fkValue << Color::RESET << "\n";
        results.push_back({"Fractional Knapsack", dt, fkValue, "O(n log n)"});
        cout << "    ⏱  Time: " << dt << " µs\n\n";

        // ── Job Scheduling ──
        printSection("Job Scheduling with Deadlines — Profit Maximization");
        vector<Job> jobs = {
            {1,4,20}, {2,1,10}, {3,1,40}, {4,1,30},
            {5,3,50}, {6,2,60}, {7,2,70}
        };
        t0 = timeUs();
        auto [profit, scheduled] = jobScheduling(jobs);
        dt = timeUs() - t0;
        cout << "    Scheduled jobs: ";
        for (int id : scheduled) cout << "#" << id << " ";
        cout << "\n    " << Color::GREEN << "Total Profit: $" << profit << Color::RESET << "\n";
        results.push_back({"Job Scheduling", dt, (double)profit, "O(n² / n log n)"});
        cout << "    ⏱  Time: " << dt << " µs\n\n";

        // ── Greedy Failure Demonstration ──
        printSection("⚠  Greedy FAILS for 0/1 Knapsack — Proof");
        vector<Item> failItems = {{1,1,1},{2,1,1},{3,2,4}};
        double cap = 2;
        double greedyResult = greedyKnapsack01(failItems, cap);
        // Optimal: take item 3 (value=4), greedy takes items 1+2 (value=2)
        cout << "    Items: #1(1kg,$1) #2(1kg,$1) #3(2kg,$4)  |  Capacity: 2kg\n";
        cout << "    Greedy result:  $" << Color::RED << greedyResult << Color::RESET 
             << "  (takes #1 + #2 by ratio)\n";
        cout << "    Optimal result: $" << Color::GREEN << "4" << Color::RESET
             << "  (takes #3 alone)\n";
        cout << "    → Greedy misses global optimum! DP needed (see Module 4).\n\n";

        // ── Summary Table ──
        cout << Color::BOLD;
        cout << "    ┌─────────────────────┬────────────────┬────────────┐\n";
        cout << "    │ Algorithm           │ Complexity     │ Time (µs)  │\n";
        cout << "    ├─────────────────────┼────────────────┼────────────┤\n";
        for (auto& r : results)
            printf("    │ %-19s │ %-14s │ %-10lld │\n",
                   r.algorithm.c_str(), r.complexity.c_str(), r.timeUs);
        cout << "    └─────────────────────┴────────────────┴────────────┘\n" << Color::RESET;

        cout << "\n  ✅ Greedy is fast and near-optimal for most real-time scenarios.\n";
        cout << "     Fractional knapsack = globally optimal.\n";
        cout << "     0/1 knapsack requires Dynamic Programming (Module 4).\n";

        return results;
    }
};
