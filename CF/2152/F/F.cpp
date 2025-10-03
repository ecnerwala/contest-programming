#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Z; cin >> N >> Z;
		std::vector<int> X(N); for (auto& x : X) cin >> x;
		int Q; cin >> Q;
		std::vector<std::array<int, 2>> queries(Q);
		for (auto& [l, r] : queries) { cin >> l >> r; l--; }

		std::vector<int> depth(N+1);
		std::vector<int> par(N);
		std::vector<int> jump_loc(N);
		std::vector<int> jump_dist(N);
		for (int i = N-1, j = N; i >= 0; i--) {
			while (X[j-1] - X[i] > Z) j--;
			assert(j > i);
			par[i] = j;
			depth[i] = depth[j] + 1;
			if (j != N && jump_loc[j] != N && jump_dist[j] == jump_dist[jump_loc[j]]) {
				jump_dist[i] = 1 + jump_dist[j] * 2;
				jump_loc[i] = jump_loc[jump_loc[j]];
			} else {
				jump_dist[i] = 1;
				jump_loc[i] = j;
			}
		}

		std::vector<int> depth2(N+1);
		std::vector<int> par2(N);
		std::vector<int> jump2_loc(N);
		std::vector<int> jump2_dist(N);
		std::vector<int> jump2_len(N);
		for (int i = N-1; i >= 0; i--) {
			// find the lca of i and i+1
			int a = i, b = i+1;
			assert(depth[a] >= depth[b]);
			assert(depth[a] <= depth[b]+1);
			if (depth[a] > depth[b]) {
				a = par[a];
			}
			assert(depth[a] == depth[b]);
			if (a != b) {
				while (par[a] != par[b]) {
					assert(jump_dist[a] == jump_dist[b]);
					if (jump_loc[a] != jump_loc[b]) {
						a = jump_loc[a];
						b = jump_loc[b];
					} else {
						a = par[a];
						b = par[b];
					}
				}
				a = par[a], b = par[b];
			}
			assert(a == b);
			int j = a;
			depth2[i] = depth2[j] + (depth[i] + depth[i+1] - 2 * depth[j]);
			par2[i] = j;
			if (j != N && jump2_loc[j] != N && jump2_dist[j] == jump2_dist[jump2_loc[j]]) {
				jump2_dist[i] = 1 + jump2_dist[j] * 2;
				jump2_loc[i] = jump2_loc[jump2_loc[j]];
			} else {
				jump2_dist[i] = 1;
				jump2_loc[i] = j;
			}
		}

		for (auto [l, r] : queries) {
			assert(l < r);

			int ans = 0;
			{
				int cur = l;
				assert(cur < r);
				while (par2[cur] < r) {
					if (jump2_loc[cur] < r) {
						cur = jump2_loc[cur];
					} else {
						cur = par2[cur];
					}
				}
				ans += depth2[l] - depth2[cur];
				l = cur;
			}
			assert(par2[l] >= r);
			assert(l < r);

			for (int st : {l, l+1}) {
				if (st >= r) {
					continue;
				}
				int cur = st;
				while (cur < r) {
					if (jump_loc[cur] < r) {
						cur = jump_loc[cur];
					} else {
						cur = par[cur];
					}
				}
				ans += depth[cur] - depth[st];
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
