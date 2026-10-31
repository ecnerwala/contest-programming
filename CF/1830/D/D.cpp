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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		auto ans = std::y_combinator([&](auto self, int cur, int prv) -> std::array<std::vector<int>, 2> {
			std::array<std::vector<int>, 2> res;
			res[0].push_back(0);
			res[0].push_back(1);
			res[1].push_back(0);
			res[1].push_back(2);
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				auto ch = self(nxt, cur);
				std::array<std::vector<int>, 2> nres;
				for (int z = 0; z < 2; z++) {
					nres[z].resize(res[z].size() + ch[z].size() - 1, 2*N+1);
					for (int i = 1; i < int(res[z].size()); i++) {
						for (int j = 0; j < int(ch[z].size()); j++) {
							setmin(nres[z][i+j], res[z][i] + ch[z][j] + i * j * (z+1));
						}
					}
					while (nres[z].back() > 2*N) {
						nres[z].pop_back();
					}
				}
				res = nres;
			}
			res[0][0] = *std::min_element(res[1].begin()+1, res[1].end());
			res[1][0] = *std::min_element(res[0].begin()+1, res[0].end());
			return res;
		})(0, -1);
		int res = std::min(ans[0][0], ans[1][0]);
		cout << int64_t(N) * int64_t(N+1) - res << '\n';
	}

	return 0;
}
