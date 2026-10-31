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
		int N, M, Q; cin >> N >> M >> Q;
		std::unordered_map<int, std::vector<std::array<int, 2>>> edges_map;
		for (int e = 0; e < M; e++) {
			int u, v, k; cin >> u >> v >> k; u--, v--, k--;
			edges_map[k].push_back({u, v});
		}
		std::vector<std::vector<std::array<int, 2>>> edges; edges.reserve(edges_map.size());
		for (auto& [k, v] : edges_map) {
			edges.emplace_back(std::move(v));
		}
		edges_map = {};

		std::vector<std::array<int, 2>> queries(Q);
		for (auto& [x, y] : queries) {
			cin >> x >> y; x--, y--;
		}
		std::vector<int> num_broken(Q);

		{
			std::vector<int> par(N, -1);
			std::vector<std::pair<int, int>> ops; ops.reserve(N);
			auto get_par = [&](int a) -> int {
				while (par[a] >= 0) {
					a = par[a];
				}
				return a;
			};
			auto merge = [&](int a, int b) -> bool {
				a = get_par(a), b = get_par(b);
				if (a == b) return false;
				if (par[a] > par[b]) std::swap(a, b);
				ops.push_back({b, par[b]});
				par[a] += par[b];
				par[b] = a;
				return true;
			};
			auto revert_to = [&](int s) {
				while (int(ops.size()) > s) {
					auto [b, vb] = ops.back();
					ops.pop_back();
					int a = par[b];
					par[b] = vb;
					par[a] -= vb;
				}
			};

			std::y_combinator([&](auto self, int lo, int hi) -> void {
				if (hi - lo == 1) {
					for (int q = 0; q < int(queries.size()); q++) {
						auto [x, y] = queries[q];
						if (get_par(x) != get_par(y)) {
							num_broken[q]++;
						}
					}
					return;
				}
				int md = (lo + hi) / 2;
				{
					int s = int(ops.size());
					for (int k = md; k < hi; k++) {
						for (auto [x, y] : edges[k]) merge(x, y);
					}
					self(lo, md);
					revert_to(s);
				}
				{
					int s = int(ops.size());
					for (int k = lo; k < md; k++) {
						for (auto [x, y] : edges[k]) merge(x, y);
					}
					self(md, hi);
					revert_to(s);
				}
			})(0, int(edges.size()));
		}

		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		for (const auto& es : edges) {
			for (auto [x, y] : es) {
				merge(x, y);
			}
		}

		std::vector<int> comp_colors(N);
		std::vector<int> last_comp_color(N, -1);
		for (int i = 0; i < int(edges.size()); i++) {
			for (auto [x, y] : edges[i]) {
				x = get_par(x);
				if (last_comp_color[x] != i) {
					last_comp_color[x] = i;
					comp_colors[x] ++;
				}
			}
		}

		int tot_ans = 0;
		for (int q = 0; q < Q; q++) {
			auto [x, y] = queries[q];
			int cx = get_par(x), cy = get_par(y);
			if (cx != cy) {
				// bad, can't connect at all
				assert(num_broken[q] == int(edges.size()));
			} else {
				int num_colors = comp_colors[cx];
				if (num_colors % 2 == 1 || num_broken[q] < num_colors) {
					tot_ans ++;
				}
			}
		}
		cout << "Case #" << case_num << ": " << tot_ans << '\n';
	}

	return 0;
}
