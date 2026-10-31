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
	vector<vector<pair<int, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back({e, b});
		adj[b].push_back({e, a});
	}

	int cycle_len = -1;
	int64_t cnt = 0;

	vector<int> depth(N, -1);
	depth[0] = 0;
	vector<int> lowval(N, -1);
	vector<array<int, 2>> stk; stk.reserve(M);

	vector<int> deg(N, 0);

	std::y_combinator([&](auto self, int cur, int prvE) -> void {
		lowval[cur] = depth[cur];
		for (auto [e, nxt] : adj[cur]) {
			if (e == prvE) continue;
			if (depth[nxt] == -1) {
				depth[nxt] = depth[cur]+1;
				int st = int(stk.size());
				stk.push_back({cur, nxt});
				self(nxt, e);
				if (lowval[nxt] == depth[cur]) {
					// we have BCC, so let's pop the edges

					assert(cycle_len != -1);

					int en = int(stk.size());
					int num_edges = en - st;

					int num_big = 0;
					int max_deg = 0;
					for (int i = st; i < en; i++) {
						for (int v : stk[i]) {
							if (++deg[v] == 3) {
								num_big++;
							}
							max_deg = max(max_deg, deg[v]);
						}
					}
					for (int i = st; i < en; i++) {
						for (int v : stk[i]) {
							--deg[v];
						}
					}

					if (num_big > 2) {
						cout << "NIE" << '\n';
						exit(0);
					}

					if (num_big == 0) {
						cnt ++;
					} else if (num_big == 2) {
						if (num_edges * 2 != max_deg * cycle_len) {
							cout << "NIE" << '\n';
							exit(0);
						}

						cnt += int64_t(max_deg) * int64_t(max_deg - 1) / 2;
					} else assert(false);

					stk.resize(st);
				} else if (lowval[nxt] == depth[nxt]) {
					// e is a bridge, so we can skip it
					stk.resize(st);
				}

				lowval[cur] = min(lowval[cur], lowval[nxt]);
			} else if (depth[nxt] < depth[cur]) {
				stk.push_back({cur, nxt});

				lowval[cur] = min(lowval[cur], depth[nxt]);

				int cur_len = depth[cur] - depth[nxt] + 1;
				if (cycle_len == -1) {
					cycle_len = cur_len;
				} else if (cycle_len != cur_len) {
					cout << "NIE" << '\n';
					exit(0);
				}
			} else {
				// This is the top of an ancestor edge, so we can skip
			}
		}
	})(0, -1);

	if (cycle_len == -1) {
		cout << "BRAK" << '\n';
	} else {
		int MOD = int(1e9)+7;
		cnt %= MOD;
		cnt *= cycle_len * 2;
		cnt %= MOD;

		cout << "TAK" << '\n';
		cout << cycle_len << ' ' << cnt << '\n';
	}

	return 0;
}

// Which biconnected components have only 1 simple cycle length?
//
// Cycles or
//
//     *
//   //|\
//   ****
//   ||||
//   ****
//   ||||
//   ****
//   \\|/
//     *
//
// The number of cycles is # paths choose 2
