#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

namespace ecnerwala {
std::mt19937 mt(48);
struct treap_node {
    std::array<treap_node*, 2> c = {nullptr, nullptr};
    std::mt19937::result_type pri = mt();
    std::pair<int64_t, int> val;
};

std::pair<treap_node*, treap_node*> split(treap_node* a, std::pair<int64_t, int> key) {
    if (!a) return {nullptr, nullptr};
    treap_node *x, *y;
    if (key < a->val) {
        y = a;
        std::tie(x, y->c[0]) = split(a->c[0], key);
    } else {
        x = a;
        std::tie(x->c[1], y) = split(a->c[1], key);
    }
    return {x, y};
}

treap_node* meld(treap_node* a, treap_node* b) {
    if (!a) return b;
    if (!b) return a;
    if (a->pri > b->pri) {
        std::swap(a, b);
    }
    auto v = split(b, a->val);
    a->c[0] = meld(a->c[0], v.first);
    a->c[1] = meld(a->c[1], v.second);
	return a;
}

} // namespace ecnerwala

using namespace std;
typedef long long ll;
const int MAXN = 300100;

int N, K;
ll w[MAXN];
ll dp[MAXN]; // dp to own this fool. potential function
ll sdp[MAXN]; // suffix dp
int nv[MAXN];

const int MAXNODES = 1e6;
int NEXT_NODE = 0;
ecnerwala::treap_node node_pool[MAXNODES];
ecnerwala::treap_node* mm[MAXN];

void vput (int x, int y)
{
    mm[y] = meld(mm[x], mm[y]);
}

void insert(ecnerwala::treap_node* &a, int64_t v) {
    int idx = NEXT_NODE++;
    node_pool[idx].c = {nullptr, nullptr};
    node_pool[idx].val = {v, idx};
    a = meld(a, &node_pool[idx]);
}

int works (ll C) // cut everything >= C
{
    if (C <= 0) return 0;

    NEXT_NODE = 0;
    for (int i = 0; i < N; i++)
        mm[i] = nullptr;
    insert(mm[0], 0);

    int nlive = 1;
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j <= min (N - 1, i + 3); j++)
        {
            if (nv[i] == j) continue;
            ll ndiff = (sdp[j] - sdp[i] + w[j]);
            std::y_combinator([&](auto self, ecnerwala::treap_node* cur) -> bool {
                if (!cur) return false;

                if (self(cur->c[0])) return true;

                ll x = cur->val.first;
                if (x + ndiff >= C) return true;
                insert(mm[j], x + ndiff);
                nlive++;
                if (nlive >= K) return true;

                if (self(cur->c[1])) return true;

                return false;
            })(mm[i]);
            if (nlive >= K) return K;
        }

        if (i < N - 1)
            vput (i, nv[i]);
        //cout << i << " " << mm[i].size() << "\n";
    }
    return nlive;
}

void gogo()
{
    cin >> N >> K;
    for (int i = 2; i < N + 2; i++)
        cin >> w[i];
    w[0] = w[N+3] = 0;
    w[1] = w[N+2] = 1e15;
    N += 4;

    dp[0] = 0;
    for (int i = 1; i < N; i++)
    {
        dp[i] = 1e16;
        for (int j = max (i - 3, 0); j < i; j++)
            dp[i] = min (dp[i], dp[j] + w[i]);
    }

    sdp[N-1] = 0;
    for (int i = N - 2; i >= 0; i--)
    {
        sdp[i] = 1e16;
        for (int j = i + 1; j <= min (i + 3, N - 1); j++)
            sdp[i] = min (sdp[i], sdp[j] + w[i]);
    }
    for (int i = 0; i < N; i++)
        sdp[i] -= w[i];
    for (int i = 0; i < N - 2; i++)
        for (int j = i + 1; j <= min (i + 3, N - 1); j++)
            if (sdp[i] == sdp[j] + w[j])
                nv[i] = j;
    //cout << sdp[0] << " " << dp[N-1] << "\n";

    //for (int i = 0; i < N; i++)
    //    cout << nv[i] << "\n";

    if (N <= 25 && works ((ll) 1e14) < K)
    {
        int V = K;
        K = 1e9;

        works ((ll) 1e14);
        K = V;
        //cout << mm[N-1].size() << "\n";
        int np = 0;
        std::y_combinator([&](auto self, ecnerwala::treap_node* cur) -> bool {
            if (!cur) return false;

            if (self(cur->c[0])) return true;

            ll x = cur->val.first;
            cout << x + dp[N-1] << "\n";
            np++;
            if (np >= K) return true;

            if (self(cur->c[1])) return true;

            return false;
        })(mm[N-1]);
        for (int i = np; i < K; i++)
            cout << "-1\n";
        return;
    }

    ll v = 1;
    while (v <= 1e15 && works (v) < K)
        v = v * 2;
    ll lo = v / 2, hi = v - 1;
    while (lo < hi)
    {
        //cout << lo << " " << hi << "\n";
        ll mid = (lo + hi + 1) / 2;
        if (works (mid) < K)
            lo = mid;
        else
            hi = mid - 1;
    }
    works (lo);

    ll ans = dp[N-1];
    int np = 0;
    std::y_combinator([&](auto self, ecnerwala::treap_node* cur) -> bool {
        if (!cur) return false;

        if (self(cur->c[0])) return true;

        ll x = cur->val.first;
        cout << x + ans << "\n";
        np++;
        if (np >= K) return true;

        if (self(cur->c[1])) return true;

        return false;
    })(mm[N-1]);
    for (int i = np; i < K; i++)
        cout << lo + ans << "\n";
}

int main()
{
    ios_base::sync_with_stdio(0);

    int T = 1;
    for (int t = 0; t < T; t++)
        gogo();    
}
