# Logistics Network Optimizer

An interactive C++ command-line application that models a delivery logistics pipeline — package intake, route planning, vehicle assignment, and load optimization — using five classic algorithm families implemented from scratch. Built as a demonstration of algorithmic problem-solving across sorting, graph theory, greedy methods, dynamic programming, and divide-and-conquer.

## Overview

The optimizer simulates a warehouse network shipping packages to major Indian cities. A live menu lets you add packages, then run each algorithm module against the current package set to see prioritization, routing, vehicle loading, and scalability results — with timing and complexity reported for every run.

## Features

- **Live package intake** — add packages interactively with destination, weight, deadline, priority, and value
- **12-city warehouse network** — a pre-loaded weighted graph connecting a central warehouse to major cities
- **Five algorithm modules**, each isolated in its own header and independently runnable
- **Performance reporting** — every algorithm run reports execution time (microseconds) and theoretical complexity
- **Full-demo mode** — run all five modules back-to-back against the current package set
- **Colorized terminal UI** — ANSI color output for banners, sections, and results

## Algorithm modules

| Module | Focus | Algorithms |
|---|---|---|
| 1 — Sorting | Delivery prioritization | Merge Sort, Quick Sort, Heap Sort |
| 2 — Graph | Route optimization | Dijkstra, Bellman-Ford, Prim's MST, Kruskal's MST, BFS, DFS, Floyd-Warshall |
| 3 — Greedy | Vehicle assignment | Activity Selection, Fractional Knapsack, Job Scheduling (also demonstrates where greedy fails vs. 0/1 Knapsack) |
| 4 — Dynamic Programming | Optimal loading | 0/1 Knapsack, Multi-Stage DP, Floyd-Warshall |
| 5 — Divide & Conquer | Scalability | Parallel Merge Sort, Closest Pair of Warehouses, Zone Division, scalability benchmarking |

## Project structure

```
logistics-network-optimizer/
├── include/
│   ├── logistics.h          # Shared data structures, ANSI color codes, utilities
│   ├── module1_sorting.h    # Merge / Quick / Heap sort
│   ├── module2_graph.h      # Shortest path and MST algorithms
│   ├── module3_greedy.h     # Greedy vehicle assignment
│   ├── module4_dp.h         # Dynamic programming optimization
│   └── module5_dc.h         # Divide & conquer algorithms
├── src/
│   └── main.cpp             # Interactive CLI entry point and menu system
└── README.md
```

## Getting started

### Prerequisites

- A C++17-compatible compiler (`g++` or `clang++`)
- A terminal that supports ANSI escape codes (for colored output)

### Build

```bash
g++ -std=c++17 -O2 -o logistics src/main.cpp
```

### Run

```bash
./logistics
```

## Usage

On launch, you'll see the main menu:

```
1. Add Package (Live Input)
2. View All Packages
3. MODULE 1 — Sort Packages
4. MODULE 2 — Find Shortest Route
5. MODULE 3 — Greedy Vehicle Assignment
6. MODULE 4 — DP Optimal Loading
7. MODULE 5 — Scalability Test
8. RUN ALL MODULES (Full Demo)
9. Clear All Packages
0. Exit
```

A typical session:

1. Choose **1** a few times to add packages with different destinations, weights, deadlines, and priorities.
2. Choose **3–7** to run individual modules against those packages, or **8** to run everything at once.
3. Compare the reported timings and complexity for each algorithm.

## Design notes

- Each module is a self-contained header exposing static methods, so modules can be included and tested independently of the CLI.
- `logistics.h` centralizes shared types (`Package`, `Edge`, `Vehicle`, `PerformanceResult`) and terminal color utilities used across all modules.
- Module 3 deliberately includes a greedy 0/1 knapsack approximation alongside Module 4's exact DP solution, to illustrate where greedy strategies produce suboptimal results compared to dynamic programming.
- Module 5's scalability test benchmarks algorithm performance across increasing input sizes to demonstrate empirical time complexity.

## License

No license specified. Add a `LICENSE` file if you intend to distribute this project publicly.
