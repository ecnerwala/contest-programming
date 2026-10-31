#include <bits/stdc++.h>

int main() {
    std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);

    int N, K; std::cin >> N >> K;
    std::vector<int> R(N); for (int& r : R) { std::cin >> r; r--; }

    std::vector<int> firstOcc(K, N);
    std::vector<int> lastOcc(K, N);
    for (int i = N-1; i >= 0; i--) {
        if (lastOcc[R[i]] == N) {
            lastOcc[R[i]] = i;
        }
        firstOcc[R[i]] = i;
    }

    int64_t num_ways = 0;
    int best_diff = N;

    struct interval {
        int l, r, n;
    };

    std::vector<interval> intervals; intervals.reserve(N+1);
    intervals.push_back({-1, -1, N});

    for (int i = 0; i <= N; i++) {
        if (i < N && firstOcc[R[i]] == i) {
            // we're the first occurrence, there's no merging to be done
            intervals.push_back({i, i, lastOcc[R[i]]});
            continue;
        }
        int p = i == N ? -1 : firstOcc[R[i]]; // we'll pop until we hit p
        int cur_furthest = i == N ? N : lastOcc[R[i]];

        while (intervals.back().l > p) {
            if (intervals.back().n >= i) {
                cur_furthest = std::max(cur_furthest, intervals.back().n);
                intervals.pop_back();
                continue;
            } else {
                assert(intervals.back().n == intervals.back().r);
                int hi = int(intervals.size())-1;
                int lo = hi;
                while (intervals[lo-1].n < i) {
                    lo--;
                    assert(intervals[lo].n == intervals[lo].r);
                }

                // we have lo-hi inclusive
                int cnt = hi-lo+1;
                if (i == N) cnt--; // prefix == suffix, which is no good
                num_ways += int64_t(cnt) * int64_t(cnt+1) / 2;

                for (int x = lo, y = lo; x <= hi && y <= hi; ) {
                    int d = intervals[y].r - intervals[x].l + 1;
                    assert(d >= 0);
                    if (d <= (N-d)) {
                        best_diff = std::min(best_diff, (N-d) - d);
                        y++;
                    } else {
                        best_diff = std::min(best_diff, d - (N-d));
                        x++;
                    }
                }

                intervals.erase(intervals.begin()+lo, intervals.end());
            }
        }

        assert(intervals.back().l <= p && p <= intervals.back().r);
        intervals.back().r = i;
        intervals.back().n = std::max(intervals.back().n, cur_furthest);
    }

    std::cout << num_ways << ' ' << best_diff << '\n';

    return 0;
}
