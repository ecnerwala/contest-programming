#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> C(N);
		for (auto& v : C) cin >> v;
		int64_t ans = -1e18;
		for (int z = 0; z < 2 && z < N; z++) {
			int64_t cnd = C[z];
			for (int i = z + 2; i < N; i += 2) {
				cnd = std::max<int64_t>({cnd, C[i], cnd + C[i]});
			}
			ans = std::max(ans, cnd);
		}
		cout << ans << '\n';
	}


	return 0;
}
