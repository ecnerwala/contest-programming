#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int64_t> F(N);
		for (auto& f : F) cin >> f;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }

		const int64_t INF = 1e18;
		int64_t ans = 0;
		std::vector<int64_t> ch_min(N, INF);
		for (int i = N-1; i >= 0; i--) {
			int64_t v = F[i];
			if (ch_min[i] != INF) {
				ans -= ch_min[i];
				v = std::max(v, ch_min[i]);
			}
			ans += v;
			if (P[i] != -1) {
				ch_min[P[i]] = std::min(ch_min[P[i]], v);
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
