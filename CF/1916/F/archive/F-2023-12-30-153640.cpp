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
			cin >> u >> v;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> lowval(N);
		std::vector<int> low_ch(N);
		std::vector<int> depth(N);
		std::vector<std::vector<int>> ch(N);
		std::vector<std::vector<int>> ears; ears.reserve(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			depth[cur] = prv != -1 ? depth[prv] + 1 : 0;
			lowval[cur] = depth[cur];
			low_ch[cur] = -1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					ch[cur].push_back(nxt);
					self(nxt, cur);
					if (lowval[nxt] < lowval[cur]) {
						lowval[cur] = lowval[nxt];
						low_ch[cur] = nxt;
					}
				} else if (depth[nxt] < depth[cur]) {
					if (depth[nxt] < lowval[cur]) {
						lowval[cur] = depth[nxt];
						low_ch[cur] = nxt;
					}
				} else {
					// down edge
				}
			}
			if (prv != -1) {
				assert(lowval[cur] < depth[cur]);
				assert(low_ch[cur] != -1);
			}
			if (prv == 0) {
				low_ch[cur] = -1;
			}
			if (prv != -1) {
				for (int nxt : ch[cur]) {
					if (nxt == low_ch[cur]) continue;
					ears.push_back({});
					ears.back().push_back(cur);
					for (int a = nxt; true; a = low_ch[a]) {
						ears.back().push_back(a);
						if (depth[a] < depth[cur]) break;
					}
					assert(ears.back().front() != ears.back().back());
				}
			}
		})(0, -1);
		std::vector<std::vector<int>> ear_ids(N);
	}

	return 0;
}
