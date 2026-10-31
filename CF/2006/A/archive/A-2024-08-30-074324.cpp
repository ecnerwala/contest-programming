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
		for (int i = 1; i < N; i++) {
			if (deg[i] == 1) {
				if (S[i] == '?') leaf_unknown++;
				else leaf_cnt[S[i] - '0']++;
			}
		}

		cerr << leaf_cnt[0] << ' ' << leaf_cnt[1] << ' ' << leaf_unknown << ' ' << S[0] << '\n';

		// take the first turn
		int root_val;
		if (S[0] == '?') {
			// set it to be different from the majority
			root_val = (leaf_cnt[0] > leaf_cnt[1]) ? 1 : 0;
		} else {
			assert(S[0] == '0' || S[0] == '1');
			root_val = (S[0] - '0');
			if (leaf_unknown) {
				leaf_cnt[!root_val]++;
				leaf_unknown--;
			}
		}
		// now dora takes the first turn
		cout << leaf_unknown / 2 + leaf_cnt[!root_val] << '\n';
	}

	return 0;
}
