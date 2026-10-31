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

	int N; cin >> N;
	std::vector<int64_t> R(N); for (auto& r : R) cin >> r;
	std::vector<int64_t> C(N); for (auto& c : C) cin >> c;
	std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; int64_t l; cin >> u >> v >> l; u--, v--;
		adj[u].push_back({v, l});
		adj[v].push_back({u, l});
	}

	struct node_t {
		int par;
		int jump_par;
		int jump_par_len;
		std::pair<int64_t, int64_t> val;
	};
	std::vector<node_t> nodes(N);

	auto do_cuts = [&](int cur, std::pair<int64_t, int64_t> nval) -> int {
		auto should_cut = [&](int a) -> bool {
			if (a == -1) return false;
			auto v1 = nodes[a].val;
			if (nval.second >= v1.second) {
				return true;
			}
			if (nodes[a].par == -1) return false;
			auto v0 = nodes[nodes[a].par].val;
			pair<int64_t, int64_t> s0{v1.first - v0.first, v0.second - v1.second};
			pair<int64_t, int64_t> s1{nval.first - v1.first, v1.second - nval.second};
			assert(s0.first >= 0 && s0.second >= 0);
			assert(s1.first >= 0 && s1.second >= 0);
			__int128 cross = __int128(s0.first) * __int128(s1.second) - __int128(s0.second) * __int128(s1.first);
			return cross <= 0;
		};

		if (!should_cut(cur)) return cur;
		while (true) {
			if (should_cut(nodes[cur].jump_par)) {
				cur = nodes[cur].jump_par;
			} else if (nodes[cur].par != nodes[cur].jump_par && should_cut(nodes[cur].par)) {
				cur = nodes[cur].par;
			} else {
				return nodes[cur].par;
			}
		}
	};

	std::vector<double> ans(N);
	std::y_combinator([&](auto self, int cur, int prv, int64_t depth) -> void {
		{
			int cnd = do_cuts(prv, {depth, C[cur]});
			if (cnd == -1) ans[cur] = -1;
			else ans[cur] = double(depth - nodes[cnd].val.first) / double(nodes[cnd].val.second - C[cur]);
		}
		{
			int p = do_cuts(prv, nodes[cur].val = {depth, R[cur]});
			nodes[cur].par = p;
			if (p == -1) {
				nodes[cur].jump_par = -1;
				nodes[cur].jump_par_len = 0;
			} else if (nodes[p].jump_par_len > 0 && nodes[nodes[p].jump_par].jump_par_len == nodes[p].jump_par_len){
				nodes[cur].jump_par = nodes[nodes[p].jump_par].jump_par;
				nodes[cur].jump_par_len = 1 + 2 * nodes[p].jump_par_len;
			} else {
				nodes[cur].jump_par = nodes[cur].par;
				nodes[cur].jump_par_len = 1;
			}
		}
		for (auto [nxt, len] : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur, depth + len);
		}
	})(0, -1, 0);

	for (int i = 1; i < N; i++) {
		if (ans[i] < -0.5) cout << "Unreachable" << '\n';
		else cout << fixed << setprecision(12) << ans[i] << '\n';
	}

	return 0;
}
