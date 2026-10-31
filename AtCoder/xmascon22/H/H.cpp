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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [x, y] : edges) {
			cin >> x >> y; x--, y--;
		}
		std::vector<std::vector<std::pair<int, int>>> tree_adj(N);
		for (int e = 0; e < M; e++) {
			auto [x, y] = edges[e];
			tree_adj[x].push_back({e, y});
			tree_adj[y].push_back({e, x});
		}

		std::vector<int> block_id(M);
		int V = N;
		std::vector<std::vector<int>> bc_adj(V); bc_adj.reserve(2*N);

		{
			std::vector<int> depth(N, -1);
			std::vector<int> lowval(N, -1);
			std::vector<std::pair<int, int>> stk; stk.reserve(N);
			depth[0] = 0;

			std::y_combinator([&](auto self, int cur, int prv_e) -> void {
				lowval[cur] = depth[cur];
				for (auto [e, nxt] : tree_adj[cur]){
					if (e == prv_e) continue;
					if (depth[nxt] == -1) {
						depth[nxt] = depth[cur] + 1;
						stk.push_back({e, nxt});
						self(nxt, e);
						lowval[cur] = std::min(lowval[cur], lowval[nxt]);
						if (lowval[nxt] >= depth[cur]) {
							// pop until e
							int block_num = V++;
							bc_adj.emplace_back();
							while (true) {
								auto [stk_e, stk_v] = stk.back(); stk.pop_back();
								block_id[stk_e] = block_num;
								//cerr << "stk_e " << stk_e << ' ' << block_num << '\n';
								if (stk_v != -1) {
									bc_adj[stk_v].push_back(block_num);
									bc_adj[block_num].push_back(stk_v);
									//cerr << "stk_v " << stk_v << ' ' << block_num << '\n';
								}
								if (stk_e == e) break;
							}
							bc_adj[block_num].push_back(cur);
							bc_adj[cur].push_back(block_num);
							//cerr << "stk_v " << cur << ' ' << block_num << '\n';
						}
					} else if (depth[nxt] < depth[cur]) {
						stk.push_back({e, -1});
						lowval[cur] = std::min(lowval[cur], depth[nxt]);
					}
				}
			})(0, -1);
		}

		int max_singles = N;

		std::vector<int> subtree_weight(V);
		int centroid;
		{
			std::y_combinator([&](auto self, int cur, int prv) -> void {
				subtree_weight[cur] = (cur < N);
				for (int nxt : bc_adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
					subtree_weight[cur] += subtree_weight[nxt];
				}
			})(0, -1);
			centroid = 0;
			int centroid_par = -1;
			while (true) {
				bool found = false;
				for (int nxt : bc_adj[centroid]) {
					if (nxt == centroid_par) continue;
					if (subtree_weight[nxt] * 2 >= N) {
						centroid_par = centroid;
						centroid = nxt;
						found = true;
						break;
					}
				}
				if (!found) break;
			}
			std::y_combinator([&](auto self, int cur, int prv) -> void {
				subtree_weight[cur] = (cur < N);
				for (int nxt : bc_adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
					subtree_weight[cur] += subtree_weight[nxt];
				}
			})(centroid, -1);
		}
		//cerr << "centroid " << centroid << '\n';

		std::vector<int> dist(N, -2);

		std::vector<int> q; q.reserve(V);

		std::vector<int> dp_st(V);
		std::vector<int> dp_en(V);
		auto dfs_dp = std::y_combinator([&](auto self, int cur, int prv) -> void {
			//cerr << "dfs " << cur << ' ' << prv << '\n';
			assert(cur < N);
			for (int b : bc_adj[cur]) {
				if (b == prv) continue;
				for (int nxt : bc_adj[b]) {
					if (nxt == cur) continue;
					self(nxt, b);
				}
			}
			dp_st[cur] = 0;
			dp_en[cur] = subtree_weight[cur];
			for (int b : bc_adj[cur]) {
				if (b == prv) continue;
				for (int nxt : bc_adj[b]) {
					if (nxt == cur) continue;
					dist[nxt] = -1;
				}
			}

			q.clear();
			dist[cur] = 0;
			q.push_back(cur);
			for (int z = 0; z < int(q.size()); z++) {
				int i = q[z];
				for (auto [_, j] : tree_adj[i]) {
					if (dist[j] == -1) {
						dist[j] = dist[i] + 1;
						q.push_back(j);
					}
				}
			}

			// use and clear them
			for (int b : bc_adj[cur]) {
				if (b == prv) continue;
				for (int nxt : bc_adj[b]) {
					if (nxt == cur) continue;
					//cerr << "dist from " << nxt << '-' << cur << ' ' << dist[nxt] << '\n';
					dp_st[nxt] += 2 * dist[nxt];
					dp_en[nxt] += 2 * dist[nxt];
					setmax(dp_st[cur], dp_st[nxt]);
					setmax(dp_en[cur], dp_en[nxt]);
					setmax(max_singles, dp_st[nxt] + (N - subtree_weight[b]));
				}
			}
			dist[cur] = -2;
			//cerr << "dp " << cur << ' ' << dp_st[cur] << ' ' << dp_en[cur] << '\n';
		});
		if (centroid < N) {
			dfs_dp(centroid, -1);
			int best_st = 0;
			int best_en = 0;
			for (int b : bc_adj[centroid]) {
				int b_best_st = 0;
				int b_best_en = 0;
				for (int nxt : bc_adj[b]) {
					if (nxt == centroid) continue;
					setmax(b_best_st, dp_st[nxt]);
					setmax(b_best_en, dp_en[nxt]);
				}
				setmax(max_singles, best_st + b_best_en);
				setmax(max_singles, best_en + b_best_st);
				setmax(best_st, b_best_st);
				setmax(best_en, b_best_en);
			}
		} else {
			for (int v : bc_adj[centroid]) {
				dfs_dp(v, centroid);
			}
			std::nth_element(bc_adj[centroid].begin(), bc_adj[centroid].begin(), bc_adj[centroid].end(), [&](auto a, auto b) -> bool {
				return subtree_weight[a] > subtree_weight[b];
			});
			for (int root_pt_idx = 0; root_pt_idx < 1 && root_pt_idx < int(bc_adj[centroid].size()); root_pt_idx++) {
				int st = bc_adj[centroid][root_pt_idx];
				for (int v : bc_adj[centroid]) {
					dist[v] = -1;
				}
				q.clear();
				dist[st] = 0;
				q.push_back(st);
				for (int z = 0; z < int(q.size()); z++) {
					int i = q[z];
					for (auto [_, j] : tree_adj[i]) {
						if (dist[j] == -1) {
							dist[j] = dist[i] + 1;
							q.push_back(j);
						}
					}
				}
				for (int v : bc_adj[centroid]) {
					if (v == st) continue;
					setmax(max_singles, dp_st[st] + 2 * dist[v] + dp_en[v]);
					setmax(max_singles, dp_en[st] + 2 * dist[v] + dp_st[v]);
				}
			}
		}

		max_singles -= N;
		//cerr << "max singles" << ' ' << max_singles << '\n';
		cout << (N - 1 - max_singles + 1) / 2 + max_singles << '\n';
	}

	return 0;
}
