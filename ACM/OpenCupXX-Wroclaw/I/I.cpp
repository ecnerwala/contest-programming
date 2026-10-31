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

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		vector<vector<pair<int, i64>>> adj(N);
		for (int e = 0; e < M; e++) {
			int a, b; i64 c; cin >> a >> b >> c; a--, b--;
			adj[a].emplace_back(b, c);
		}

		vector<i64> P(N, -1);
		vector<int> par(N, -1);
		{ // dikjstra
			priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;
			pq.emplace(P[0] = 0, 0);
			while (!pq.empty()) {
				auto [d, cur] = pq.top(); pq.pop();
				if (d > P[cur]) continue;
				for (auto [nxt, w] : adj[cur]) {
					if (P[nxt] == -1 || w + d < P[nxt]) {
						P[nxt] = w + d;
						par[nxt] = cur;
						pq.emplace(P[nxt], nxt);
					}
				}
			}
		}

		vector<vector<int>> ch(N);
		for (int i = 1; i < N; i++) ch[par[i]].push_back(i);

		vector<int> sz(N);
		vector<int> depth(N);
		depth[0] = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			sz[cur] = 1;
			for (int nxt : ch[cur]) {
				depth[nxt] = depth[cur] + 1;
				self(nxt);
				sz[cur] += sz[nxt];
			}
		})(0);

		vector<int> st(N);
		vector<int> en(N);
		vector<int> heavyTop(N);
		{ // build the HLD
			int idx = 0;
			heavyTop[0] = 0;
			std::y_combinator([&](auto self, int cur) -> void {
				st[cur] = idx++;
				if (!ch[cur].empty()) {
					swap(*ch[cur].begin(), *max_element(ch[cur].begin(), ch[cur].end(), [&](int a, int b) { return sz[a] < sz[b]; }));
					bool first = true;
					for (int nxt : ch[cur]) {
						if (first) {
							heavyTop[nxt] = heavyTop[cur];
							first = false;
						} else {
							heavyTop[nxt] = nxt;
						}
						self(nxt);
					}
				}
				en[cur] = idx;
			})(0);
			assert(idx == N);
		}
		auto lca = [&](int x, int y) -> int {
			while (heavyTop[x] != heavyTop[y]) {
				if (depth[heavyTop[x]] > depth[heavyTop[y]]) {
					swap(x, y);
				}
				y = par[heavyTop[y]];
			}
			assert(heavyTop[x] == heavyTop[y]);
			if (depth[x] > depth[y]) swap(x, y);
			return x;
		};

		vector<vector<pair<int, pair<int, i64>>>> cndEdges(N);

		for (int i = 0; i < N; i++) {
			for (auto [j, w] : adj[i]) {
				assert(w >= P[j] - P[i]);
				w -= P[j] - P[i];
				if (i == par[j]) {
					// this is a tree edge
					assert(w == 0);
					continue;
				}
				int k = lca(i, j);
				cndEdges[st[i]].emplace_back(st[k], pair<int, i64>(j ,w));
				cndEdges[st[j]].emplace_back(st[k], pair<int, i64>(j, w));
			}
		}

		for (int i = 0; i < N; i++) {
			sort(cndEdges[i].begin(), cndEdges[i].end(), std::greater<>());
		}

		int S = 1;
		while (S < N) S *= 2;
		vector<int> seg(2*S); // earliest that any edge from this subtree goes
		auto updateNode = [&](int a) {
			if (a >= S) {
				if (a-S < N && !cndEdges[a-S].empty()) {
					seg[a] = cndEdges[a-S].back().first;
				} else {
					seg[a] = N;
				}
			} else {
				seg[a] = min(seg[2*a], seg[2*a+1]);
			}
		};
		for (int a = 2*S-1; a; a--) updateNode(a);

		vector<i64> D2(N, -1);
		priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq2;

		D2[0] = 0;
		pq2.emplace(D2[0], 0);

		while (!pq2.empty()) {
			auto [_d, _cur] = pq2.top(); pq2.pop();
			i64 d = _d; int cur = _cur; // not sure why this is necessary
			if (d > D2[cur]) continue;

			auto relaxEdge = [&](int nxt, i64 w) {
				if (D2[nxt] == -1 || w + d < D2[nxt]) {
					D2[nxt] = w + d;
					pq2.emplace(D2[nxt], nxt);
				}
			};

			auto searchSubSeg = std::y_combinator([&](auto self, int a) -> void {
				if (seg[a] > st[cur]) {
					return;
				}
				if (a >= S) {
					assert(a-S < N);
					auto& cnds = cndEdges[a-S];
					while (!cnds.empty() && cnds.back().first <= st[cur]) {
						auto [nxt, w] = cnds.back().second;
						relaxEdge(nxt, w);
						cnds.pop_back();
					}
				} else {
					self(2*a);
					self(2*a+1);
				}
				updateNode(a);
			});

			for (int a = S+st[cur], b = S+en[cur]; a < b; a /= 2, b /= 2) {
				if (a & 1) searchSubSeg(a++);
				if (b & 1) searchSubSeg(--b);
			}
		}
		for (int i = 1; i < N; i++) {
			cout << (D2[i] == -1 ? -1 : P[i] * 2 + D2[i]) << " \n"[i+1==N];
		}
	}

	return 0;
}
