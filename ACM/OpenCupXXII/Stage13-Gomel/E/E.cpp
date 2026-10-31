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
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<std::pair<int, bool>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back({v, true});
			adj[v].push_back({u, false});
		}

		int64_t ans = 0;

		std::vector<bool> cur_edges; cur_edges.reserve(N);
		std::vector<std::array<int, 2>> cur_verts; cur_verts.reserve(N+1);
		auto solve_cur = [&]() -> std::array<int, 2> {
			int L = int(cur_edges.size());
			assert(int(cur_verts.size()) == L-1);
			int cnt = 0;
			for (int i = 0; i < L; i++) cnt += cur_edges[i];
			if (cnt == 0 || cnt == L) {
				// the whole thing is a cycle, anything can go to anything
				std::array<int, 2> res{0, 0};
				for (int i = 0; i < L-1; i++) {
					ans += int64_t(res[0]) * int64_t(cur_verts[i][1]);
					ans += int64_t(res[1]) * int64_t(cur_verts[i][0]);
					res[0] += cur_verts[i][0];
					res[1] += cur_verts[i][1];
				}
				return res;
			}
			std::array<int, 2> res{0, 0};
			{
				int a = 0;
				for (int i = 0; cur_edges[i] == cur_edges[0]; i++) {
					a = i;
					res[cur_edges[0]] += cur_verts[a][cur_edges[0]];
				}
				int b = L;
				for (int i = L-1; cur_edges[i] == cur_edges[L-1]; i--) {
					b = i-1;
					if (b == a) continue;
					res[!cur_edges[L-1]] += cur_verts[b][!cur_edges[L-1]];
				}
			}
			cur_verts.push_back({0, 0});

			assert(int(cur_verts.size()) == L);

			for (int i = 0; i < L; i++) {
				if (cur_edges[i] != cur_edges[(i+1)%L]) {
					// pick this to be the start
					std::rotate(cur_verts.begin(), cur_verts.begin() + i, cur_verts.end());
					std::rotate(cur_edges.begin(), cur_edges.begin() + (i+1), cur_edges.end());
					break;
				}
			}
			assert(cur_edges[0] != cur_edges[L-1]);
			cur_verts.push_back(cur_verts.front());
			for (int i = 0, j = 0; i < L; i = j) {
				int cur = cur_verts[i][!cur_edges[i]];
				for (j = i; j < L && cur_edges[j] == cur_edges[i]; j++) {
					ans += int64_t(cur) * cur_verts[j+1][cur_edges[i]];
					cur += cur_verts[j+1][!cur_edges[i]];
				}
			}

			int num_flips = 0;
			int flip_idx = -1;
			for (int i = 0; i+1 < L; i++) {
				if (cur_edges[i+1] != cur_edges[i]) {
					num_flips++;
					flip_idx = i+1;
				}
			}
			if (num_flips == 1) {
				// we have to subtract out duplicate paths
				ans -= int64_t(cur_verts[0][!cur_edges[0]]) * int64_t(cur_verts[flip_idx][cur_edges[0]]);
			}

			return res;
		};

		std::vector<int> depth(N, -1);
		std::vector<int> lowval(N, -1);
		std::vector<std::pair<int, bool>> stk; stk.reserve(M);
		std::vector<std::array<int, 2>> vert_stk; vert_stk.reserve(N+1);
		depth[0] = 0;
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			ans++;
			std::array<int, 2> cur_ways{1, 1};
			lowval[cur] = depth[cur];
			for (auto [nxt, d] : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					depth[nxt] = depth[cur] + 1;
					self(nxt, cur);
					lowval[cur] = std::min(lowval[cur], lowval[nxt]);
					stk.push_back({nxt, d});
					if (lowval[nxt] >= depth[cur]) {
						if (lowval[nxt] == depth[nxt]) {
							stk.pop_back();
							stk.push_back({cur, !d});
							stk.push_back({nxt, d});
						}
						int L = 2;
						while (stk.end()[-L].first != cur) {
							L++;
						}

						for (int i = int(stk.size()) - L; i < int(stk.size()); i++) {
							cur_edges.push_back(stk[i].second);
						}
						cur_verts.insert(cur_verts.end(), vert_stk.end() - (L-1), vert_stk.end());
						auto n = solve_cur();
						cur_edges.clear();
						cur_verts.clear();

						ans += int64_t(n[0]) * int64_t(cur_ways[1]);
						ans += int64_t(n[1]) * int64_t(cur_ways[0]);
						cur_ways[0] += n[0];
						cur_ways[1] += n[1];

						stk.resize(stk.size() - L);
						vert_stk.resize(vert_stk.size() - (L-1));
					}
				} else if (depth[nxt] < depth[cur]) {
					stk.push_back({nxt, d});
					lowval[cur] = std::min(lowval[cur], depth[nxt]);
				} else {
					// down-edge, just skip
					continue;
				}
			}
			vert_stk.push_back(cur_ways);
		})(0, -1);
		cout << ans << '\n';
	}

	return 0;
}
