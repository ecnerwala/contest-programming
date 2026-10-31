#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		std::vector<int> cnt(N);
		for (auto& a : A) { cin >> a; a--; cnt[a] ++; }
		if (*max_element(cnt.begin(), cnt.end()) > K) {
			cout << -1 << '\n';
			continue;
		}

		std::vector<int> dp; dp.reserve(K+1);
		dp.push_back(-1);

		for (int l = 0; l < N; l++) {
			int good_v = N;
			int good_idx = int(dp.size());

			{
				int idx = int(dp.size());
				int mx = 0;
				for (int v = N-1; v >= 0; v--) {
					if (cnt[v] == 0) continue;
					while (dp[idx-1] > v) idx--;
					if (idx + mx <= K) {
						good_v = v;
						good_idx = idx;
					}
					mx = max(mx, cnt[v]);
				}
			}

			assert(good_v != N);
			cout << good_v+1 << " \n"[l+1==N];
			if (good_idx == int(dp.size())) dp.push_back(good_v);
			else dp[good_idx] = good_v;
			cnt[good_v]--;
		}
	}

	return 0;
}
