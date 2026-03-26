#pragma once
#include "logistics.h"

// ═══════════════════════════════════════════════════════════════════════════
//  MODULE 1: SORTING — Delivery Prioritization Engine
//  Algorithms: Merge Sort, Quick Sort, Heap Sort
//  Complexity: O(n log n) vs O(n²) comparison
// ═══════════════════════════════════════════════════════════════════════════

class SortingModule {
public:

    // ── Merge Sort (stable — preserves relative order for equal deadlines) ──
    static void merge(vector<Package>& arr, int l, int m, int r,
                      function<bool(const Package&, const Package&)> cmp) {
        vector<Package> left(arr.begin()+l, arr.begin()+m+1);
        vector<Package> right(arr.begin()+m+1, arr.begin()+r+1);
        int i=0, j=0, k=l;
        while (i < (int)left.size() && j < (int)right.size())
            arr[k++] = cmp(left[i], right[j]) ? left[i++] : right[j++];
        while (i < (int)left.size())  arr[k++] = left[i++];
        while (j < (int)right.size()) arr[k++] = right[j++];
    }

    static void mergeSort(vector<Package>& arr, int l, int r,
                          function<bool(const Package&, const Package&)> cmp) {
        if (l >= r) return;
        int m = (l + r) / 2;
        mergeSort(arr, l, m, cmp);
        mergeSort(arr, m+1, r, cmp);
        merge(arr, l, m, r, cmp);
    }

    // ── Quick Sort (pivot-based — fast priority ranking) ───────────────────
    static int partition(vector<Package>& arr, int l, int r,
                         function<bool(const Package&, const Package&)> cmp) {
        Package pivot = arr[r];
        int i = l - 1;
        for (int j = l; j < r; j++)
            if (cmp(arr[j], pivot)) swap(arr[++i], arr[j]);
        swap(arr[i+1], arr[r]);
        return i + 1;
    }

    static void quickSort(vector<Package>& arr, int l, int r,
                          function<bool(const Package&, const Package&)> cmp) {
        if (l >= r) return;
        int p = partition(arr, l, r, cmp);
        quickSort(arr, l, p-1, cmp);
        quickSort(arr, p+1, r, cmp);
    }

    // ── Heap Sort (in-place — dynamic priority queue for live requests) ────
    static void heapify(vector<Package>& arr, int n, int i,
                        function<bool(const Package&, const Package&)> cmp) {
        int largest = i, l = 2*i+1, r = 2*i+2;
        if (l < n && cmp(arr[l], arr[largest])) largest = l;
        if (r < n && cmp(arr[r], arr[largest])) largest = r;
        if (largest != i) { swap(arr[i], arr[largest]); heapify(arr, n, largest, cmp); }
    }

    static void heapSort(vector<Package>& arr,
                         function<bool(const Package&, const Package&)> cmp) {
        int n = arr.size();
        for (int i = n/2-1; i >= 0; i--) heapify(arr, n, i, cmp);
        for (int i = n-1; i > 0; i--) { swap(arr[0], arr[i]); heapify(arr, i, 0, cmp); }
    }

    // ── Bubble Sort for O(n²) comparison ──────────────────────────────────
    static void bubbleSort(vector<Package>& arr,
                           function<bool(const Package&, const Package&)> cmp) {
        int n = arr.size();
        for (int i = 0; i < n-1; i++)
            for (int j = 0; j < n-i-1; j++)
                if (cmp(arr[j+1], arr[j])) swap(arr[j], arr[j+1]);
    }

    // ── Demo & Benchmarking ────────────────────────────────────────────────
    static vector<PerformanceResult> run(vector<Package> packages) {
        printBanner("MODULE 1: SORTING — Delivery Prioritization Engine", Color::YELLOW);

        auto byDeadline  = [](const Package& a, const Package& b){ return a.deadline < b.deadline; };
        auto byPriority  = [](const Package& a, const Package& b){ return a.priority > b.priority; };
        auto byValue     = [](const Package& a, const Package& b){ return a.value > b.value; };

        vector<PerformanceResult> results;
        int n = packages.size();

        // ── 1. Merge Sort by Deadline ──────────────────────────────────────
        {
            auto arr = packages;
            long long t0 = timeUs();
            mergeSort(arr, 0, n-1, byDeadline);
            long long dt = timeUs() - t0;
            printSection("Merge Sort (by Deadline) — O(n log n) STABLE");
            cout << "    Top 5 by earliest deadline:\n";
            for (int i = 0; i < min(5,(int)arr.size()); i++) arr[i].print();
            results.push_back({"Merge Sort", dt, (double)arr[0].deadline, "O(n log n)"});
            cout << "    ⏱  Time: " << dt << " µs\n\n";
        }

        // ── 2. Quick Sort by Priority ──────────────────────────────────────
        {
            auto arr = packages;
            long long t0 = timeUs();
            quickSort(arr, 0, n-1, byPriority);
            long long dt = timeUs() - t0;
            printSection("Quick Sort (by Priority) — O(n log n) avg");
            cout << "    Top 5 by highest priority:\n";
            for (int i = 0; i < min(5,(int)arr.size()); i++) arr[i].print();
            results.push_back({"Quick Sort", dt, (double)arr[0].priority, "O(n log n) avg"});
            cout << "    ⏱  Time: " << dt << " µs\n\n";
        }

        // ── 3. Heap Sort by Value ──────────────────────────────────────────
        {
            auto arr = packages;
            long long t0 = timeUs();
            heapSort(arr, byValue);
            long long dt = timeUs() - t0;
            printSection("Heap Sort (by Value) — O(n log n) in-place");
            cout << "    Top 5 by highest value:\n";
            for (int i = 0; i < min(5,(int)arr.size()); i++) arr[i].print();
            results.push_back({"Heap Sort", dt, arr[0].value, "O(n log n)"});
            cout << "    ⏱  Time: " << dt << " µs\n\n";
        }

        // ── 4. Bubble Sort for comparison ─────────────────────────────────
        if (n <= 5000) {
            auto arr = packages;
            long long t0 = timeUs();
            bubbleSort(arr, byDeadline);
            long long dt = timeUs() - t0;
            printSection("Bubble Sort — O(n²) [for comparison only]");
            results.push_back({"Bubble Sort O(n²)", dt, 0, "O(n²)"});
            cout << "    ⏱  Time: " << dt << " µs  ← " << Color::RED << "much slower!" << Color::RESET << "\n\n";
        }

        // ── Complexity Table ───────────────────────────────────────────────
        cout << Color::BOLD << "    ┌─────────────────┬────────────┬────────────┐\n";
        cout <<              "    │ Algorithm       │ Complexity │ Time (µs)  │\n";
        cout <<              "    ├─────────────────┼────────────┼────────────┤\n";
        for (auto& r : results)
            printf("    │ %-15s │ %-10s │ %-10lld │\n",
                   r.algorithm.c_str(), r.complexity.c_str(), r.timeUs);
        cout <<              "    └─────────────────┴────────────┴────────────┘\n" << Color::RESET;

        cout << "\n  ✅ Stability Analysis: Merge Sort preserves fairness for equal deadlines.\n";
        cout << "     Quick Sort is faster in practice for priority ranking.\n";
        cout << "     Heap Sort best for streaming/dynamic arrival of packages.\n";

        return results;
    }
};
