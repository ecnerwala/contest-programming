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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, Q; cin >> N >> Q;

		struct edge_t {
			array<int, 2> V;
			int L;
			int64_t A;
		};
		vector<edge_t> edges(N-1);
		vector<vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int x, y, l; int64_t a; cin >> x >> y >> l >> a; x--, y--;
			edges[e].V = {x,y};
			edges[e].L = l;
			edges[e].A = a;
		}
		sort(edges.begin(), edges.end(), [&](const auto& a, const auto& b) { return a.L < b.L; });
		for (int e = 0; e < N-1; e++) {
			auto [x, y] = edges[e].V;
			adj[x].push_back(e);
			adj[y].push_back(e);
		}
		vector<array<int, 2>> bounds(N);
		{
			int cur_idx = 0;
			std::y_combinator([&](auto self, int cur) -> void {
				bounds[cur][0] = cur_idx;
				cur_idx++;
				for (int e : adj[cur]) {
					if (edges[e].V[0] != cur) swap(edges[e].V[0], edges[e].V[1]);
					int nxt = edges[e].V[1];
					adj[nxt].erase(std::find(adj[nxt].begin(), adj[nxt].end(), e));
					self(nxt);
				}
				bounds[cur][1] = cur_idx;
			})(0);
			assert(cur_idx == N);
		}

		vector<int64_t> ans(Q);
		vector<tuple<int, int, int>> queries(Q);
		for (int q = 0; q < Q; q++) {
			int c, w; cin >> c >> w; c--;
			queries[q] = {w, c, q};
		}
		sort(queries.begin(), queries.end());

		vector<int64_t> seg(2*N);

		auto it = edges.begin();
		for (auto [w, c, q] : queries) {
			while (it != edges.end() && w >= it->L) {
				auto v = it->V[1];
				for (int a = N+bounds[v][0], b = N+bounds[v][1]; a < b; a >>= 1, b >>= 1) {
					if (a & 1) {
						seg[a] = std::gcd(seg[a], it->A);
						a++;
					}
					if (b & 1) {
						--b;
						seg[b] = std::gcd(seg[b], it->A);
					}
				}
				++it;
			}

			int64_t res = 0;
			for (int a = N+bounds[c][0]; a; a >>= 1) {
				res = std::gcd(res, seg[a]);
			}
			ans[q] = res;
		}

		cout << "Case #" << case_num << ": ";
		for (int q = 0; q < Q; q++) {
			cout << ans[q] << " \n"[q+1==Q];
		}
	}

	return 0;
}
