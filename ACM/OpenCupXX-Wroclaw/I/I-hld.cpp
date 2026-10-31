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

		int S = 1;
		while (S < N) S *= 2;
		vector<vector<pair<int, i64>>> seg(2*S);

		for (int i = 0; i < N; i++) {
			for (auto [j, w] : adj[i]) {
				assert(w >= P[j] - P[i]);
				w -= P[j] - P[i];
				if (i == par[j]) {
					// this is a tree edge
					assert(w == 0);
					continue;
				}
				int x = i, y = j;
				while (heavyTop[x] != heavyTop[y]) {
					if (depth[heavyTop[x]] > depth[heavyTop[y]]) {
						swap(x, y);
					}
					assert(depth[heavyTop[x]] <= depth[heavyTop[y]]);
					for (int a = S+st[heavyTop[y]], b = S+st[y]+1; a < b; a /= 2, b /= 2) {
						if (a & 1) seg[a++].emplace_back(j, w);
						if (b & 1) seg[--b].emplace_back(j, w);
					}
					y = par[heavyTop[y]];
				}
				assert(heavyTop[x] == heavyTop[y]);
				if (depth[x] > depth[y]) swap(x, y);
				for (int a = S+st[x], b = S+st[y]+1; a < b; a /= 2, b /= 2) {
					if (a & 1) seg[a++].emplace_back(j, w);
					if (b & 1) seg[--b].emplace_back(j, w);
				}
			}
		}

		vector<i64> D2(N, -1);
		priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq2;
		D2[0] = 0;
		pq2.emplace(D2[0], 0);
		while (!pq2.empty()) {
			auto [d, cur] = pq2.top(); pq2.pop();
			if (d > D2[cur]) continue;

			for (int a = S+st[cur]; a; a /= 2) {
				for (auto [nxt, w] : seg[a]) {
					if (D2[nxt] == -1 || w + d < D2[nxt]) {
						D2[nxt] = w + d;
						pq2.emplace(D2[nxt], nxt);
					}
				}
				seg[a].clear();
			}
		}
		for (int i = 1; i < N; i++) {
			cout << (D2[i] == -1 ? -1 : P[i] * 2 + D2[i]) << " \n"[i+1==N];
		}
	}

	return 0;
}
