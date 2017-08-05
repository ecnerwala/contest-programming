#include <bits/stdc++.h>
using namespace std;

// support mark range, query number unmarked
struct seg_tree {
    static const int L = 18;
    static const int S = 1 << L;
    int N;
    struct node {
        int lazy = 0, val = 0;
    } s[2 * S];
    void update_node(int i) {
        if (s[i].lazy) {
            s[i].val = 0;
        } else if (i >= S) {
            s[i].val = (i - S < N); // only the included things
        } else {
            s[i].val = s[2 * i].val + s[2 * i + 1].val;
        }
    }
    seg_tree(int n) : N(n) {
        memset(s, 0, sizeof(s));
        for (int i = 2 * S - 1; i; i--) {
            update_node(i);
        }
    }
    void update_chain(int i) {
        for (; i; i /= 2) {
            update_node(i);
        }
    }
    int cnt() {
        return s[1].val;
    }
    void do_update(int a, int b, int v) {
        assert(a <= b);
        if (a == b) return;
        for (int i = a+S, j = b+S; i < j; i /= 2, j /= 2) {
            if (i % 2 == 1) {
                s[i].lazy += v;
                update_node(i);
                i++;
            }
            if (j % 2 == 1) {
                j--;
                s[j].lazy += v;
                update_node(j);
            }
        }
        // update one outside
        if (a) update_chain(a+S-1);
        update_chain(b+S);
    }
    // inclusive/exclusive
    void mark(int a, int b) {
        do_update(a,b,1);
    }
    void unmark(int a, int b) {
        do_update(a,b,-1);
    }
} seg(0);

const int MAXN = int(2e5) + 200;
int N;
int V[MAXN];
vector<int> adj[MAXN];
int X[MAXN]; // xor of two vals
int depth[MAXN];
int par[MAXN][20];

int L = 0;
pair<int, int> B[MAXN]; // boundaries
void dfsB(int cur, int prv = -1) {
    if (prv == -1) {
        depth[cur] = 0;
    } else {
        depth[cur] = depth[prv] + 1;
    }
    memset(par[cur], -1, sizeof(par[cur]));
    par[cur][0] = prv;
    for (int i = 0; par[cur][i] != -1; i++) {
        par[cur][i+1] = par[par[cur][i]][i];
    }
    B[cur].first = L;
    L++;
    for (int nxt : adj[cur]) {
        if (nxt == prv) continue;
        dfsB(nxt, cur);
    }
    B[cur].second = L;
}

// is i a par of j
bool is_par(int i, int j) {
    assert(i != j);
    return B[i].first < B[j].first && B[j].first < B[i].second;
}

int get_par(int i, int cnt) {
    assert(i >= 0);
    assert(cnt >= 0);
    for (int k = 0; cnt; k ++, cnt /= 2) {
        if (cnt & 1) {
            i = par[i][k];
            assert(i >= 0);
        }
    }
    return i;
}

// i is the parent of j
int get_dir(int i, int j) {
    int di = depth[i], dj = depth[j];
    assert(di < dj);
    return get_par(j, dj-di-1);
}

long long res = 0;
void dfs2(int cur, int prv) {
    assert(0 <= prv && prv < N);
    int pprv = X[V[prv]] ^ prv;
    int pcur = X[V[cur]] ^ cur;
    assert(is_par(prv, cur));
    if (cur == pprv) assert(pcur == prv);
    if (cur == pprv || is_par(cur, pprv)) {
        //cerr << "unmark " << pprv << ": " << B[pprv].first << '-' << B[pprv].second << '\n';
        seg.unmark(B[pprv].first, B[pprv].second);
    }
    if (!(pcur == cur || is_par(cur, pcur))) {
        // pcur is outside cur
        if (is_par(pcur, cur)) {
            // remove the complement
            int pt = get_dir(pcur, cur);
            //cerr << "mark opp " << pt << ": " << B[pt].first << '-' << B[pt].second << '\n';
            seg.mark(0, B[pt].first);
            seg.mark(B[pt].second, N);
        } else {
            //cerr << "mark " << pcur << ": " << B[pcur].first << '-' << B[pcur].second << '\n';
            seg.mark(B[pcur].first, B[pcur].second);
        }
    }
    int val = seg.cnt();
    //cerr << cur << ' ' << val << '\n';
    res += val;
    for (int nxt : adj[cur]) {
        if (nxt == prv) continue;
        dfs2(nxt, cur);
    }
    assert(val == seg.cnt());
    if (cur == pprv || is_par(cur, pprv)) {
        seg.mark(B[pprv].first, B[pprv].second);
    }
    if (!(pcur == cur || is_par(cur, pcur))) {
        // pcur is outside cur
        if (is_par(pcur, cur)) {
            // remove the complement
            int pt = get_dir(pcur, cur);
            seg.unmark(0, B[pt].first);
            seg.unmark(B[pt].second, N);
        } else {
            seg.unmark(B[pcur].first, B[pcur].second);
        }
    }
}
int main() {
    scanf("%d", &N);
    assert(N % 2 == 0);
    for (int i = 1; i < N; i++) {
        int j; scanf("%d", &j); j--;
        adj[i].push_back(j);
        adj[j].push_back(i);
    }
    for (int i = 0; i < N; i++) {
        scanf("%d", &V[i]);
        V[i] --;
        X[V[i]] ^= i;
    }
    for (int v = 0; v < N / 2; v++) {
        assert(X[v]);
    }
    dfsB(0);
    seg = seg_tree(N);
    for (int i = 0; i < N; i++) {
        int j = X[V[i]] ^ i;
        assert(j != i);
        if (is_par(i, j)) {
            //cerr << j << ' ' << B[j].first << ' ' << B[j].second << '\n';
            seg.mark(B[j].first, B[j].second);
        }
    }
    res += seg.cnt();
    for (int nxt : adj[0]) {
        dfs2(nxt, 0);
    }
    cout << res << '\n';
    return 0;
}
