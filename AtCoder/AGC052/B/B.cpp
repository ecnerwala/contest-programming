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

	struct edge_t {
		int dest;
		int w1, w2;
	};
	vector<vector<edge_t>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		int w1, w2; cin >> w1 >> w2;
		adj[u].push_back(edge_t{v, w1, w2});
		adj[v].push_back(edge_t{u, w1, w2});
	}

	vector<int> v1(N);
	vector<int> v2(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		for (auto e : adj[cur]) {
			int nxt = e.dest;
			if (nxt == prv) continue;
			v1[nxt] = v1[cur] ^ e.w1;
			v2[nxt] = v2[cur] ^ e.w2;
			self(nxt, cur);
		}
	})(0, -1);

	int tot1 = 0;
	int tot2 = 0;
	for (int i = 0; i < N; i++) {
		tot1 ^= v1[i];
		tot2 ^= v2[i];
	}
	for (int i = 0; i < N; i++) {
		v1[i] ^= tot1;
		v2[i] ^= tot2;
	}
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	cout << (v1 == v2 ? "YES" : "NO") << '\n';

	return 0;
}

// We can find the total xor for each vertex such that each bit is even in total
//
// Initially all the v_i are equal to xor of path to root of w_1
// The goal is to make them equal to xor of path to root of w_2
