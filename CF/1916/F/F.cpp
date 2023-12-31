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
		int N1, N2, M; cin >> N1 >> N2 >> M;
		int N = N1 + N2;
		std::vector<std::array<int, 2>> edges(M);
		std::vector<std::vector<int>> adj(N);
		for (auto& [u, v] : edges) {
			cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> lowval(N);
		std::vector<int> depth(N, -1);
		std::vector<std::vector<int>> ch(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			depth[cur] = prv != -1 ? depth[prv] + 1 : 0;
			lowval[cur] = depth[cur];
			ch[cur].reserve(adj[cur].size());
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					ch[cur].push_back(nxt);
					self(nxt, cur);
					lowval[cur] = std::min(lowval[cur], lowval[nxt]);
				} else if (depth[nxt] < depth[cur]) {
					lowval[cur] = std::min(lowval[cur], depth[nxt]);
				} else {
					// down edge
				}
			}
		})(0, -1);
		// true is after, false is before
		std::vector<bool> edge_dir(N, false);
		std::vector<int> lst_nxt(N, -1);
		auto lst = std::y_combinator([&](auto self, int cur) -> std::array<int, 2> {
			std::array<int, 2> res{cur, cur};
			for (int nxt : ch[cur]) {
				// if lowval[nxt] == cur, this is the top of a bcc, so we get lots of freedom
				assert(lowval[nxt] <= depth[cur]);
				bool d = lowval[nxt] == depth[cur] ? true : !edge_dir[lowval[nxt]];
				edge_dir[depth[cur]] = d;

				auto ch_res = self(nxt);

				// Join res and ch
				if (!d) std::swap(res, ch_res);
				lst_nxt[std::exchange(res[1], ch_res[1])] = ch_res[0];
			}
			return res;
		})(0);

		int cur = lst[0];
		for (int l : {N1, N2}) {
			for (int i = 0; i < l; i++) {
				cout << cur+1 << " \n"[i+1==l];
				cur = lst_nxt[cur];
			}
		}
	}

	return 0;
}
