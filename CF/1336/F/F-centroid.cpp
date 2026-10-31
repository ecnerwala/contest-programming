#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;

template <typename K, typename V, typename Comp = std::less<K>>
using order_statistic_map = __gnu_pbds::tree<
	K, V, Comp,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update
>;

template <typename K, typename Comp = std::less<K>>
using order_statistic_set = order_statistic_map<K, __gnu_pbds::null_type, Comp>;

// Supports
//  auto iterator = order_statistic_set().find_by_order(int); // (0-indexed)
//  int num_strictly_smaller = order_statistic_set().order_of_key(key);

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

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N, M, K; cin >> N >> M >> K;
	vector<vector<int>> adj(N+1);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<pair<int, int>> allPaths(M);
	for (int j = 0; j < M; j++) {
		int s, t; cin >> s >> t;
		allPaths[j] = {s,t};
	}

	int64_t ans = 0;

	vector<int> sz(N+1);
	vector<int> st(N+1);
	vector<int> en(N+1);
	vector<int> subRoot(N+1);
	vector<int> depth(N+1);
	vector<array<int, 20>> par(N+1);
	vector<vector<pair<int, int>>> chPaths(N+1);
	vector<int> smallVal(N+1);
	vector<vector<int>> pathEnd(N+1);

	vector<int> preorder; preorder.reserve(N+1);

	std::y_combinator([&](auto processCentroid, int centroid, vector<pair<int, int>> paths) -> void {
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			sz[cur] = 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				sz[cur] += sz[nxt];
			}
		})(centroid, 0);

		int totSz = sz[centroid];
		centroid = std::y_combinator([&](auto self, int cur, int prv) -> int {
			assert(sz[cur] * 2 >= totSz);
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (sz[nxt] * 2 >= totSz) return self(nxt, cur);
			}
			return cur;
		})(centroid, 0);

		preorder.clear();
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			st[cur] = int(preorder.size());
			preorder.push_back(cur);

			depth[cur] = depth[prv] + 1;
			fill(par[cur].begin(), par[cur].end(), 0);
			par[cur][0] = prv;
			for (int i = 0; par[cur][i]; i++) {
				par[cur][i+1] = par[par[cur][i]][i];
			}

			if (cur == centroid) subRoot[cur] = 0;
			else if (prv == centroid) subRoot[cur] = cur;
			else subRoot[cur] = subRoot[prv];

			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}

			en[cur] = int(preorder.size());
		})(centroid, 0);

		auto getAnc = [&](int cur, int k) -> int {
			assert(k >= 0);
			while (k) {
				int i = __builtin_ctz(k);
				cur = par[cur][i];
				k &= (k-1);
			}
			return cur;

		};
		auto lca = [&](int a, int b) -> int {
			if (depth[a] > depth[b]) swap(a, b);
			b = getAnc(b, depth[b] - depth[a]);
			if (a == b) return a;
			int i = 0;
			while (par[a][i] != par[b][i]) i++;
			while (i) {
				i--;
				if (par[a][i] != par[b][i]) {
					a = par[a][i];
					b = par[b][i];
				}
			}
			return par[a][0];
		};

		for (int i : preorder) {
			smallVal[i] = 0;
			pathEnd[i] = {};
			chPaths[i] = {};
		}

		for (auto it : paths) {
			int s = it.first, t = it.second;
			if (s != centroid && t != centroid && subRoot[s] == subRoot[t]) {
				chPaths[subRoot[s]].emplace_back(s, t);
				int c = lca(s, t);
				if (depth[s] - depth[c] >= K) {
					smallVal[getAnc(s, depth[s] - depth[c] - K)]++;
				}
				if (depth[t] - depth[c] >= K) {
					smallVal[getAnc(t, depth[t] - depth[c] - K)]++;
				}
			} else {
				pathEnd[s].push_back(t);
				pathEnd[t].push_back(s);
			}
		}

		int idx = 0;
		for (int ch : adj[centroid]) {
			std::y_combinator([&](auto self, int cur, int prv) -> order_statistic_set<pair<int, int>> {
				order_statistic_set<pair<int, int>> res;
				auto updateFromVal = [&](int dest) -> void {
					assert(!(st[ch] <= st[dest] && st[dest] < en[ch]));
					if (depth[cur] - depth[centroid] >= K) {
						// take everything with st[dest] < st[ch] or subRoot[dest] != subRoot[other]
						if (st[dest] < st[ch]) {
							ans += int(res.size());
						} else {
							ans += int(res.size()) - (int(res.order_of_key({en[subRoot[dest]], -1})) - int(res.order_of_key({st[subRoot[dest]], -1})));
						}
					} else {
						if (st[dest] < st[ch]) {
							if (depth[cur] - depth[centroid] + depth[dest] - depth[centroid] >= K) {
								int destAnc = getAnc(dest, (depth[cur] - depth[centroid] + depth[dest] - depth[centroid]) - K);
								ans += int(res.order_of_key({en[destAnc], -1})) - int(res.order_of_key({st[destAnc], -1}));
							}
						}
					}
				};
				for (int i : pathEnd[cur]) {
					pair<int, int> key(st[i], idx++);
					updateFromVal(i);
					res.insert(key);
				}
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					auto o = self(nxt, cur);
					if (o.size() > res.size()) res.swap(o);
					for (auto i : o) {
						updateFromVal(preorder[i.first]);
					}
					for (auto i : o) {
						res.insert(i);
					}
				}

				ans += int64_t(res.size()) * smallVal[cur];

				return res;
			})(ch, centroid);
		}

		for (int ch : adj[centroid]) {
			adj[ch].erase(find(adj[ch].begin(), adj[ch].end(), centroid));
			processCentroid(ch, std::move(chPaths[ch]));
		}
	})(1, std::move(allPaths));

	cout << ans << '\n';

	return 0;
}
