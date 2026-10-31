#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		const int L = N * (N-1) / 2;
		std::vector<int> B(L);
		for (auto& b : B) cin >> b;
		std::vector<int> dp_fewest(L+1, -1);
		dp_fewest[0] = N;
		std::vector<int> dp_smallest(L+1, -1);
		dp_smallest[0] = 0;
		for (int i = N; i >= 2; i--) {
			int dl = i * (i-1) / 2;
			for (int l = 0; l + dl <= L; l++) {
				if (i <= dp_fewest[l]) {
					dp_fewest[l + dl] = std::max(dp_fewest[l + dl], dp_fewest[l] - i);
					dp_smallest[l + dl] = i;
				}
			}
		}
		cerr << N << ' ' << L << '\n';
		for (int i = 0; i <= L; i++) {
			if (dp_fewest[i] == -1) {
				cerr << i << '\n';
				int j = i+1;
				while (dp_fewest[j] == -1) j++;
				int k = i-1;
				while (dp_fewest[k] == -1) k--;
				cerr << i << ' ' << j << ' ' << k << ' ' << dp_fewest[j] << '\n';
				assert(j - k == dp_fewest[j]);
			}
		}
	}

	return 0;
}
