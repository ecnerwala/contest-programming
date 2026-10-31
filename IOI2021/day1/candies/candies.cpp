#include "candies.h"

#include <bits/stdc++.h>

std::vector<int> distribute_candies(
    std::vector<int> C,
    std::vector<int> L,
    std::vector<int> R,
    std::vector<int> V
) {
    struct range_data {
        int64_t pref_min, pref_max, tot_val;
        bool incr;

        explicit range_data(int v = 0) {
            pref_min = std::min(0, v);
            pref_max = std::max(0, v);
            tot_val = v;
            incr = (v >= 0);
        }
        range_data(range_data a, range_data b) {
            pref_min = std::min(a.pref_min, a.tot_val + b.pref_min);
            pref_max = std::max(a.pref_max, a.tot_val + b.pref_max);
            tot_val = a.tot_val + b.tot_val;
            incr = a.incr;
        }

        int64_t range() const {
            return pref_max - pref_min;
        }
    };

    int N = int(C.size());
    int Q = int(L.size());
    assert(Q == int(R.size()));
    assert(Q == int(V.size()));
    int S = 1; while (S < Q+1) S *= 2;

    std::vector<range_data> seg(2*S);
    auto update_node = [&](int a) -> void {
        seg[a] = range_data(seg[2*a], seg[2*a+1]);
    };
    auto update_parents = [&](int a) -> void {
        for (a >>= 1; a; a >>= 1) update_node(a);
    };
    seg[S] = range_data(-*max_element(C.begin(), C.end()));
    update_parents(S);

    std::vector<std::vector<int>> evts(N+1);
    for (int q = 0; q < Q; q++) {
        evts[L[q]].push_back(q);
        evts[R[q]+1].push_back(~q);
    }

    std::vector<int> ans(N);
    for (int i = 0; i < N; i++) {
        for (int q : evts[i]) {
            if (q < 0) {
                q = ~q;
                seg[S+1+q] = range_data();
            } else {
                seg[S+1+q] = range_data(V[q]);
            }
            update_parents(S+1+q);
        }

        range_data cur_suff;
        int a = 1;
        while (a < S) {
            assert(cur_suff.range() < C[i]);
            assert(range_data(seg[a], cur_suff).range() >= C[i]);
            a <<= 1;
            a ++;
            range_data rhs = range_data(seg[a], cur_suff);
            if (rhs.range() >= C[i]) {
                // Keep digging
            } else {
                cur_suff = rhs;
                a--;
            }
        }
        assert(cur_suff.range() < C[i]);
        cur_suff = range_data(seg[a], cur_suff);
        assert(cur_suff.range() >= C[i]);
        ans[i] = cur_suff.incr ? int(C[i] - (cur_suff.pref_max - cur_suff.tot_val)) : int(cur_suff.tot_val - cur_suff.pref_min);
    }

    return ans;
}
