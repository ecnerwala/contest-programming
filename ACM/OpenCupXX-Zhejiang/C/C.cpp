#include<bits/stdc++.h>
using namespace std;

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

void wssert(bool b) {
	if (!b) {
		cout << "GARBAGE" << '\n';
		exit(0);
	}
}

using ll = long long;

const int MAXN = 2.1e5;
int N, K;
vector<pair<int, ll>> adj[MAXN];

bool dead[MAXN];
int sz[MAXN];
int cnt[MAXN];
int curLCA[MAXN];
int tree[MAXN];

int par[MAXN][20];
int depth[MAXN];

bool inPath[MAXN];

ll solve() {
	ll ans = 0;
	for (int i = 1; i <= N; i++) dead[i] = false;

	auto getCentroid = [&](int p) -> int {
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (dead[cur]) {
				sz[cur] = 0;
				return;
			}
			sz[cur] = 1;
			for (auto it : adj[cur]) {
				int nxt = it.first;
				if (nxt == prv) continue;
				self(nxt, cur);
				sz[cur] += sz[nxt];
			}
		})(p, 0);
		int totSz = sz[p];
		while (true) {
			int nxtP = -1;
			for (auto it : adj[p]) {
				int n = it.first;
				if (sz[n] > sz[p]) continue;
				if (sz[n] * 2 > totSz) {
					nxtP = n;
				}
			}
			if (nxtP == -1) return p;
			p = nxtP;
		}
	};

	auto solvePt = [&](int pt) -> vector<int> {
		vector<pair<ll, int>> vals; vals.reserve(N);
		tree[pt] = pt;
		vals.emplace_back(0, pt); // self
		curLCA[pt] = 0;
		cnt[pt] = 0;
		depth[pt] = 1;
		memset(par[pt], 0, sizeof(par[pt]));

		for (auto e : adj[pt]) {
			cnt[e.first] = 0;
			curLCA[e.first] = 0;
			std::y_combinator([&](auto self, int cur, int prv, ll dist) -> void {
				tree[cur] = e.first;
				depth[cur] = depth[prv] + 1;
				memset(par[cur], 0, sizeof(par[cur]));
				par[cur][0] = prv;
				for (int i = 0; par[cur][i]; i++) {
					assert(depth[cur] - depth[par[cur][i]] == (1<<i));
					par[cur][i+1] = par[par[cur][i]][i];
				}
				vals.emplace_back(dist, cur);
				for (auto it : adj[cur]) {
					int nxt = it.first;
					if (nxt == prv) continue;
					self(nxt, cur, dist + it.second);
				}
			})(e.first, pt, e.second);
		}
		assert(int(vals.size()) == N);
		auto lca = [&](int a, int b) {
			wssert(1 <= a && a <= N);
			wssert(1 <= b && b <= N);
			if (depth[b] < depth[a]) swap(a, b);
			for (int d = depth[b] - depth[a], i = 0; d; i++, d /= 2) {
				if (d & 1) {
					b = par[b][i];
				}
			}
			assert(depth[b] == depth[a]);
			if (a == b) return a;
			int i = 0;
			while (par[a][i] != par[b][i]) i++;
			while (i) {
				--i;
				if (par[a][i] != par[b][i]) {
					a = par[a][i];
					b = par[b][i];
				}
			}
			return par[a][0];
		};

		ll curAns = 0;
		int curCnt = 0;
		sort(vals.begin(), vals.end());
		reverse(vals.begin(), vals.end());
		vector<int> nxtPt;
		for (auto it : vals) {
			ll v = it.first;
			int n = it.second;
			int t = tree[n];
			if (cnt[t] * 2 > K) {
				continue;
			}
			++cnt[t];
			if (cnt[t] * 2 > K) {
				nxtPt.push_back(t);
				continue;
			}

			if (curLCA[t] == 0) curLCA[t] = n;
			else curLCA[t] = lca(curLCA[t], n);

			if (nxtPt.size() == 2) {
				assert(curCnt == K-1);
				ans = max(ans, curAns + v);
				break;
			}
			curAns += v;
			curCnt++;
			if (curCnt == K) break;
		}

		if (curCnt == K) ans = max(ans, curAns);

		if (int(nxtPt.size()) == 2) {
			assert(K % 2 == 1);
			assert(curCnt == K-1);
			for (int i = 1; i <= N; i++) {
				inPath[i] = false;
			}
			for (int z = 0; z < 2; z++) {
				for (int cur = curLCA[nxtPt[z]]; cur; cur = par[cur][0]) {
					inPath[cur] = true;
				}
			}
			for (int z = 0; z < 2; z++) {
				for (int cur = par[curLCA[nxtPt[z]]][0]; cur; cur = par[cur][0]) {
					std::y_combinator([&](auto self, int cur, ll d) -> void {
						ans = max(ans, curAns + d);
						for (auto e : adj[cur]) {
							int nxt = e.first;
							if (nxt == par[cur][0]) continue;
							if (inPath[nxt]) continue;
							self(nxt, d + e.second);
						}
					})(cur, 0);
				}
			}
		}
		return nxtPt;
	};

	for (int pt = 1; pt <= N; pt++) {
		solvePt(pt);
	}
	/*
	for (int pt = 1; true; ) {
		pt = getCentroid(pt);
		dead[pt] = true;

		vector<int> nxtPt = solvePt(pt);
		if (nxtPt.empty()) break;
		else if (nxtPt.size() == 1) {
			pt = nxtPt[0];
			if (dead[pt]) break;
		} else if (nxtPt.size() == 2) {
			solvePt(nxtPt[0]);
			solvePt(nxtPt[1]);
			break;
		} else assert(false);
	}
	*/

	return ans * 2;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N >> K;
		for (int i = 0; i < N-1; i++) {
			int u, v; ll l; cin >> u >> v >> l;
			adj[u].emplace_back(v, l);
			adj[v].emplace_back(u, l);
		}

		ll ans = solve();
		cout << ans << '\n';

		for (int i = 1; i <= N; i++) {
			adj[i] = {};
		}
	}

	return 0;
}
