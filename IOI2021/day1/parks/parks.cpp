#include "parks.h"

#include <bits/stdc++.h>

int construct_roads(std::vector<int> X, std::vector<int> Y) {
    int N = int(X.size());

    std::vector<int> north(N, -1);
    std::vector<int> east(N, -1);

    {
        struct pt_t {
            int x, y, idx;
        };
        std::vector<pt_t> pts(N);
        for (int i = 0; i < N; i++) pts[i] = {X[i], Y[i], i};

        std::sort(pts.begin(), pts.end(), [](pt_t a, pt_t b) { return std::tie(a.x, a.y) < std::tie(b.x, b.y); });
        for (int i = 0; i+1 < N; i++) {
            if (pts[i].x == pts[i+1].x && pts[i].y + 2 == pts[i+1].y) {
                north[pts[i].idx] = pts[i+1].idx;
            }
        }

        std::sort(pts.begin(), pts.end(), [](pt_t a, pt_t b) { return std::tie(a.y, a.x) < std::tie(b.y, b.x); });
        for (int i = 0; i+1 < N; i++) {
            if (pts[i].y == pts[i+1].y && pts[i].x + 2 == pts[i+1].x) {
                east[pts[i].idx] = pts[i+1].idx;
            }
        }
    }

    std::vector<int> par(N, -1);
    auto get_par = [&](int a) -> int {
        while (par[a] >= 0) {
            if (par[par[a]] >= 0) par[a] = par[par[a]];
            a = par[a];
        }
        return a;
    };
    auto merge = [&](int a, int b) -> bool {
        a = get_par(a), b = get_par(b);
        if (a == b) return false;
        if (par[a] > par[b]) std::swap(a, b);
        par[a] += par[b];
        par[b] = a;
        return true;
    };

    std::vector<int> U, V, A, B; U.reserve(N-1), V.reserve(N-1), A.reserve(N-1), B.reserve(N-1);

    for (int i = 0; i < N; i++) {
        bool is_full = east[i] != -1 && north[i] != -1 && north[east[i]] != -1;

        if (east[i] != -1) {
            if ((X[i]^Y[i])&2) {
                // We use below us
                if (merge(i, east[i])) {
                    U.push_back(i);
                    V.push_back(east[i]);
                    A.push_back(X[i]+1);
                    B.push_back(Y[i]-1);
                }
            } else {
                // We use above of us, and we're not preferred
                if (!is_full) {
                    if (merge(i, east[i])) {
                        U.push_back(i);
                        V.push_back(east[i]);
                        A.push_back(X[i]+1);
                        B.push_back(Y[i]+1);
                    }
                }
            }
        }
        if (north[i] != -1) {
            if ((X[i]^Y[i])&2) {
                // We use right of us, and we're not preferred
                if (!is_full) {
                    if (merge(i, north[i])) {
                        U.push_back(i);
                        V.push_back(north[i]);
                        A.push_back(X[i]+1);
                        B.push_back(Y[i]+1);
                    }
                }
            } else {
                // We use left of us
                if (merge(i, north[i])) {
                    U.push_back(i);
                    V.push_back(north[i]);
                    A.push_back(X[i]-1);
                    B.push_back(Y[i]+1);
                }
            }
        }
    }

    if (int(U.size()) != N-1) return false;
    build(U, V, A, B); return true;
}
