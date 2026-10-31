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

	int N, M; cin >> N >> M;

	std::vector<int> T(N);
	for (auto& t : T) cin >> t;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	int K_edge_cnt; cin >> K_edge_cnt;
	std::vector<int> K_deg(N);
	std::vector<std::array<int, 2>> K_edges(K_edge_cnt);
	for (auto& [u, v] : K_edges) {
		cin >> u >> v;
		K_deg[u] ++;
		K_deg[v] ++;
	}
	int V_K = 0;
	std::vector<int> K_id(N, -1);
	std::vector<int> K_verts; K_verts.reserve(9);
	for (int i = 0; i < N; i++) {
		if (K_deg[i] > 1) {
			K_id[i] = V_K++;
			K_verts.push_back(i);
		}
	}
	// silly edge case: no interior vertices
	for (auto [u, v] : K_edges) {
		if (K_id[u] == -1 && K_id[v] == -1) {
			assert(K_deg[u] == 1 && K_deg[v] == 1);
			K_id[u] = V_K++;
			K_verts.push_back(u);
		}
	}

	std::vector<int> K_adj(N);
	for (auto [u, v] : K_edges) {
		if (K_id[u] != -1) K_adj[v] |= (1 << K_id[u]);
		if (K_id[v] != -1) K_adj[u] |= (1 << K_id[v]);
	}

	std::vector<bool> is_leaf(N);
	std::vector<bool> is_cactus(N);
	{
		std::vector<int> depth(N, -1);
		depth[0] = 0;
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			int deg = prv != -1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					depth[nxt] = depth[cur] + 1;
					deg++;
					self(nxt, cur);
				} else if (depth[nxt] < depth[cur]) {
					is_cactus[cur] = true;
				} else {
				}
			}
			if (deg == 1) {
				is_leaf[cur] = true;
			}
		})(0, -1);
	}

	std::vector<int> depth(N, -1);
	std::vector<int> lowval(N);

	using bs = bitset<500>;

	int ans = 0;
	bs best_set;

	for (int K_msk = 0; K_msk < (1 << V_K); K_msk++) {
		using dp_t = std::array<std::pair<int, bs>, 16>;
		// returns: depending on whether you can take the root, the cactus child, and the first/last leaf vertex
		const int ROOT = 1;
		const int CACTUS = 2;
		const int FIRST_LEAF = 4;
		const int LAST_LEAF = 8;

		std::fill(depth.begin(), depth.end(), -1);
		depth[0] = 0;
		auto root_res = std::y_combinator([&](auto self, int cur, int prv) -> dp_t {
			dp_t res; res.fill({0, bs()});
			if (K_id[cur] != -1 && !(K_msk & (1 << K_id[cur]))) {
				// promised not to take ourself
			} else if (K_adj[cur] & K_msk) {
				// not allowed to take self
			} else {
				for (int v = 0; v < 16; v++) {
					if (!(v & ROOT)) continue;
					if (is_leaf[cur] && !(v & FIRST_LEAF)) continue;
					if (is_leaf[cur] && !(v & LAST_LEAF)) continue;
					if (is_cactus[cur] && !(v & CACTUS)) continue;
					res[v].first = T[cur];
					res[v].second[cur] = true;
				}
			}

			bool has_leaf = is_leaf[cur];

			lowval[cur] = depth[cur];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					depth[nxt] = depth[cur] + 1;
					auto ch_res = self(nxt, cur);
					if (lowval[nxt] < depth[cur]) {
						lowval[cur] = std::min(lowval[cur], lowval[nxt]);
						// nxt is the cactus child, let's propagate
						assert(!is_cactus[cur]);
					}

					dp_t n_res; n_res.fill({0, bs()});
					for (int v_cur = 0; v_cur < 16; v_cur++) {
						for (int v_nxt = 0; v_nxt < 16; v_nxt++) {
							if ((v_cur & ROOT) && (v_nxt & ROOT)) continue;
							if ((v_cur & LAST_LEAF) && (v_nxt & FIRST_LEAF)) continue;
							if (lowval[nxt] == depth[cur] && (v_cur & ROOT) && (v_nxt & CACTUS)) continue;

							int nv = (v_cur & ROOT) | ((has_leaf ? v_cur : v_nxt) & FIRST_LEAF) | (v_nxt & LAST_LEAF);

							if (lowval[nxt] < depth[cur]) nv |= (v_nxt & CACTUS);
							else nv |= (v_cur & CACTUS);

							int cnd = res[v_cur].first + ch_res[v_nxt].first;
							if (cnd > n_res[nv].first) {
								n_res[nv].first = cnd;
								n_res[nv].second = res[v_cur].second | ch_res[v_nxt].second;
							}
						}
					}
					res = n_res;
					has_leaf = true;
				} else if (depth[nxt] < depth[cur]) {
					// cactus backedge, don't need to do anything
					lowval[cur] = std::min(lowval[cur], depth[nxt]);
					assert(is_cactus[cur]);
				} else {
					// cactus downedge, just ignore it
				}
			}
			return res;
		})(0, -1);

		for (int v = 0; v < 16; v++) {
			if ((v & FIRST_LEAF) && (v & LAST_LEAF)) continue;
			if (root_res[v].first > ans) {
				ans = root_res[v].first;
				best_set = root_res[v].second;
			}
		}
	}

	std::vector<int> our_set; our_set.reserve(N);
	for (int i = 0; i < N; i++) {
		if (best_set[i]) our_set.push_back(i);
	}
	cout << ans << ' ' << our_set.size() << '\n';
	for (int z = 0; z < int(our_set.size()); z++) {
		cout << our_set[z] << " \n"[z+1==int(our_set.size())];
	}

	return 0;
}
