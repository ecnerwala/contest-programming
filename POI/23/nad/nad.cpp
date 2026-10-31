#include<bits/stdc++.h>

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

	int N; cin >> N;
	if (N == 1) {
		cout << 0 << '\n';
		exit(0);
	}

	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	struct dp_value {
		// i = # of transmitters at root
		// j = # of transmitters adjacent to the root
		// k = # of transmitters needed by edges adjacent to the root
		int val[3][3][3];
	};

	const int INF = 1e9;

	auto root = std::y_combinator([&](auto self, int cur, int prv) -> dp_value {
		dp_value res;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					res.val[i][j][k] = INF;
				}
			}
		}
		res.val[0][0][0] = 0;
		res.val[1][0][0] = 1;
		res.val[2][0][0] = 2;

		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			dp_value ch = self(nxt, cur);
			dp_value nres;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						nres.val[i][j][k] = INF;
					}
				}
			}

			for (int pi = 0; pi < 3; pi++) {
				for (int pj = 0; pj < 3; pj++) {
					for (int pk = 0; pk < 3; pk++) {
						int pv = res.val[pi][pj][pk];
						if (pv == INF) continue;
						for (int ci = 0; ci < 3; ci++) {
							for (int cj = 0; cj < 3; cj++) {
								for (int ck = 0; ck < 3; ck++) {
									int cv = ch.val[ci][cj][ck];
									if (cv == INF) continue;

									// not enough to fill in child
									if (pi < ck) continue;

									int ni = pi;
									int nj = min(pj + ci, 2);
									int nk = max(0, pk - ci);
									if (pi == 0 && ci == 0) {
										nk = max(nk, 2 - (pj + cj));
									}

									setmin(nres.val[ni][nj][nk], pv + cv);
								}
							}
						}
					}
				}
			}

			res = nres;
		}

		/*
		cerr << "finished " << cur << ' ' << prv << '\n';
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (res.val[i][j][k] != INF) {
						cerr << cur << ' ' << i << ' ' << j << ' ' << k << ' ' << res.val[i][j][k] << '\n';
					}
				}
			}
		}
		*/
		return res;
	})(0, -1);

	int ans = INF;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (k > 0) continue;
				setmin(ans, root.val[i][j][k]);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}

// states of a subtree:
//
// 0: root node has 2 transmitters
// 1: root node has 1 transmitter
// 2: root node has 0 transmitters, and 2 neighbors of root have transmitters
// 3: root node has 0 transmitters, and 1 neighbor of root has transmitter
// 4: root node has 0 transmitters, and 0 neighbors of root has transmitter
// In cases 3 and 4, we also need to know if there's a need-1 edge or a need-2 edge
