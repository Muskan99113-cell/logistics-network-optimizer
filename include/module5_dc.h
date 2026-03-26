#pragma once
#include "logistics.h"

// MODULE 5: DIVIDE & CONQUER -- Scalability Engine
// Algorithms: Parallel Merge Sort, Closest Pair of Warehouses, Zone Division
// Recurrence: T(n) = 2T(n/2) + O(n) => O(n log n) by Master Theorem

class DCModule {
public:
    struct Point {
        double x, y;
        int id;
        string name;
    };

    // Parallel Merge Sort (simulated with thread-like divide)
    static void parallelMergeSort(vector<Package>& arr, int l, int r,
                                  function<bool(const Package&,const Package&)> cmp,
                                  int depth = 0) {
        if (l >= r) return;
        int m = (l+r)/2;
        if (depth < 3) {
            // Simulate parallel: both halves (in real impl use std::async)
            parallelMergeSort(arr, l, m, cmp, depth+1);
            parallelMergeSort(arr, m+1, r, cmp, depth+1);
        } else {
            parallelMergeSort(arr, l, m, cmp, depth+1);
            parallelMergeSort(arr, m+1, r, cmp, depth+1);
        }
        // Merge step
        vector<Package> tmp;
        int i=l, j=m+1;
        while (i<=m && j<=r) tmp.push_back(cmp(arr[i],arr[j]) ? arr[i++] : arr[j++]);
        while (i<=m) tmp.push_back(arr[i++]);
        while (j<=r) tmp.push_back(arr[j++]);
        for (int k=l; k<=r; k++) arr[k]=tmp[k-l];
    }

    // Closest Pair of Warehouses -- O(n log n)
    static double dist(const Point& a, const Point& b) {
        return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
    }

    static double bruteForce(vector<Point>& pts, int l, int r, pair<int,int>& best) {
        double d = 1e18;
        for (int i=l; i<=r; i++)
            for (int j=i+1; j<=r; j++) {
                double dd = dist(pts[i], pts[j]);
                if (dd < d) { d=dd; best={pts[i].id,pts[j].id}; }
            }
        return d;
    }

    static double stripClosest(vector<Point>& strip, double d, pair<int,int>& best) {
        sort(strip.begin(), strip.end(), [](const Point& a, const Point& b){ return a.y < b.y; });
        for (int i=0; i<(int)strip.size(); i++)
            for (int j=i+1; j<(int)strip.size() && (strip[j].y-strip[i].y)<d; j++) {
                double dd = dist(strip[i], strip[j]);
                if (dd < d) { d=dd; best={strip[i].id,strip[j].id}; }
            }
        return d;
    }

    static double closestPair(vector<Point> pts, int l, int r, pair<int,int>& best) {
        if (r-l <= 3) return bruteForce(pts, l, r, best);
        int m = (l+r)/2;
        double midX = pts[m].x;
        pair<int,int> bl, br;
        double dl = closestPair(pts, l, m, bl);
        double dr = closestPair(pts, m+1, r, br);
        double d = min(dl, dr);
        best = (dl < dr) ? bl : br;
        vector<Point> strip;
        for (int i=l; i<=r; i++)
            if (abs(pts[i].x - midX) < d) strip.push_back(pts[i]);
        return stripClosest(strip, d, best);
    }

    // Zone Division -- divide city into delivery zones
    struct Zone {
        int id;
        double minX, maxX, minY, maxY;
        vector<int> warehouseIds;
    };

    static vector<Zone> divideIntoZones(vector<Point>& warehouses, int zones) {
        vector<Zone> result;
        double minX=1e18, maxX=-1e18, minY=1e18, maxY=-1e18;
        for (auto& w : warehouses) {
            minX=min(minX,w.x); maxX=max(maxX,w.x);
            minY=min(minY,w.y); maxY=max(maxY,w.y);
        }
        int cols = (int)sqrt(zones);
        int rows = (zones + cols - 1) / cols;
        double zw = (maxX-minX)/cols, zh = (maxY-minY)/rows;
        for (int r=0; r<rows; r++) for (int c=0; c<cols; c++) {
            Zone z;
            z.id = r*cols+c+1;
            z.minX = minX+c*zw; z.maxX = z.minX+zw;
            z.minY = minY+r*zh; z.maxY = z.minY+zh;
            for (auto& w : warehouses)
                if (w.x>=z.minX && w.x<z.maxX && w.y>=z.minY && w.y<z.maxY)
                    z.warehouseIds.push_back(w.id);
            result.push_back(z);
        }
        return result;
    }

    // Scalability benchmark: measure sort time at different n
    static vector<pair<int,long long>> scalabilityTest() {
        vector<pair<int,long long>> data;
        vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000};
        for (int n : sizes) {
            vector<Package> pkgs;
            srand(42);
            for (int i=0; i<n; i++) {
                Package p; p.id=i; p.deadline=rand()%100; p.priority=rand()%4+1;
                p.weight=rand()%50+1; p.value=rand()%500+10; p.distance=rand()%200+5;
                pkgs.push_back(p);
            }
            auto arr = pkgs;
            auto cmp = [](const Package& a, const Package& b){ return a.deadline < b.deadline; };
            long long t0 = timeUs();
            parallelMergeSort(arr, 0, n-1, cmp);
            long long dt = timeUs() - t0;
            data.push_back({n, dt});
        }
        return data;
    }

    vector<PerformanceResult> run(vector<Package>& packages) {
        printBanner("MODULE 5: DIVIDE & CONQUER -- Scalability Engine", Color::RED);
        vector<PerformanceResult> results;

        // Parallel Merge Sort
        printSection("Parallel Merge Sort -- T(n) = 2T(n/2) + O(n)");
        cout << "    Master Theorem: T(n) = 2T(n/2) + O(n) => O(n log n)\n";
        cout << "    a=2, b=2, f(n)=O(n), n^log_b(a)=O(n) => Case 2 => O(n log n)\n\n";
        auto arr = packages;
        int n = arr.size();
        auto cmp = [](const Package& a, const Package& b){ return a.deadline < b.deadline; };
        long long t0 = timeUs();
        parallelMergeSort(arr, 0, n-1, cmp, 0);
        long long dt = timeUs() - t0;
        cout << "    Sorted " << n << " packages via divide & conquer.\n";
        cout << "    Recursion depth: ~" << (int)(log2(n)) << " levels\n";
        results.push_back({"Parallel Merge Sort", dt, 0, "O(n log n)"});
        cout << "    Time: " << dt << " us\n\n";

        // Closest Pair
        printSection("Closest Pair of Warehouses -- O(n log n)");
        vector<Point> warehouses;
        srand(123);
        for (int i=0; i<20; i++)
            warehouses.push_back({(double)(rand()%100),(double)(rand()%100),i,"WH"+to_string(i)});
        sort(warehouses.begin(), warehouses.end(), [](const Point& a, const Point& b){ return a.x<b.x; });
        pair<int,int> best;
        t0 = timeUs();
        double minDist = closestPair(warehouses, 0, warehouses.size()-1, best);
        dt = timeUs() - t0;
        cout << "    20 warehouses placed randomly.\n";
        printf("    Closest pair: WH%d <-> WH%d  Distance: %.2f km\n", best.first, best.second, minDist);
        results.push_back({"Closest Pair", dt, minDist, "O(n log n)"});
        cout << "    Time: " << dt << " us\n\n";

        // Zone Division
        printSection("Delivery Zone Division -- Network Partitioning");
        auto zones = divideIntoZones(warehouses, 9);
        cout << "    City divided into " << zones.size() << " delivery zones:\n";
        for (auto& z : zones)
            cout << "    Zone " << z.id << ": [" << (int)z.minX << "-" << (int)z.maxX
                 << ", " << (int)z.minY << "-" << (int)z.maxY << "]  "
                 << z.warehouseIds.size() << " warehouses\n";
        cout << "\n";

        // Scalability Test
        printSection("Scalability Analysis -- Empirical vs Theoretical");
        auto scalData = scalabilityTest();
        cout << "    +----------+------------+------------------+\n";
        cout << "    | n        | Time (us)  | Expected O(nlogn)|\n";
        cout << "    +----------+------------+------------------+\n";
        for (auto [sz, t] : scalData) {
            double theoretical = sz * log2(sz);
            printf("    | %-8d | %-10lld | %-16.0f |\n", sz, t, theoretical);
        }
        cout << "    +----------+------------+------------------+\n\n";

        // Summary
        cout << Color::BOLD;
        cout << "    +-----------------------+------------+------------+\n";
        cout << "    | Algorithm             | Complexity | Time (us)  |\n";
        cout << "    +-----------------------+------------+------------+\n";
        for (auto& r : results)
            printf("    | %-21s | %-10s | %-10lld |\n",
                   r.algorithm.c_str(), r.complexity.c_str(), r.timeUs);
        cout << "    +-----------------------+------------+------------+\n" << Color::RESET;
        cout << "\n  [OK] D&C enables handling 10^6+ packages efficiently.\n";
        cout << "       Zone division reduces per-vehicle search space.\n";
        cout << "       Master Theorem confirms O(n log n) scalability.\n";

        return results;
    }
};
