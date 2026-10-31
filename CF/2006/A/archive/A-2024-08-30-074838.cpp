#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> deg(N);
		deg[0] += 2;
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			deg[u]++, deg[v]++;
		}
		std::string S; cin >> S;
		assert(N >= 2);
		std::array<int, 2> leaf_cnt{0, 0};
		int leaf_unknown = 0;
		int other_unknown = 0;
		for (int i = 1; i < N; i++) {
			if (deg[i] == 1) {
				if (S[i] == '?') leaf_unknown++;
				else leaf_cnt[S[i] - '0']++;
			} else {
				if (S[i] == '?') other_unknown++;
			}
		}

		cerr << leaf_cnt[0] << ' ' << leaf_cnt[1] << ' ' << leaf_unknown << ' ' << S[0] << '\n';

		// take the first turn
		if (S[0] == '?') {
			if (leaf_cnt[0] == leaf_cnt[1]) {
				if (other_unknown & 1) {
					cout << leaf_cnt[0] + (leaf_unknown + 1) / 2 << '\n';
				} else {
					cout << leaf_cnt[0] + leaf_unknown / 2 << '\n';
				}
			} else {
				cout << std::max(leaf_cnt[0], leaf_cnt[1]) + leaf_unknown / 2 << '\n';
			}
		} else {
			assert(S[0] == '0' || S[0] == '1');
			int root_val = (S[0] - '0');
			cout << (leaf_unknown + 1) / 2 + leaf_cnt[!root_val] << '\n';
		}
	}

	return 0;
}
