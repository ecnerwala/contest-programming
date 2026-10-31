#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		// the marginal is (100 + L (K+1)) / (100 + LK) = 1 + L / (100+LK)
		std::vector<int> L(N);
		for (auto& l : L) cin >> l;

		auto get_smaller = [&](int a) -> int {
			// number of things of the form 100/L + k < a where 0 <= k
			// k < a - 100/l <=> k < a - floor(100 / l)
			int cnt = 0;
			for (auto l : L) {
				cnt += std::max(0, a - 100 / l);
			}
			return cnt;
		};

		int mi = -1, ma = 200 + K;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (get_smaller(md) <= K) {
				mi = md;
			} else {
				ma = md;
			}
		}
		int a = mi;
		std::vector<int> cnt(N);
		int tot_cnt = 0;
		std::vector<std::pair<double, int>> extra; extra.reserve(N);
		for (int i = 0; i < N; i++) {
			int l = L[i];
			cnt[i] = std::max(0, a - 100 / l);
			extra.push_back({100. / l + cnt[i], i});
			tot_cnt += cnt[i];
		}
		assert(tot_cnt <= K);
		std::sort(extra.begin(), extra.end());
		for (int z = 0; z < K - tot_cnt; z++) {
			cnt[extra[z].second]++;
		}
		double ans = 1;
		for (int i = 0; i < N; i++) {
			ans *= (1 + L[i] * cnt[i] / 100.);
		}
		cout << scientific << setprecision(20) << ans << '\n';
	}

	return 0;
}
