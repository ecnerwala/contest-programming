#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = int(1e9) + 9;

const ll INF = 1e18;

typedef pair<ll, ll> pii;
typedef pair<pii, ll> ppi;

const int MAXN = 6e6;
int N;

int P[MAXN];
int E[MAXN];
set<ll> _sets[3];
set<ll> * const sets = _sets + 1;

void init() {
    sets[0].clear();
    sets[-1].clear();
    sets[1].clear();
}

void create(ll a) {
    if (sets[0].count(a) || sets[1].count(a) || sets[-1].count(a)) {
        return;
    }
    sets[0].insert(a);
}

ll go(ll p, int e) {
    ll vright, vleft;
    // go right first
    {
        ll v0;
        auto it1 = sets[0].lower_bound(p);
        if (it1 == sets[0].end()) {
            v0 = INF;
        } else {
            v0 = *it1;
        }
        ll ve;
        auto it2 = sets[e].lower_bound(p);
        if (it2 == sets[e].end()) {
            ve = INF;
        } else {
            ve = *it2;
        }
        ll v;
        if (sets[-e].count(p)) {
            assert(v0 != INF);
            v = min(v0, ve);
        } else {
            v = p;
        }
        assert(!sets[-e].count(v));
        if (sets[e].count(v)) {
            sets[e].erase(v);
            sets[0].insert(v);
        } else {
            sets[0].erase(v);
            sets[-e].insert(v);
        }
        create(v+1);
        create(v-1);
        vright = v;
    }
    { // go left
        ll v0;
        auto it1 = sets[0].lower_bound(p);
        if (it1 == sets[0].begin()) {
            v0 = -INF;
        } else {
            --it1;
            v0 = *it1;
        }
        ll ve;
        auto it2 = sets[-e].lower_bound(p);
        if (it2 == sets[-e].begin()) {
            ve = -INF;
        } else {
            --it2;
            ve = *it2;
        }
        ll v;
        if (sets[e].count(p-1)) {
            assert(v0 != -INF);
            v = max(v0, ve);
        } else {
            v = p-1;
        }

        assert(!sets[e].count(v));
        if (sets[-e].count(v)) {
            sets[-e].erase(v);
            sets[0].insert(v);
        } else {
            sets[0].erase(v);
            sets[e].insert(v);
        }
        create(v+1);
        create(v-1);
        vleft = v;
    }
    return vright - vleft;
}

int main() {
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        cerr << "running case " << t << '\n';
        cin >> N;
        ll res = 0;
        init();
        for (int i = -10000; i <= 10000; i++) {
            sets[0].insert(i);
        }
        for (int i = 1; i <= N; i++) {
            int p, e; cin >> p >> e;
            ll v = go(p, e) % MOD;
            res += v * i;
            res %= MOD;
        }
        cout << res << '\n';
    }
}
