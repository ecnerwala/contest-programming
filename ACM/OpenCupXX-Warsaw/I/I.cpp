#include<bits/stdc++.h>
using namespace std;

const int MAXV = 2 * int(1.1e5);
int V;
vector<int> adj[MAXV];

namespace circles {

int par[MAXV][20];
int depth[MAXV];
void dfsPar(int cur, int prv) {
	depth[cur] = depth[prv] + 1;
	par[cur][0] = prv;
	for (int i = 0; par[cur][i]; i++) {
		par[cur][i+1] = par[par[cur][i]][i];
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfsPar(nxt, cur);
	}
}

int getAnc(int a, int l) {
	for (int i = 0; (1<<i) <= l; i++) {
		if ((1<<i) & l) {
			a = par[a][i];
		}
	}
	return a;
}

int lca(int a, int b) {
	assert(1 <= a && a <= V);
	assert(1 <= b && b <= V);

	if (depth[a] > depth[b]) swap(a,b);
	assert(depth[b] >= depth[a]);
	b = getAnc(b, depth[b] - depth[a]);

	if (a == b) return a;
	assert(a != b);
	int i = 0;
	while (par[a][i] != par[b][i]) i++;
	assert(par[a][i] == par[b][i]);
	while (i) {
		i--;
		if (par[a][i] != par[b][i]) {
			a = par[a][i], b = par[b][i];
		}
		assert(a != b);
		assert(par[a][i] == par[b][i]);
	}
	return par[a][i];
}

struct circle {
	int v;
	int r;
	friend bool operator == (const circle& a, const circle& b) {
		return tie(a.v, a.r) == tie(b.v, b.r);
	}
	friend ostream& operator << (ostream& o, const circle& c) {
		return o << "{" << "v:" << c.v << " " "r:" << c.r << "}";
	}
};

const circle EMPTY = {0, -1};
const circle FULL = {1, MAXV};
circle intersect(circle a, circle b) {
	if (a == EMPTY || b == EMPTY) return EMPTY;
	assert((depth[a.v] & 1) != (a.r & 1));
	assert((depth[b.v] & 1) != (b.r & 1));

	int mid = lca(a.v, b.v);
	int d = depth[a.v] - depth[mid] + depth[b.v] - depth[mid];
	assert(d % 2 == (a.r + b.r) % 2);
	if (d > a.r + b.r) {
		return EMPTY;
	}
	if (a.r >= b.r + d) {
		return b;
	}
	if (b.r >= a.r + d) {
		return a;
	}

	assert(abs(a.r - b.r) < d);
	int da = (a.r - b.r + d) / 2;
	int db = (b.r - a.r + d) / 2;
	int r = (a.r + b.r - d) / 2;
	assert(da + r == a.r);
	assert(db + r == b.r);
	assert(da + db == d);

	circle res;
	res.v = (da <= (depth[a.v] - depth[mid]) ? getAnc(a.v, da) : getAnc(b.v, db));
	res.r = (a.r + b.r - d) / 2;

	return res;
}

void precomp() {
	dfsPar(1, 0);
}

} // namespace circles

const int MAXQ = 3.1e5;
int Q;
int ans[MAXQ];
vector<pair<int, pair<int, int>>> queries[MAXV]; // radius, location, weight

namespace centroid {

bool hasWeight(int vert) {
	return vert <= (V+1)/2;
}

bool alive[MAXV];

int sz[MAXV];
void dfsSz(int cur, int prv) {
	assert(alive[cur]);
	sz[cur] = 1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (!alive[nxt]) continue;

		dfsSz(nxt, cur);
		sz[cur] += sz[nxt];
	}
}

int getCentroid(int cur, int prv, int z) {
	assert(alive[cur]);
	if (sz[cur] * 2 < z) {
		return -1;
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (!alive[nxt]) continue;
		int v = getCentroid(nxt, cur, z);
		if (v != -1) return v;
	}
	return cur;
}

void addDepth(int cur, int prv, int curDepth, vector<int>& totDepth, vector<int>& chDepth) {
	assert(alive[cur]);
	if (hasWeight(cur)) {
		// it's good
		totDepth[curDepth] ++;
		chDepth[curDepth] ++;
	}

	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (!alive[nxt]) continue;
		addDepth(nxt, cur, curDepth+1, totDepth, chDepth);
	}
}

int getPrefSum(const vector<int>& s, int d) {
	if (s.empty()) return 0;
	if (d >= int(s.size())) return s.back();
	return s[d];
}

void doPrefSum(vector<int>& d) {
	for (int i = 1; i < int(d.size()); i++) {
		d[i] += d[i-1];
	}
}

void solveDepth(int cur, int prv, int curDepth, const vector<int>& totDepth, const vector<int>& chDepth) {
	assert(alive[cur]);
	for (auto q : queries[cur]) {
		int r = q.first - curDepth;
		if (r < 0) {
			continue;
		}
		ans[q.second.first] += (getPrefSum(totDepth, r) - getPrefSum(chDepth, r)) * q.second.second;
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (!alive[nxt]) continue;
		solveDepth(nxt, cur, curDepth+1, totDepth, chDepth);
	}
}

void solveCentroid(int root) {
	dfsSz(root, 0);
	root = getCentroid(root, 0, sz[root]);
	dfsSz(root, 0);
	vector<int> totDepth(sz[root]+1);
	if (hasWeight(root)) {
		totDepth[0] ++;
	}
	vector<pair<int, vector<int>>> chDepths;
	for (int ch : adj[root]) {
		if (!alive[ch]) continue;
		chDepths.emplace_back(ch, vector<int>(sz[ch] + 1));
		addDepth(ch, root, 1, totDepth, chDepths.back().second);
		doPrefSum(chDepths.back().second);
	}
	doPrefSum(totDepth);
	for (auto q : queries[root]) {
		ans[q.second.first] += getPrefSum(totDepth, q.first) * q.second.second;
	}
	for (const auto& it : chDepths) {
		int ch = it.first;
		assert(alive[ch]);
		solveDepth(ch, root, 1, totDepth, it.second);
	}

	alive[root] = false;
	for (int ch : adj[root]) {
		if (!alive[ch]) continue;
		solveCentroid(ch);
	}
}

void solve() {
	for (int i = 1; i <= V; i++) {
		alive[i] = true;
	}
	solveCentroid(1);
}

} // namespace centroid

void do_input() {
	int N; cin >> N;
	V = N + (N-1);
	for (int e = N+1; e <= V; e++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(e);
		adj[v].push_back(e);
		adj[e].push_back(u);
		adj[e].push_back(v);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	do_input();
	circles::precomp();
	cin >> Q;
	for (int q = 0; q < Q; q++) {
		int K; cin >> K;
		vector<circles::circle> circs(K);
		for (int i = 0; i < K; i++) {
			cin >> circs[i].v >> circs[i].r; circs[i].r *= 2;
		}
		vector<circles::circle> prefs(K+1);
		vector<circles::circle> suffs(K+1);
		prefs[0] = circles::FULL;
		for (int i = 0; i < K; i++) {
			prefs[i+1] = circles::intersect(prefs[i], circs[i]);
		}
		suffs[K] = circles::FULL;
		for (int i = K-1; i >= 0; i--) {
			suffs[i] = circles::intersect(suffs[i+1], circs[i]);
		}
		for (int i = 0; i < K; i++) {
			auto cur = circles::intersect(prefs[i], suffs[i+1]);
			//cerr << "add " << cur << '\n';
			if (cur == circles::EMPTY) {
				// do nothing
			} else {
				queries[cur.v].emplace_back(cur.r, pair<int, int>({q, 1}));
			}
		}
		//cerr << "remove " << prefs[K] << '\n';
		auto cur = prefs[K];
		if (cur == circles::EMPTY) {
			// do nothing
		} else {
			queries[cur.v].emplace_back(cur.r, pair<int, int>({q, -(K-1)}));
		}
	}

	centroid::solve();

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
