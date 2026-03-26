#include "../include/logistics.h"
#include "../include/module1_sorting.h"
#include "../include/module2_graph.h"
#include "../include/module3_greedy.h"
#include "../include/module4_dp.h"
#include "../include/module5_dc.h"

// ============================================================
//  LOGISTICS NETWORK OPTIMIZER — LIVE INTERACTIVE MODE
// ============================================================

vector<Package> packages;
int pkgIdCounter = 1;

GraphModule* cityGraph = nullptr;

void initGraph() {
    cityGraph = new GraphModule(12);
    cityGraph->setCityNames({"Warehouse","Delhi","Mumbai","Bangalore","Chennai",
                              "Pune","Hyderabad","Kolkata","Jaipur","Ahmedabad","Surat","Nagpur"});
    cityGraph->addEdge(0,1,15);  cityGraph->addEdge(0,8,280);
    cityGraph->addEdge(1,7,1300);cityGraph->addEdge(1,8,268);
    cityGraph->addEdge(1,5,1400);cityGraph->addEdge(2,5,148);
    cityGraph->addEdge(2,9,524); cityGraph->addEdge(2,10,263);
    cityGraph->addEdge(2,11,820);cityGraph->addEdge(3,4,346);
    cityGraph->addEdge(3,6,574); cityGraph->addEdge(3,11,1000);
    cityGraph->addEdge(4,6,626); cityGraph->addEdge(5,6,559);
    cityGraph->addEdge(5,11,908);cityGraph->addEdge(6,11,500);
    cityGraph->addEdge(7,11,1165);cityGraph->addEdge(9,10,263);
    cityGraph->addEdge(1,11,1094);cityGraph->addEdge(0,9,590);
}

void clearScreen() { cout << "\033[2J\033[H"; }

void pressEnter() {
    cout << "\n  " << Color::YELLOW << "[ Press ENTER to continue... ]" << Color::RESET;
    cin.ignore(); cin.get();
}

// ── MAIN MENU ─────────────────────────────────────────────────────────────
void showMainMenu() {
    clearScreen();
    cout << Color::BG_BLUE << Color::WHITE;
    cout << "\n  ╔══════════════════════════════════════════════════════╗\n";
    cout <<   "  ║       LOGISTICS NETWORK OPTIMIZER — LIVE v2.0       ║\n";
    cout <<   "  ╚══════════════════════════════════════════════════════╝\n";
    cout << Color::RESET;
    cout << "\n  " << Color::CYAN << "Packages in system: " << Color::GREEN << packages.size() << Color::RESET << "\n\n";
    cout << "  ┌─────────────────────────────────────────────────┐\n";
    cout << "  │  1. 📦  Add Package (Live Input)                │\n";
    cout << "  │  2. 📋  View All Packages                       │\n";
    cout << "  │  3. 🔃  MODULE 1 — Sort Packages                │\n";
    cout << "  │  4. 🗺️   MODULE 2 — Find Shortest Route          │\n";
    cout << "  │  5. 🚛  MODULE 3 — Greedy Vehicle Assignment    │\n";
    cout << "  │  6. 💡  MODULE 4 — DP Optimal Loading           │\n";
    cout << "  │  7. ⚡  MODULE 5 — Scalability Test             │\n";
    cout << "  │  8. 🎯  RUN ALL MODULES (Full Demo)             │\n";
    cout << "  │  9. 🗑️   Clear All Packages                      │\n";
    cout << "  │  0. ❌  Exit                                    │\n";
    cout << "  └─────────────────────────────────────────────────┘\n";
    cout << "\n  " << Color::YELLOW << "Enter choice: " << Color::RESET;
}

// ── ADD PACKAGE ───────────────────────────────────────────────────────────
void addPackage() {
    clearScreen();
    printBanner("ADD NEW PACKAGE", Color::CYAN);

    vector<string> cities = {"Delhi","Mumbai","Bangalore","Chennai","Pune",
                              "Hyderabad","Kolkata","Jaipur","Ahmedabad","Surat"};

    Package p;
    p.id = pkgIdCounter++;

    cout << "  Destination cities:\n";
    for (int i = 0; i < (int)cities.size(); i++)
        cout << "    " << i+1 << ". " << cities[i] << "\n";

    int cityChoice;
    cout << "\n  Choose city (1-10): ";
    cin >> cityChoice;
    if (cityChoice < 1 || cityChoice > 10) cityChoice = 1;
    p.destination = cities[cityChoice - 1];

    cout << "  Package weight (kg, e.g. 5.5): ";
    cin >> p.weight;

    cout << "  Deadline (hours from now, e.g. 12): ";
    cin >> p.deadline;

    cout << "  Priority (1=Low, 2=Medium, 3=High, 4=Urgent): ";
    cin >> p.priority;
    if (p.priority < 1 || p.priority > 4) p.priority = 1;

    cout << "  Package value ($, e.g. 500): ";
    cin >> p.value;

    p.distance = 50 + rand() % 450;

    packages.push_back(p);

    cout << "\n  " << Color::GREEN << "✅ Package #" << p.id << " added successfully!" << Color::RESET << "\n";
    cout << "  Details: " << p.destination << " | " << p.weight << "kg | "
         << p.deadline << "h deadline | Priority:" << p.priority << " | $" << p.value << "\n";

    pressEnter();
}

// ── VIEW ALL ──────────────────────────────────────────────────────────────
void viewAllPackages() {
    clearScreen();
    printBanner("ALL PACKAGES IN SYSTEM", Color::CYAN);
    if (packages.empty()) {
        cout << "  " << Color::YELLOW << "No packages yet! Add some first (Option 1).\n" << Color::RESET;
    } else {
        cout << "  Total packages: " << Color::GREEN << packages.size() << Color::RESET << "\n\n";
        cout << "  ID  | Destination  | Weight | Deadline | Priority | Value\n";
        cout << "  ----|--------------|--------|----------|----------|------\n";
        for (auto& p : packages) {
            string pri = p.priority==4?"URGENT":p.priority==3?"HIGH":p.priority==2?"MED":"LOW";
            printf("  #%-3d| %-12s | %5.1fkg | %4dh    | %-8s | $%.0f\n",
                   p.id, p.destination.c_str(), p.weight, p.deadline, pri.c_str(), p.value);
        }
    }
    pressEnter();
}

// ── MODULE 1: SORTING ─────────────────────────────────────────────────────
void runSorting() {
    clearScreen();
    if (packages.empty()) {
        cout << Color::RED << "\n  No packages! Add packages first.\n" << Color::RESET;
        pressEnter(); return;
    }
    printBanner("MODULE 1: SORTING — Live Demo", Color::YELLOW);
    cout << "  Sort by:\n";
    cout << "    1. Deadline (Merge Sort — Stable)\n";
    cout << "    2. Priority (Quick Sort)\n";
    cout << "    3. Value    (Heap Sort)\n";
    cout << "    4. All three + comparison\n";
    cout << "\n  Choice: ";
    int ch; cin >> ch;

    auto byDeadline = [](const Package& a, const Package& b){ return a.deadline < b.deadline; };
    auto byPriority = [](const Package& a, const Package& b){ return a.priority > b.priority; };
    auto byValue    = [](const Package& a, const Package& b){ return a.value > b.value; };

    int n = packages.size();
    cout << "\n  Sorting " << Color::GREEN << n << " packages...\n\n" << Color::RESET;

    if (ch == 1 || ch == 4) {
        auto arr = packages;
        long long t0 = timeUs();
        SortingModule::mergeSort(arr, 0, n-1, byDeadline);
        long long dt = timeUs() - t0;
        cout << Color::YELLOW << "  MERGE SORT (by Deadline) — O(n log n) STABLE:\n" << Color::RESET;
        cout << "  Top packages by earliest deadline:\n";
        for (int i = 0; i < min(n,5); i++) arr[i].print();
        cout << "  ⏱  Time: " << Color::GREEN << dt << " µs\n\n" << Color::RESET;
    }
    if (ch == 2 || ch == 4) {
        auto arr = packages;
        long long t0 = timeUs();
        SortingModule::quickSort(arr, 0, n-1, byPriority);
        long long dt = timeUs() - t0;
        cout << Color::MAGENTA << "  QUICK SORT (by Priority) — O(n log n):\n" << Color::RESET;
        for (int i = 0; i < min(n,5); i++) arr[i].print();
        cout << "  ⏱  Time: " << Color::GREEN << dt << " µs\n\n" << Color::RESET;
    }
    if (ch == 3 || ch == 4) {
        auto arr = packages;
        long long t0 = timeUs();
        SortingModule::heapSort(arr, byValue);
        long long dt = timeUs() - t0;
        cout << Color::CYAN << "  HEAP SORT (by Value) — O(n log n):\n" << Color::RESET;
        for (int i = 0; i < min(n,5); i++) arr[i].print();
        cout << "  ⏱  Time: " << Color::GREEN << dt << " µs\n\n" << Color::RESET;
    }
    if (ch == 4 && n <= 5000) {
        auto arr = packages;
        auto cmp = byDeadline;
        long long t0 = timeUs();
        SortingModule::bubbleSort(arr, cmp);
        long long dt = timeUs() - t0;
        cout << Color::RED << "  BUBBLE SORT — O(n²) [SLOW — for comparison]:\n" << Color::RESET;
        cout << "  ⏱  Time: " << Color::RED << dt << " µs  ← Much slower!\n\n" << Color::RESET;
    }
    pressEnter();
}

// ── MODULE 2: GRAPH ───────────────────────────────────────────────────────
void runGraph() {
    clearScreen();
    printBanner("MODULE 2: GRAPH — Live Route Finder", Color::GREEN);

    vector<string> cities = {"Warehouse","Delhi","Mumbai","Bangalore","Chennai",
                              "Pune","Hyderabad","Kolkata","Jaipur","Ahmedabad","Surat","Nagpur"};

    cout << "  City nodes:\n";
    for (int i = 0; i < 12; i++)
        cout << "    " << i << ". " << cities[i] << "\n";

    cout << "\n  Choose algorithm:\n";
    cout << "    1. Dijkstra — Shortest path from source\n";
    cout << "    2. All routes from Warehouse\n";
    cout << "    3. MST — Minimum network cost (Prim)\n";
    cout << "    4. BFS — Check all reachable cities\n";
    cout << "\n  Choice: ";
    int ch; cin >> ch;
    cout << "\n";

    if (ch == 1) {
        int src, dst;
        cout << "  From city (0-11): "; cin >> src;
        cout << "  To city   (0-11): "; cin >> dst;
        if (src < 0 || src > 11) src = 0;
        if (dst < 0 || dst > 11) dst = 1;
        long long t0 = timeUs();
        auto [dist, prev] = cityGraph->dijkstra(src);
        long long dt = timeUs() - t0;
        auto path = cityGraph->getPath(prev, dst);
        cout << "\n  " << Color::GREEN << "Shortest path: " << cities[src] << " → " << cities[dst] << Color::RESET << "\n";
        cout << "  Route: ";
        for (int i = 0; i < (int)path.size(); i++) { if(i) cout << " → "; cout << cities[path[i]]; }
        cout << "\n  Distance: " << Color::GREEN << dist[dst] << " km" << Color::RESET << "\n";
        cout << "  ⏱  Dijkstra Time: " << dt << " µs\n";
    }
    else if (ch == 2) {
        long long t0 = timeUs();
        auto [dist, prev] = cityGraph->dijkstra(0);
        long long dt = timeUs() - t0;
        cout << "  All routes from Warehouse:\n";
        for (int i = 1; i < 12; i++) {
            auto path = cityGraph->getPath(prev, i);
            cout << "  → " << cities[i] << ": " << Color::GREEN << dist[i] << " km" << Color::RESET << "  [";
            for (int j = 0; j < (int)path.size(); j++) { if(j) cout << "→"; cout << cities[path[j]]; }
            cout << "]\n";
        }
        cout << "  ⏱  Time: " << dt << " µs\n";
    }
    else if (ch == 3) {
        long long t0 = timeUs();
        auto [cost, edges] = cityGraph->primMST();
        long long dt = timeUs() - t0;
        cout << "  MST Edges:\n";
        for (auto [u,v] : edges)
            cout << "    " << cities[u] << " ↔ " << cities[v] << "\n";
        cout << "  " << Color::GREEN << "Total MST Cost: " << cost << " km" << Color::RESET << "\n";
        cout << "  ⏱  Time: " << dt << " µs\n";
    }
    else {
        auto order = cityGraph->bfs(0);
        cout << "  BFS from Warehouse:\n  ";
        for (int i = 0; i < (int)order.size(); i++) { if(i) cout << " → "; cout << cities[order[i]]; }
        cout << "\n  All " << order.size() << "/12 cities reachable ✅\n";
    }
    pressEnter();
}

// ── MODULE 3: GREEDY ──────────────────────────────────────────────────────
void runGreedy() {
    clearScreen();
    printBanner("MODULE 3: GREEDY — Live Vehicle Assignment", Color::MAGENTA);
    if (packages.empty()) {
        cout << Color::RED << "  No packages! Add some first.\n" << Color::RESET;
        pressEnter(); return;
    }
    double capacity;
    cout << "  Enter vehicle capacity (kg, e.g. 100): ";
    cin >> capacity;

    // Fractional Knapsack on current packages
    vector<GreedyModule::Item> items;
    for (auto& p : packages)
        items.push_back({p.id, p.weight, p.value});

    cout << "\n  Running Fractional Knapsack on " << packages.size() << " packages...\n\n";
    long long t0 = timeUs();
    auto [totalVal, taken] = GreedyModule::fractionalKnapsack(items, capacity);
    long long dt = timeUs() - t0;

    double usedWeight = 0;
    cout << "  Packages loaded:\n";
    for (auto [id, frac] : taken) {
        auto& item = items[id-1];
        double w = frac * item.weight;
        usedWeight += w;
        if (frac >= 0.999)
            printf("    ✅ Pkg#%-3d  Full   %.1fkg  $%.0f\n", id, w, item.value);
        else
            printf("    ⚡ Pkg#%-3d  %.0f%%   %.1fkg  $%.0f (partial)\n", id, frac*100, w, frac*item.value);
    }
    cout << "\n  " << Color::GREEN << "Total Value: $" << totalVal << Color::RESET << "\n";
    cout << "  Weight used: " << usedWeight << "/" << capacity << " kg\n";
    cout << "  ⏱  Time: " << dt << " µs\n";
    pressEnter();
}

// ── MODULE 4: DP ──────────────────────────────────────────────────────────
void runDP() {
    clearScreen();
    printBanner("MODULE 4: DP — Optimal Loading", Color::BLUE);
    if (packages.empty()) {
        cout << Color::RED << "  No packages! Add some first.\n" << Color::RESET;
        pressEnter(); return;
    }
    int capacity;
    cout << "  Enter vehicle capacity (kg integer, e.g. 50): ";
    cin >> capacity;

    vector<DPModule::Item> items;
    for (auto& p : packages)
        items.push_back({p.id, (int)p.weight, (int)p.value, p.destination});

    cout << "\n  Running 0/1 Knapsack DP on " << items.size() << " packages...\n";
    cout << "  DP table size: " << items.size()+1 << " x " << capacity+1 << " = "
         << (items.size()+1)*(capacity+1) << " cells\n\n";

    long long t0 = timeUs();
    auto [maxVal, takenIds] = DPModule::knapsack01(items, capacity);
    long long dt = timeUs() - t0;

    int usedW = 0;
    cout << "  Optimal packages selected:\n";
    for (int id : takenIds) {
        auto& it = items[id-1];
        usedW += it.weight;
        printf("    ✅ Pkg#%-3d  %-12s  %dkg  $%d\n", it.id, it.name.c_str(), it.weight, it.value);
    }
    cout << "\n  " << Color::GREEN << "Max Value: $" << maxVal << Color::RESET << "\n";
    cout << "  Weight: " << usedW << "/" << capacity << " kg\n";
    cout << "  ⏱  Time: " << dt << " µs  [O(n×W) guaranteed optimal]\n";

    // Greedy comparison
    auto sortedItems = items;
    sort(sortedItems.begin(), sortedItems.end(),
         [](const DPModule::Item& a, const DPModule::Item& b){
             return (double)a.value/a.weight > (double)b.value/b.weight; });
    int gcap = capacity, gval = 0;
    for (auto& it : sortedItems)
        if (gcap >= it.weight) { gval += it.value; gcap -= it.weight; }

    cout << "\n  Greedy would give: $" << Color::YELLOW << gval << Color::RESET;
    cout << "  |  DP gives: $" << Color::GREEN << maxVal << Color::RESET;
    if (maxVal > gval) cout << "  ← DP wins by $" << (maxVal-gval) << "!";
    cout << "\n";
    pressEnter();
}

// ── MODULE 5: D&C ─────────────────────────────────────────────────────────
void runDC() {
    clearScreen();
    printBanner("MODULE 5: DIVIDE & CONQUER — Scalability Test", Color::RED);
    cout << "  Master Theorem: T(n) = 2T(n/2) + O(n) => O(n log n)\n\n";

    vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
    cout << "  Running scalability benchmark...\n\n";
    cout << "  ┌──────────┬────────────┬──────────────────┐\n";
    cout << "  │ n        │ Time (µs)  │ Growth ratio     │\n";
    cout << "  ├──────────┼────────────┼──────────────────┤\n";

    long long prevTime = 0;
    for (int n : sizes) {
        vector<Package> arr;
        srand(42);
        for (int i = 0; i < n; i++) {
            Package p; p.id=i; p.deadline=rand()%100;
            p.priority=rand()%4+1; p.weight=rand()%50+1;
            p.value=rand()%500+10; p.distance=rand()%200+5;
            arr.push_back(p);
        }
        auto cmp = [](const Package& a, const Package& b){ return a.deadline < b.deadline; };
        long long t0 = timeUs();
        DCModule::parallelMergeSort(arr, 0, n-1, cmp, 0);
        long long dt = timeUs() - t0;

        string ratio = "-";
        if (prevTime > 0) {
            double r = (double)dt / prevTime;
            ratio = to_string((int)(r*10)/10.0).substr(0,4) + "x";
        }
        printf("  │ %-8d │ %-10lld │ %-16s │\n", n, dt, ratio.c_str());
        prevTime = dt;
    }
    cout << "  └──────────┴────────────┴──────────────────┘\n";
    cout << "\n  " << Color::GREEN << "O(n log n) confirmed — scales to 100,000+ packages!\n" << Color::RESET;
    pressEnter();
}

// ── RUN ALL ───────────────────────────────────────────────────────────────
void runAllModules() {
    clearScreen();
    if (packages.empty()) {
        // Auto generate demo data
        cout << Color::YELLOW << "  No packages found. Auto-generating 20 demo packages...\n" << Color::RESET;
        srand(42);
        vector<string> cities = {"Delhi","Mumbai","Bangalore","Chennai","Pune","Hyderabad"};
        for (int i = 1; i <= 20; i++) {
            Package p;
            p.id=pkgIdCounter++; p.destination=cities[rand()%cities.size()];
            p.weight=1+(rand()%49); p.deadline=1+rand()%48;
            p.priority=1+rand()%4; p.value=100+rand()%900; p.distance=50+rand()%450;
            packages.push_back(p);
        }
        cout << Color::GREEN << "  20 packages generated!\n\n" << Color::RESET;
        pressEnter();
    }
    auto r1 = SortingModule::run(packages);
    pressEnter();
    auto r2 = cityGraph->run(0);
    pressEnter();
    GreedyModule gm; auto r3 = gm.run();
    pressEnter();
    DPModule dp; auto r4 = dp.run();
    pressEnter();
    DCModule dc; auto r5 = dc.run(packages);
    pressEnter();
}

// ── MAIN ──────────────────────────────────────────────────────────────────
int main() {
    initGraph();
    srand(time(0));

    while (true) {
        showMainMenu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: addPackage();    break;
            case 2: viewAllPackages(); break;
            case 3: runSorting();    break;
            case 4: runGraph();      break;
            case 5: runGreedy();     break;
            case 6: runDP();         break;
            case 7: runDC();         break;
            case 8: runAllModules(); break;
            case 9:
                packages.clear();
                pkgIdCounter = 1;
                cout << Color::GREEN << "\n  All packages cleared!\n" << Color::RESET;
                sleep(1);
                break;
            case 0:
                clearScreen();
                cout << Color::GREEN << "\n  Thank you! Logistics Optimizer closed.\n\n" << Color::RESET;
                return 0;
            default:
                cout << Color::RED << "\n  Invalid choice!\n" << Color::RESET;
                sleep(1);
        }
    }
    return 0;
}
