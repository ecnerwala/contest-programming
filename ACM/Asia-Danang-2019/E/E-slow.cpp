#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;

		vector<int> A(N); for (auto& a : A) cin >> a;

		for (int q = 0; q < Q; q++) {
			int l, r, u, v; cin >> l >> r >> u >> v; l--, u--;
			assert(0 <= l && l < r && r <= N);
			vector<int> S(A.begin()+l, A.begin()+r);
			std::sort(S.begin(), S.end());
			array<int, 3> f{0,0,0};
			for (int i = u; i < v; i++) {
				for (int z = 0; z < 3; z++) {
					if (i-z >= u && S[i-z] == S[i]) f[z] |= S[i];
				}
			}
			int64_t ans = int64_t(f[0]) + int64_t(f[1]) + int64_t(f[2]);
			cout << ans << '\n';
		}
	}

	return 0;
}
