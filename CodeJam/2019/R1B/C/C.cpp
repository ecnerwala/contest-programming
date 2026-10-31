#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
ll N;
int K;
int C[MAXN][2];

ll go() {
    cin >> N >> K;
    for (int i = 0; i < N; i++) cin >> C[i][0];
    for (int i = 0; i < N; i++) cin >> C[i][1];

    vector<pair<int, pair<int, bool>>> vals;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 2; j++) {
            vals.emplace_back(C[i][j], pair<int, bool>(i,j));
        }
    }
    sort(vals.begin(), vals.end());
    reverse(vals.begin(), vals.end());

    ll res = 0;
    set<int> minusK[2];
    minusK[0].emplace(-1), minusK[1].emplace(-1);
    minusK[0].emplace(N), minusK[1].emplace(N);
    auto minusKIt = vals.begin();
    set<pair<int, bool>> bigger;
    bigger.emplace(-1, true);
    bigger.emplace(N, false);
    for (auto it : vals) {
        while (minusKIt != vals.end() && minusKIt->first >= it.first - K) {
            minusK[minusKIt->second.second].insert(minusKIt->second.first);
            minusKIt++;
        }

        auto bIns = bigger.insert(it.second);
        assert(bIns.second);
        auto bIt = bIns.first;

        int bLo, mLo, mHi, bHi;
        { // high first
            auto bJt = bIt;
            bJt++;
            bHi = bJt->first;
            auto mIt = minusK[!it.second.second].lower_bound(it.second.first);
            mHi = min(bHi, *mIt);
        }
        {
            auto bJt = bIt;
            --bJt;
            bLo = bJt->first;
            auto mIt = minusK[!it.second.second].upper_bound(it.second.first);
            --mIt;
            mLo = max(bLo, *mIt);
        }

        assert(bLo <= mLo && mLo <= it.second.first && it.second.first <= mHi && mHi <= bHi);

        res += ll(bHi - it.second.first) * ll(it.second.first - bLo);
        res -= ll(mHi - it.second.first) * ll(it.second.first - mLo);
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int T; cin >> T;
    for (int t = 1; t <= T; t++) {
        ll ans = go();
        cout << "Case #" << t << ": " << ans << '\n';
    }

    return 0;
}
