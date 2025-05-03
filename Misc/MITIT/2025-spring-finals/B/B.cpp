#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 6>> V(N);
		for (int i = 0; i < N; i++) {
			V[i][0] = 0;
			for (int z = 0; z < 5; z++) {
				int a; cin >> a;
				V[i][z+1] = V[i][z] + a;
			}
			assert(V[i][5] == N);
		}

		std::vector<int> ans(N+1);
		ans[0] = 1;
		for (int z = 4; z >= 0; z--) {
			cerr << "solve " << z << '\n';
			std::vector<std::array<int, 2>> B(N);
			std::vector<int> lefts(N+1);
			std::vector<int> rights(N+1);
			for (int i = 0; i < N; i++) {
				B[i] = {V[i][z], V[i][z+1]};
				lefts[i] = V[i][z];
				rights[i] = V[i][z+1];
			}
			lefts[N] = N+1;
			rights[N] = -1;
			std::sort(lefts.begin(), lefts.end());
			std::sort(rights.begin(), rights.end());
			std::vector<bool> can_right(N+1);
			for (int i = 0; i <= N; i++) can_right[i] = lefts[i] > i;
			std::vector<bool> can_left(N+1);
			for (int i = 0; i <= N; i++) can_left[i] = rights[i] < i;

			std::vector<std::vector<std::pair<int, int>>> adj(N+1);
			for (int e = 0; e < N; e++) {
				auto [u, v] = B[e];
				adj[u].push_back({v, e});
				adj[v].push_back({u, e});
				cerr << "edge " << u << ' ' << v << '\n';
			}
			std::vector<int> depth(N+1, -1);
			std::vector<int> lowval(N+1, -1);
			std::vector<bool> is_bridge(N, false);
			for (int i = 0; i <= N; i++) {
				if (depth[i] == -1) {
					depth[i] = 0;
					[&](this auto&& self, int cur, int prv_e) -> void {
						lowval[cur] = depth[cur];
						for (auto [nxt, e] : adj[cur]) {
							if (e == prv_e) continue;
							if (depth[nxt] == -1) {
								depth[nxt] = depth[cur] + 1;
								self(nxt, e);
								lowval[cur] = std::min(lowval[cur], lowval[nxt]);
								if (lowval[nxt] == depth[nxt]) {
									// bridge
									is_bridge[e] = true;
								}
							} else if (depth[nxt] <= depth[cur]) {
								lowval[cur] = std::min(lowval[cur], depth[nxt]);
							}
						}
					}(i, -1);
				}
			}
			std::vector<int> skip_til(N+1, -1);
			for (int e = 0; e < N; e++) {
				if (!is_bridge[e]) {
					auto [u, v] = B[e];
					skip_til[u] = std::max(skip_til[u], v);
					cerr << "nonbridge " << u << ' ' << v << '\n';
				}
			}
			for (int i = 0; i <= N; i++) { cerr << can_left[i] << ' '; } cerr << '\n';
			for (int i = 0; i <= N; i++) { cerr << can_right[i] << ' '; } cerr << '\n';
			for (int i = 0; i < N; i++) {
				if (skip_til[i] >= i+1) {
					skip_til[i+1] = std::max(skip_til[i+1], skip_til[i]);
				}
			}
			std::vector<int> nans(N+1);
			int cur_dp = 0;
			for (int i = 0; i <= N; i++) {
				if (skip_til[i] > i) continue;
				if (skip_til[i] != -1) {
					cur_dp = 0;
				}
				cur_dp ^= can_left[i] ? ans[i] : 0;

				nans[i] = can_right[i] ? cur_dp : 0;
			}
			ans = std::move(nans);
		}
		cout << ans[N] << '\n';
	}

	return 0;
}
