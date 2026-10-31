#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 8.1e5;
int N;
vector<int> adj[MAXN];

int st[MAXN];
int en[MAXN];
int par[MAXN][30];
int depth[MAXN];

void dfs(int cur, int prv, int& c) {
	depth[cur] = depth[prv] + 1;
	par[cur][0] = prv;
	for (int i = 0; par[cur][i]; i++) {
		par[cur][i+1] = par[par[cur][i]][i];
	}

	st[cur] = c;
	c++;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur, c);
	}
	en[cur] = c;
}

int anc(int cur, int d) {
	for (int i = 0; (1<<i) <= d; i++) {
		if (d & (1<<i)) {
			cur = par[cur][i];
		}
	}
	return cur;
}

const int S = 1 << 20;
struct {
	int val;
	int sum;
} seg[2 * S];

ll go() {
	cin >> N;
	map<string, vector<int>> mp;
	for (int i = 1; i <= N; i++) {
		string a; cin >> a;
		if (a == "-") continue;
		mp[a].push_back(i);
	}
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	{
		int c = 0;
		dfs(1, 0, c);
		assert(c == N);
	}

	vector<vector<pair<int, int>>> inserts(N+1);
	vector<vector<pair<int, int>>> removes(N+1);

	for (const auto& it : mp) {
		const vector<int>& v = it.second;
		assert(v.size() <= 2);
		if (v.size() < 2) continue;
		int a = v[0], b = v[1];
		assert(a != b);
		assert(st[a] != st[b]);
		if (st[a] > st[b]) {
			swap(a,b);
		}
		assert(st[a] < st[b]);

		//cerr << a << ":" << st[a] << "-" << en[a] << " " << b << ":" << st[b] << "-" << en[b] << '\n';

		if (en[a] > st[b]) {
			assert(st[a] < st[b] && st[b] < en[b] && en[b] <= en[a]);
			assert(en[a] >= en[b]);

			int c = anc(b, depth[b] - depth[a] - 1);
			//cerr << "c = " << c << '\n';
			assert(par[c][0] == a);
			assert(st[c] <= st[b] && en[b] <= en[c]);
			// a includes b
			inserts[0].emplace_back(st[b], en[b]);
			removes[st[c]].emplace_back(st[b], en[b]);
			inserts[en[c]].emplace_back(st[b], en[b]);
			removes[N].emplace_back(st[b], en[b]);

			inserts[st[b]].emplace_back(0, st[c]);
			removes[en[b]].emplace_back(0, st[c]);

			inserts[st[b]].emplace_back(en[c], N);
			removes[en[b]].emplace_back(en[c], N);
		} else {
			// they're disjoint
			inserts[st[a]].emplace_back(st[b], en[b]);
			removes[en[a]].emplace_back(st[b], en[b]);
			inserts[st[b]].emplace_back(st[a], en[a]);
			removes[en[b]].emplace_back(st[a], en[a]);
		}
	}

	auto pullUp = [&](int i) {
		assert(seg[i].val >= 0);
		if (seg[i].val) {
			seg[i].sum = 0;
		} else {
			if (i >= S) seg[i].sum = (i - S < N);
			else seg[i].sum = seg[2 * i].sum + seg[2 * i + 1].sum;
		}
	};

	for (int i = 2 * S - 1; i > 0; i--) {
		seg[i].val = 0;
		pullUp(i);
	}

	auto delta = [&](pair<int, int> range, int v) {
		int i = range.first, j = range.second;
		assert(i <= j);
		if (i == j) return;
		for (int a = i+S, b = j+S; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				seg[a].val += v;
				pullUp(a);
				++a;
			}
			if (b & 1) {
				--b;
				seg[b].val += v;
				pullUp(b);
			}
		}
		for (int a = i+S; a; a /= 2) {
			pullUp(a);
		}
		for (int b = j+S-1; b; b /= 2) {
			pullUp(b);
		}
	};

	ll ans = 0;
	for (int i = 0; i <= N; i++) {
		//cerr << "col = " << i << '\n';
		for (auto it : inserts[i]) {
			//cerr << "ins " << it.first << ' ' << it.second << '\n';
			delta(it, +1);
		}
		for (auto it : removes[i]) {
			//cerr << "rem " << it.first << ' ' << it.second << '\n';
			delta(it, -1);
		}
		//cerr << seg[1].sum << '\n';
		if (i < N) ans += seg[1].sum;
	}
	assert(seg[1].sum == N);
	return ans - N;
}

void cleanup() {
	for (int i = 1; i <= N; i++) {
		adj[i] = {};
		for (int z = 0; z < 30; z++) {
			par[i][z] = 0;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cerr << "Case #" << t << ":" << '\n';
		ll ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
		cleanup();
	}

	return 0;
}
