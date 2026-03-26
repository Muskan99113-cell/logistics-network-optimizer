#pragma once
#include <bits/stdc++.h>
using namespace std;

// ─── Data Structures ────────────────────────────────────────────────────────

struct Package {
    int id;
    string destination;
    double weight;
    int deadline;       // hours from now
    int priority;       // 1=low, 2=med, 3=high, 4=urgent
    double value;       // monetary value
    double distance;    // km from warehouse
    
    void print() const {
        cout << "  Pkg#" << id << " | Dest:" << destination
             << " | W:" << weight << "kg | DL:" << deadline << "h"
             << " | P:" << priority << " | Val:$" << value << "\n";
    }
};

struct Edge {
    int to, weight;
};

struct GraphEdge {
    int u, v, w;
    bool operator<(const GraphEdge& o) const { return w < o.w; }
};

struct Vehicle {
    int id;
    double capacity;
    double currentLoad;
    int currentNode;
    vector<int> assignedPackages;
    double totalFuelCost;
};

struct PerformanceResult {
    string algorithm;
    long long timeUs;
    double result;
    string complexity;
};

// ─── Colors for Terminal Output ──────────────────────────────────────────────
namespace Color {
    const string RED     = "\033[1;31m";
    const string GREEN   = "\033[1;32m";
    const string YELLOW  = "\033[1;33m";
    const string BLUE    = "\033[1;34m";
    const string MAGENTA = "\033[1;35m";
    const string CYAN    = "\033[1;36m";
    const string WHITE   = "\033[1;37m";
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string BG_BLUE = "\033[44m";
    const string BG_GREEN= "\033[42m";
}

// ─── Utility ─────────────────────────────────────────────────────────────────
inline void printBanner(const string& title, const string& color) {
    int w = 60;
    string line(w, '=');
    cout << color << "\n" << line << "\n";
    int pad = (w - title.size()) / 2;
    cout << string(pad, ' ') << title << "\n";
    cout << line << Color::RESET << "\n\n";
}

inline void printSection(const string& s) {
    cout << Color::CYAN << "  ▶ " << s << Color::RESET << "\n";
}

inline long long timeUs() {
    return chrono::duration_cast<chrono::microseconds>(
        chrono::high_resolution_clock::now().time_since_epoch()).count();
}
