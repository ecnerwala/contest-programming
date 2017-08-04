#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1e6;
int N;
vector<int> adj[MAXN];
const int MAXM = 2e6;
int M;
pii E[MAXM];

int V;
bool on_stack[MAXN];
int ind[MAXN];
int lowlink[MAXN];
stack<int> stk;

int scc[MAXN];
bool is_alone[MAXN];

void dfs(int cur) {
    ind[cur] = lowlink[cur] = V++;
    stk.push(cur); on_stack[cur] = true;

    for (int nxt : adj[cur]) {
        if (ind[nxt] == -1) {
            dfs(nxt);
            lowlink[cur] = min(lowlink[cur], lowlink[nxt]);
        } else if (on_stack[nxt]) {
            lowlink[cur] = min(lowlink[cur], ind[nxt]);
        }
    }

    if (lowlink[cur] == ind[cur]) {
        int cnt = 0;
        while (true) {
            int val = stk.top(); stk.pop();
            on_stack[val] = false;
            scc[val] = cur;
            cnt++;
            if (val == cur) break;
        }
        assert(cnt);
        is_alone[cur] = (cnt == 1);
    }
}

const int MAXS = 300;
bool con[MAXS][MAXS];
void slow() {
    if (!(N < MAXS)) {
        return;
    }
    memset(con, false, sizeof(con));
    for (int i = 1; i <= M; i++) {
        con[E[i].A][E[i].B] = true;
    }
    for (int i = 1; i <= N; i++) con[i][i] = true;
    for (int k = 1; k <= N; k++) {
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                if (con[i][k] && con[k][j]) con[i][j] = true;
            }
        }
    }
    for (int i = 1; i <= N; i++) {
        assert(con[1][i] == (ind[i] != -1));
        if (ind[i] == -1) continue;
        int cnt = 0;
        for (int j = 1; j <= N; j++) {
            //cerr << i << ' ' << j << '\n';
            assert((con[i][j] && con[j][i]) == (scc[i] == scc[j]));
            cnt += (con[i][j] && con[j][i]);
        }
        if (scc[i] == i) {
            if (is_alone[i]) {
                assert(cnt == 1);
            }
        }
    }
}

int indeg[MAXN];
int topo[MAXN];
vector<int> toposort() {
    for (int i = 1; i <= M; i++) {
        if (scc[E[i].A] == -1 || scc[E[i].B] == -1) continue;
        if (scc[E[i].A] == scc[E[i].B]) continue;
        adj[scc[E[i].B]].push_back(scc[E[i].A]);
    }
    memset(indeg, 0, sizeof(indeg));
    for (int i = 1; i <= N; i++) {
        for (int j : adj[i]) {
            indeg[j] ++;
        }
    }
    vector<int> q;
    for (int i = 1; i <= N; i++) {
        if (scc[i] != i) continue;
        if (i == scc[N]) continue;
        if (indeg[i] == 0) q.push_back(i);
    }
    for (int i = 0; i < int(q.size()); i++) {
        int cur = q[i];
        //cerr << "clear " << cur << '\n';
        scc[cur] = -1;
        for (int nxt : adj[cur]) {
            assert(scc[nxt] == nxt);
            assert(indeg[nxt]);
            indeg[nxt]--;
            if (!indeg[nxt]) {
                if (nxt == scc[N]) continue;
                q.push_back(nxt);
            }
        }
    }
    // propogate the -1
    for (int i = 1; i <= N; i++) {
        if (scc[i] == -1) continue;
        scc[i] = scc[scc[i]];
    }
    assert(scc[N] != -1);
    for (int i = 1; i<= N; i++) {
        adj[i].clear();
    }
    for (int i = 1; i <= M; i++) {
        if (scc[E[i].A] == -1 || scc[E[i].B] == -1) continue;
        if (scc[E[i].A] == scc[E[i].B]) continue;
        adj[scc[E[i].B]].push_back(scc[E[i].A]);
    }
    memset(indeg, 0, sizeof(indeg));
    for (int i = 1; i <= N; i++) {
        for (int j : adj[i]) {
            indeg[j] ++;
        }
    }
    q.clear();
    for (int i = 1; i <= N; i++) {
        if (scc[i] != i) continue;
        if (indeg[i] == 0) q.push_back(i);
    }
    assert(q.size() == 1);
    assert(q[0] == scc[N]);
    vector<int> res;
    set<int> seen;
    seen.insert(q[0]);
    for (int i = 0; i < int(q.size()); i++) {
        int cur = q[i];
        assert(scc[cur] == cur);
        if (int(seen.size())-1 == i) {
            if (is_alone[cur]) {
                res.push_back(cur);
            }
        }
        for (int nxt : adj[cur]) {
            assert(scc[nxt] == nxt);
            assert(indeg[nxt]);
            indeg[nxt]--;
            seen.insert(nxt);
            if (!indeg[nxt]) {
                q.push_back(nxt);
            }
        }
    }
    reverse(res.begin(), res.end());
    return res;
}

int mt[MAXN];

vector<int> go() {
    for (int i = 1; i <= M; i++) {
        adj[E[i].A].push_back(E[i].B);
    }
    V = 1;
    memset(on_stack, false, sizeof(on_stack));
    memset(ind, -1, sizeof(ind));
    memset(lowlink, -1, sizeof(lowlink));
    memset(scc, -1, sizeof(scc));
    dfs(1);
    assert(stk.empty());
    for (int i = 1; i <= M; i++) {
        if (E[i].A == E[i].B) {
            is_alone[E[i].A] = false;
        }
    }
    //for (int i = 1; i <= N; i++) cerr << i << ' ' << scc[i] << '\n'; cerr << '\n';
    slow();
    for (int i = 1; i <= N; i++) adj[i].clear();
    if (ind[N] == -1) {
        return vector<int>();
    }
    return toposort();
}

int main() {
    ios_base::sync_with_stdio(0);
    int T; cin >> T;
    for (int t = 0; t < T; t++) {
        cin >> N >> M;
        for (int i = 1; i <= M; i++) {
            cin >> E[i].A >> E[i].B;
        }
        //cerr << N << ',' << M << '\n';
        //for (int i = 1; i <= M; i++) { cerr << E[i].A  << "->" << E[i].B << '\n'; }

        vector<int> res = go();
        cout << res.size() << '\n';
        for (int i = 0; i < int(res.size()); i++) {
            if (i) cout << ' ';
            cout << res[i];
        }
        cout << '\n';

        //cerr << '\n' << '\n';

        for (int i = 1; i <= N; i++) adj[i].clear();
    }
}
